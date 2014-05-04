#include "scene/Transform.h"
#include "logging/Logging.h"

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
    
    void Transform::OnEnable()
    {
        // Enable each of the children of this transform
        std::for_each(children->begin(), children->end(), [](Transform::Ptr child){
            child->componentParent->Enable();
        });
    }
    
    void Transform::OnDisable()
    {
        // Disable each of the children of this transform
        std::for_each(children->begin(), children->end(), [](Transform::Ptr child){
            child->componentParent->Disable();
        });
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
		needUpdate(true);
		return ThisPtr();
	}
	//-----------------------------------------------------------------------
	Transform::Ptr Transform::SetLocalOrientation(float w, float x, float y, float z)
	{
		orientation.w = w;
		orientation.x = x;
		orientation.y = y;
		orientation.z = z;
		needUpdate(true);
		return ThisPtr();
	}

    //-----------------------------------------------------------------------
    Transform::Ptr Transform::SetOrientation( const Quaternion & q )
    {
		if (parent)
		{
			orientation = (parent->_getDerivedOrientation() * q) *q.Inverse();
		}
		else
		{
			orientation = q;
		}
		orientation.Normalise();
        needUpdate(true);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::SetOrientation( float w, float x, float y, float z)
    {
		SetOrientation(Quaternion(x, y, z, w));
		needUpdate(true);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::ResetOrientation(void)
    {
        orientation = Quaternion::IDENTITY;
		needUpdate(true);
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
		needUpdate(true);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
	Transform::Ptr Transform::SetLocalScale(float x, float y, float z)
    {
        scale.x = x;
        scale.y = y;
        scale.z = z;
		needUpdate(true);
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
		needUpdate(true);
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
		needUpdate(true);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Scaled(float x, float y, float z)
    {
        scale[0] *= x;
        scale[1] *= y;
        scale[0] *= z;
		needUpdate(true);
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
        needUpdate(true);
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
        Rotate(Forward(), angle, relativeTo);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Pitch(const float& angle, TransformSpace relativeTo)
    {
        Rotate(Right(), angle, relativeTo);
		return ThisPtr();
    }
    //-----------------------------------------------------------------------
    Transform::Ptr Transform::Yaw(const float& angle, TransformSpace relativeTo)
    {
        Rotate(Up(), angle, relativeTo);
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
        needUpdate(true);
		return ThisPtr();
    }

	void Transform::LookAt(Vector3 target)
	{
		LookAt(position, target);
	}

	void Transform::LookAt(Vector3 from, Vector3 to)
	{
		LookAt(from.x, from.y, from.z, to.x, to.y, to.z);
	}

	void Transform::LookAt(float x, float y, float z,
		float lookAtX, float lookAtY, float lookAtZ)
	{
		Vector3 up(0.f, 1.f, 0.f);
		Vector3 from(x, y, z);
		Vector3 to(lookAtX, lookAtY, lookAtZ);

		// create the look at matrix.
		cachedTransform = Matrix4::CreateLookAt(from, to, up);

		// Set the orientation in the transform from the look at matrix
		SetLocalOrientation(cachedTransform.GetRotation());

		SetPosition(from);

		needUpdate(true);
	}

	void Transform::FPS(Vector3 pos, float pitch, float yaw)
	{
		/*
		float cosPitch = cos(pitch);
		float sinPitch = sin(pitch);
		float cosYaw = cos(yaw);
		float sinYaw = sin(yaw);

		Vector3 xaxis(cosYaw, 0, -sinYaw);
		Vector3 yaxis(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
		Vector3 zaxis(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

		cachedTransform = Matrix4(
			xaxis.x, xaxis.y, xaxis.z, -xaxis.Dot(pos),
			yaxis.x, yaxis.y, yaxis.z, -yaxis.Dot(pos),
			zaxis.x, zaxis.y, zaxis.z, -zaxis.Dot(pos),
			0,		 0,		  0,		1);

		SetPosition(cachedTransform.GetTranslation());
		SetLocalOrientation(cachedTransform.GetRotation());
		*/

		Quaternion currentRot = derivedOrientation;

		Vector3 xAxis, yAxis, zAxis;
		currentRot.ToAxes(xAxis, yAxis, zAxis);

		Quaternion yawQ = Quaternion(yAxis, yaw);
		Quaternion pitchQ = Quaternion(xAxis, pitch);
		Quaternion finalRot = pitchQ * yawQ;

		finalRot = finalRot * currentRot;
		finalRot.Normalise();

		SetPosition(pos);
		SetLocalOrientation(finalRot);

		//derivedOrientation = finalRot;

		needUpdate(true);
	}

	const Matrix4& Transform::_getFullTransform(void)
	{
		// Make a transform from the transform components
		if (cachedTransformOutOfDate)
		{
			Matrix4 rot, scaleM, pos, res;

			//cachedTransform = Matrix4::CreateTranslation(derivedPosition.x, derivedPosition.y, derivedPosition.z);
			//cachedTransform *= orientation.GetMatrix();
			//cachedTransform *= Matrix4::CreateScale(derivedScale.x, derivedScale.y, derivedScale.z);

			
			// Rotation & Scale
			//rot = Quaternion::IDENTITY.GetMatrix();// derivedOrientation.GetMatrix();
			rot = orientation.GetMatrix();
			scaleM = Matrix4::CreateScale(derivedScale.x, derivedScale.y, derivedScale.z);
			res = rot * scaleM;

			for (int i = 0; i < 3; i++)
			{
				//cachedTransform[Vector4(res3[i][0],res3[i][1],res3[i][2],0));
				cachedTransform[4 * i]	   = res[4 * i];
				cachedTransform[4 * i + 1] = res[4 * i + 1];
				cachedTransform[4 * i + 2] = res[4 * i + 2];
				cachedTransform[4 * i + 3] = 0;

			}

			// Translation
			cachedTransform[3] = derivedPosition.x;
			cachedTransform[7] = derivedPosition.y;
			cachedTransform[11] = derivedPosition.z;
			

			cachedTransformOutOfDate = false;
		}
		return cachedTransform;
	}

	// Transform to Vector Helpers
	const Vector3 & Transform::Forward() 
	{
		if (cachedTransformOutOfDate)
		{
			_updateFromParent();
		}
		return forward;
	};

	const Vector3 & Transform::Up()
	{
		if (cachedTransformOutOfDate)
		{
			_updateFromParent();
		}
		return up;
	};

	const Vector3 & Transform::Right()
	{
		if (cachedTransformOutOfDate)
		{
			_updateFromParent();
		}
		return right;
	};

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
		//Log("needUpdate: " + ThisPtr()->componentParent->GetName());
        // Make sure we're not root and parent hasn't been notified before
        if (parent && (!parentNotified || forceParentUpdate))
        {
            parent->requestUpdate(ThisPtr(), forceParentUpdate);
			parentNotified = true ;
        }

        // all children will be updated
		childrenToUpdate->clear();
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
			//Log("Request Update: " + ThisPtr()->componentParent->GetName());
            parent->requestUpdate(ThisPtr(), forceParentUpdate);
			parentNotified = true ;
		}

		// If there isn't a parent, and the needUpdate has hit this Transform
		// trigger a cascading update.
		if ( !parent )
		{
			//Log("Hit Root: " + ThisPtr()->componentParent->GetName());
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

	void Transform::_update(bool updateChildren, bool parentHasChanged)
	{
        if (IsEnabled()) {
        
            // always clear information about parent notification
            parentNotified = false;
            
            // See if we should process everyone
            if (needParentUpdate || parentHasChanged)
            {
                // Update transforms from parent
                _updateFromParent();
            }
            /*
             if (childrenToUpdate->size() > 0)
             {
             for_each(childrenToUpdate->begin(), childrenToUpdate->end(), [](Transform::Ptr child){
             child->_update(true, false);
             });
             childrenToUpdate->clear();
             }
             */
            
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
				for_each(childrenToUpdate->begin(), childrenToUpdate->end(), [](Transform::Ptr child){
					child->_update(true, false);
				});
			}
			childrenToUpdate->clear();
			needChildUpdate = false;
            //}
            
        }

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

}