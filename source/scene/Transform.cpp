#include "scene/Transform.h"

namespace epsilon
{
	Transform::Ptr Transform::Create()
	{
		return std::make_shared<Transform>(private_struct());
	}

	Transform::Transform(const private_struct &) :
								position(Vector3::ZERO),
								orientation(Quaternion::IDENTITY),
								scale(Vector3::ONE),
								derivedPosition(Vector3::ZERO),
								derivedOrientation(Quaternion::IDENTITY),
								derivedScale(Vector3::ONE),
								inheritOrientation(true),
								inheritScale(true),
								cachedTransformOutOfDate(true),
								needParentUpdate(false),
								needChildUpdate(false),
								parentNotified(false),
								NodeComponent("Transform")
	{
		cachedTransform = Matrix4();
		children = std::make_shared<TransformList>();
		childrenToUpdate = std::make_shared<TransformList>();
	}


	Transform::~Transform(void)
	{
	}

	void Transform::Destroy()
	{
		// Empty queued updates
		//parent->cancelUpdate(ThisPtr());
		
		// Empty the Transforms children
		RemoveAllChildren();

		childrenToUpdate->clear();

		parent = nullptr;
	}

	Transform::Ptr Transform::SetParentTransform(Transform::Ptr newParent)
	{
		parent = newParent;
		needUpdate();
		return ThisPtr();
	}

	Transform::Ptr Transform::GetParentTransform() 
	{ 
		return parent; 
	}
	
	TransformListPtr Transform::GetChildren() 
	{ 
		return children; 
	}
	
	Transform::Ptr Transform::AddChild(Transform::Ptr childNode)
	{
		// Check if the node is already a child
		TransformList::iterator foundChild = find(children->begin(), children->end(), childNode);
		
		// If it's not already a child
		if ( foundChild == children->end() )
		{
			// if this node isn't the parent
			if ( (childNode->parent != nullptr) && (childNode->parent != ThisPtr() ) )
			{
				// Remove it from it's existing parent
				childNode->parent->RemoveChild(childNode);
			}

			// Set this node as the parent and add it to the children
			childNode->SetParentTransform(ThisPtr());
			children->push_back(childNode);
		}
		return ThisPtr();
	}

	Transform::Ptr Transform::RemoveChild(Transform::Ptr childNode)
	{
		// Check if the node is already a child
		TransformList::iterator foundChild = find(children->begin(), children->end(), childNode);

		// If the node is a child
		if ( foundChild != children->end() )
		{
			children->remove(childNode);
			childNode->parent = nullptr;
		}
		return ThisPtr();
	}

	Transform::Ptr Transform::RemoveAllChildren()
	{
		// Remove all children setting their parent pointers to null
		for ( TransformList::iterator childNode = children->begin(); childNode != children->end(); childNode++)
		{
			(*childNode)->parent = nullptr;
		}
		children->clear();
		return ThisPtr();
	}

	Transform::Ptr Transform::FindChildWithName(std::string name)
	{
		TransformList::iterator foundChildIt;
		Transform::Ptr foundChild;

		foundChildIt = find_if(children->begin(), children->end(), [name](Transform::Ptr trans){ 
			return trans->GetParent()->GetName() == name; 
		});
		
		if ( foundChildIt != children->end() )
		{
			foundChild = (*foundChildIt);
		}
		return foundChild;
	}

	Transform::Ptr Transform::FindChildWithNameRecursive(std::string name)
	{
		Transform::Ptr foundChild;

		for (TransformList::iterator child = children->begin(); child != children->end(); child++)
		{
			if ( (*child)->GetParent()->GetName() == name )
			{
				foundChild = (*child);
				break;
			}
			else if ( (*child)->HasChildren() )
			{
				foundChild = (*child)->FindChildWithNameRecursive(name);
			}
		}
		return foundChild;
	}

	Transform::Ptr Transform::FindChildWithId(long id)
	{
		TransformList::iterator foundChildIt;
		Transform::Ptr foundChild;

		foundChildIt = find_if(children->begin(), children->end(), [id](Transform::Ptr trans){ 
			return trans->GetParent()->GetId() == id; 
		});
		
		if ( foundChildIt != children->end() )
		{
			foundChild = (*foundChildIt);
		}
		return foundChild;
	}

	Transform::Ptr Transform::FindChildWithIdRecursive(long id)
	{
		Transform::Ptr foundChild;

		for (TransformList::iterator child = children->begin(); child != children->end(); child++)
		{
			if ( (*child)->GetParent()->GetId() == id )
			{
				foundChild = (*child);
				break;
			}
			else if ( (*child)->HasChildren() )
			{
				foundChild = (*child)->FindChildWithIdRecursive(id);
			}
		}
		return foundChild;
	}


	const Matrix4& Transform::_getFullTransform(void)
    {
		// Make a transform from the transform components
		if (cachedTransformOutOfDate)
        {
            Matrix4 rot, scaleM, pos, res;
            
			// Rotation & Scale
			rot = Quaternion::IDENTITY.GetMatrix();// derivedOrientation.GetMatrix();
			//rot = orientation.GetMatrix();
			scaleM = Matrix4::CreateScale(derivedScale.x, derivedScale.y, derivedScale.z);
			res = rot * scaleM;

			for (int i = 0; i < 3; i++)
			{
				//cachedTransform[Vector4(res3[i][0],res3[i][1],res3[i][2],0));
				cachedTransform[4 * i] = res[4 * i];
				cachedTransform[4 * i + 1] = res[4 * i + 1];
				cachedTransform[4 * i + 2] = res[4 * i + 2];
				cachedTransform[4 * i + 3] = 0;

			}

			// Translation
			cachedTransform[3] = derivedPosition.x;
			cachedTransform[7] = derivedPosition.y;
			cachedTransform[11] = derivedPosition.z;

			/*
            // Rotation & Scale
            rot = orientation.GetMatrix();
            scaleM = Matrix4::CreateScale(scale.x, scale.y, scale.z);
            res = rot * scaleM;
            
            for (int i = 0; i < 3; i++)
			{
				//cachedTransform[Vector4(res3[i][0],res3[i][1],res3[i][2],0));
                cachedTransform[ 4 * i ]     = res[ 4 * i ];
                cachedTransform[ 4 * i  + 1] = res[ 4 * i + 1];
                cachedTransform[ 4 * i  + 2] = res[ 4 * i + 2];
                cachedTransform[ 4 * i  + 3] = 0;
                
			}
            
            // Translation
            cachedTransform[ 3 ] = position.x;
            cachedTransform[ 7 ] = position.y;
            cachedTransform[ 11 ] = position.z;
			*/
            
            //res += pos;
            
            
            /*
			Matrix3 rot3, scale3, res3;

			// rotation
			orientation.ToRotationMatrix(rot3);
            
			// scale
			scale3 = Matrix3::ZERO;
			scale3[0][0] = scale.x;
			scale3[1][1] = scale.y;
			scale3[2][2] = scale.z;

			res3 = rot3 * scale3;

			for (int i = 0; i < 3; i++)
			{
				cachedTransform.SetRow(i, Vector4(res3[i][0],res3[i][1],res3[i][2],0));
			}

			// Transform
			cachedTransform(0,3) = position.x;
			cachedTransform(1,3) = position.y;
			cachedTransform(2,3) = position.z;
            */

            cachedTransformOutOfDate = false;
        }
        return cachedTransform;
    }

	void Transform::_update(bool updateChildren, bool parentHasChanged)
    {
		// always clear information about parent notification
		parentNotified = false ;

        // Short circuit the off case
		/*
		if (!needParentUpdate && !parentHasChanged )
        {
            return;
        }
		*/

        // See if we should process everyone
        if (needParentUpdate || parentHasChanged)
        {
            // Update transforms from parent
            _updateFromParent();
		}

		//if (updateChildren)
		//{
			if (needChildUpdate || parentHasChanged)
			{
				for_each(children->begin(), children->end(), [](Transform::Ptr child){
					child->_update(true, true);
				});
			}
			else
			{
				// Just update selected children
				for_each(childrenToUpdate->begin(), childrenToUpdate->end(), [](Transform::Ptr child){
					child->_update(true, false);
				});
			}
			childrenToUpdate->empty();
			needChildUpdate = false;
		//}

		

    }
	//-----------------------------------------------------------------------
	void Transform::_updateFromParent(void)
	{
		updateFromParentImpl();
	}
    //-----------------------------------------------------------------------
    void Transform::updateFromParentImpl(void)
    {
        if (parent)
        {
            // Update orientation
            const Quaternion& parentOrientation = parent->_getDerivedOrientation();
            if (inheritOrientation)
            {
                // Combine orientation with that of parent
                derivedOrientation = parentOrientation * orientation;
            }
			else
            {
                // No inheritence
                derivedOrientation = orientation;
            }

            // Update scale
            const Vector3& parentScale = parent->_getDerivedScale();
            if (inheritScale)
            {
                // Scale own position by parent scale, NB just combine
                // as equivalent axes, no shearing
				derivedScale[0] = parentScale.x * scale.x;
				derivedScale[1] = parentScale.x * scale.y;
				derivedScale[2] = parentScale.x * scale.z;
            }
            else
            {
                // No inheritence
                derivedScale = scale;
            }

            // Change position vector based on parent's orientation & scale
			derivedPosition[0] = parentScale.x * position.x;
			derivedPosition[1] = parentScale.y * position.y;
			derivedPosition[2] = parentScale.z * position.z;

			derivedPosition = parentOrientation.Rotate(derivedPosition);

            // Add altered position vector to parents
            derivedPosition += parent->_getDerivedPosition();
        }
        else
        {
            // Root node, no parent
            derivedOrientation = orientation;
            derivedPosition = position;
            derivedScale = scale;
        }

		// Update transform directional vectors
		forward = derivedOrientation * Vector3::FORWARD;
		forward.Normalise();

		up = derivedOrientation * Vector3::UP;
		up.Normalise();

		right = derivedOrientation * Vector3::RIGHT;
		right.Normalise();

		cachedTransformOutOfDate = true;
		needParentUpdate = false;

    }
	//-----------------------------------------------------------------------
	const Quaternion& Transform::GetLocalOrientation() const
	{
		return orientation;
	}
	//-----------------------------------------------------------------------
    const Quaternion& Transform::GetOrientation() const
    {
        return derivedOrientation;
    }

	//-----------------------------------------------------------------------
	Transform::Ptr Transform::SetLocalOrientation(const Quaternion & q)
	{
		orientation = q;
		orientation.Normalise();
		needUpdate();
		return ThisPtr();
	}
	//-----------------------------------------------------------------------
	Transform::Ptr Transform::SetLocalOrientation(float w, float x, float y, float z)
	{
		orientation.w = w;
		orientation.x = x;
		orientation.y = y;
		orientation.z = z;
		needUpdate();
		return ThisPtr();
	}

    //-----------------------------------------------------------------------
    Transform::Ptr Transform::SetOrientation( const Quaternion & q )
    {
		if (parent)
		{
			orientation = (parent->_getDerivedOrientation() * q) * q.Inverse();
			
		}
		else
		{
			orientation = q;
		}
		orientation.Normalise();
        needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::SetOrientation( float w, float x, float y, float z)
    {
		SetOrientation(Quaternion(x, y, z, w));
        needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::ResetOrientation(void)
    {
        orientation = Quaternion::IDENTITY;
        needUpdate();
		return ThisPtr();
    }

	//-----------------------------------------------------------------------
	Transform::Ptr Transform::SetLocalPosition(const Vector3& pos)
	{
		position = pos;
		needUpdate(true);
		return ThisPtr();
	}


	//-----------------------------------------------------------------------
	Transform::Ptr Transform::SetLocalPosition(float x, float y, float z)
	{
		Vector3 v(x, y, z);
		SetLocalPosition(v);
		return ThisPtr();
	}

    //-----------------------------------------------------------------------
	Transform::Ptr Transform::SetPosition(const Vector3& pos)
	{
		// Convert World to local
		if (parent)
		{
			position = pos - parent->_getDerivedPosition();
		}
		else
		{
			position = pos;
		}
        needUpdate(true);
		return ThisPtr();
    }


    //-----------------------------------------------------------------------
    Transform::Ptr Transform::SetPosition(float x, float y, float z)
    {
        SetPosition(Vector3(x, y, z));
		return ThisPtr();
    }

    //-----------------------------------------------------------------------
	const Vector3 & Transform::GetLocalPosition(void) const
	{
		return position;
	}//-----------------------------------------------------------------------
	const Vector3 & Transform::GetPosition(void) const
    {
        return derivedPosition;
    }//-----------------------------------------------------------------------
    Transform::Ptr Transform::SetLocalScale(const Vector3& newScale)
    {
        scale = newScale;
        needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
	Transform::Ptr Transform::SetLocalScale(float x, float y, float z)
    {
        scale.x = x;
        scale.y = y;
        scale.z = z;
        needUpdate();
		return ThisPtr();
    }
	Transform::Ptr Transform::SetScale(const Vector3& newScale)
	{
		if (parent)
		{
			scale = newScale / parent->_getDerivedScale();
		}
		else
		{
			scale = newScale;
		}
		needUpdate();
		return ThisPtr();
	}
	//-----------------------------------------------------------------------
	Transform::Ptr Transform::SetScale(float x, float y, float z)
	{
		SetScale(Vector3(x, y, z));
		return ThisPtr();
	}
	//-----------------------------------------------------------------------
	const Vector3 & Transform::GetLocalScale(void) const
	{
		return scale;
	}
    //-----------------------------------------------------------------------
    const Vector3 & Transform::GetScale(void) const
    {
        return derivedScale;
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::SetInheritOrientation(bool inherit)
    {
        inheritOrientation = inherit;
        needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    bool Transform::GetInheritOrientation(void) const
    {
        return inheritOrientation;
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::SetInheritScale(bool inherit)
    {
        inheritScale = inherit;
        needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    bool Transform::GetInheritScale(void) const
    {
        return inheritScale;
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Scaled(const Vector3& multScale)
    {
        scale[0] *= multScale.x;
        scale[1] *= multScale.y;
        scale[0] *= multScale.z;
		needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Scaled(float x, float y, float z)
    {
        scale[0] *= x;
        scale[1] *= y;
        scale[0] *= z;
        needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Matrix3 Transform::GetLocalAxes(void) const
    {
        Vector3 axisX = Vector3::UNIT_X;
        Vector3 axisY = Vector3::UNIT_Y;
        Vector3 axisZ = Vector3::UNIT_Z;

		axisX = orientation.Rotate(axisX);
        axisY = orientation.Rotate(axisY);
        axisZ = orientation.Rotate(axisZ);

        return Matrix3(axisX.x, axisY.x, axisZ.x,
					   axisX.y, axisY.y, axisZ.y,
                       axisX.z, axisY.z, axisZ.z);
    }

	//-----------------------------------------------------------------------
    Transform::Ptr Transform::Translate(const Vector3& d, TransformSpace relativeTo)
    {
        switch(relativeTo)
        {
        case TS_LOCAL:
            // position is relative to parent so transform downwards
            position += orientation.Rotate(d);
        	break;
        case TS_WORLD:
            // position is relative to parent so transform upwards
            if (parent)
            {
				Vector3 n = parent->_getDerivedOrientation().Inverse().Rotate(d);
				Vector3 d = parent->_getDerivedScale();
				position += Vector3(n.x/d.x, n.y/d.y, n.z/d.x);
            }
            else
            {
                position += d;
            }
        	break;
        case TS_PARENT:
            position += d;
            break;
        }
        needUpdate();
		return ThisPtr();

    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Translate(float x, float y, float z, TransformSpace relativeTo)
    {
        Vector3 v(x,y,z);
        Translate(v, relativeTo);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Translate(const Matrix3& axes, const Vector3& move, TransformSpace relativeTo)
    {
        Vector3 derived = axes * move;
        Translate(derived, relativeTo);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Translate(const Matrix3& axes, float x, float y, float z, TransformSpace relativeTo)
    {
        Vector3 d(x,y,z);
        Translate(axes,d,relativeTo);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Roll(const float& angle, TransformSpace relativeTo)
    {
        Rotate(Vector3::UNIT_Z, angle, relativeTo);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Pitch(const float& angle, TransformSpace relativeTo)
    {
        Rotate(Vector3::UNIT_X, angle, relativeTo);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Yaw(const float& angle, TransformSpace relativeTo)
    {
        Rotate(Vector3::UNIT_Y, angle, relativeTo);
		return ThisPtr();

    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Rotate(const Vector3& axis, const float& angle, TransformSpace relativeTo)
    {
        Quaternion q(axis, angle);
        Rotate(q, relativeTo);
		return ThisPtr();
    }

    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Rotate(const Quaternion& q, TransformSpace relativeTo)
    {
		// Normalise quaternion to avoid drift
		Quaternion qnorm = q;
		qnorm.Normalise();

        switch(relativeTo)
        {
        case TS_PARENT:
            // Rotations are normally relative to local axes, transform up
            orientation = qnorm * orientation;
            break;
        case TS_WORLD:
            // Rotations are normally relative to local axes, transform up
            orientation = orientation * _getDerivedOrientation().Inverse()
                * qnorm * _getDerivedOrientation();
            break;
        case TS_LOCAL:
            // Note the order of the mult, i.e. q comes after
            orientation = orientation * qnorm;
            break;
        }
        needUpdate();
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    const Quaternion & Transform::_getDerivedOrientation(void)
    {
		if (needParentUpdate)
		{
        	_updateFromParent();
		}
        return derivedOrientation;
    }
    //-----------------------------------------------------------------------
    const Vector3 & Transform::_getDerivedPosition(void)
    {
		if (needParentUpdate)
		{
        	_updateFromParent();
		}
        return derivedPosition;
    }
    //-----------------------------------------------------------------------
    const Vector3 & Transform::_getDerivedScale(void)
    {
        if (needParentUpdate)
        {
            _updateFromParent();
        }
        return derivedScale;
    }
	//-----------------------------------------------------------------------
    void Transform::needUpdate(bool forceParentUpdate)
    {

        needParentUpdate = true;
		needChildUpdate = true;
        cachedTransformOutOfDate = true;

        // Make sure we're not root and parent hasn't been notified before
        if (parent && (!parentNotified || forceParentUpdate))
        {
            parent->requestUpdate(ThisPtr(), forceParentUpdate);
			parentNotified = true ;
        }

        // all children will be updated
		childrenToUpdate->empty();
    }
    //-----------------------------------------------------------------------
    void Transform::requestUpdate(Transform::Ptr child, bool forceParentUpdate)
    {
        // If we're already going to update everything this doesn't matter
        if (needChildUpdate)
        {
            return;
        }

		childrenToUpdate->push_back(child);
        // Request selective update of me, if we didn't do it before
        if (parent && (!parentNotified || forceParentUpdate))
		{
            parent->requestUpdate(ThisPtr(), forceParentUpdate);
			parentNotified = true ;
		}

		// If there isn't a parent, and the needUpdate has hit this Transform
		// trigger a cascading update.
		if ( !parent )
		{
			_update(needChildUpdate, needParentUpdate);
		}
    }
    //-----------------------------------------------------------------------
    void Transform::cancelUpdate(Transform::Ptr child)
    {
		childrenToUpdate->remove(child);

        // Propogate this up if we're done
		if (childrenToUpdate->empty() && parent && !needChildUpdate)
        {
            parent->cancelUpdate(ThisPtr());
			parentNotified = false ;
        }
    }


}