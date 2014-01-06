#pragma once

#include "EpsilonCore.h"

#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "scene/NodeComponent.h"


namespace epsilon
{
	/* This class is based on the Ogre::Node object transform functionality
	   http://www.ogre3d.org for more info.
	 */
	class Transform;

	typedef std::list< shared_ptr<Transform> > TransformList;
	typedef shared_ptr<TransformList> TransformListPtr;

	class Transform : 
		public enable_shared_from_this<Transform>,
		public NodeComponent
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<Transform> Ptr;

		explicit Transform(const private_struct &);
		~Transform(void);
		
		Transform::Ptr ThisPtr() { return shared_from_this(); }

		static Transform::Ptr Create();
		void Destroy();
		
		// Children
		Transform::Ptr SetParentTransform(Transform::Ptr newParent);
		Transform::Ptr GetParentTransform();
		TransformListPtr GetChildren();
		Transform::Ptr AddChild(Transform::Ptr childNode);
		Transform::Ptr RemoveChild(Transform::Ptr childNode);
		Transform::Ptr RemoveAllChildren();
		bool HasChildren() { return !children->empty(); }

		Transform::Ptr FindChildWithName(string name);
		Transform::Ptr FindChildWithNameRecursive(string name);

		Transform::Ptr FindChildWithId(long id);
		Transform::Ptr FindChildWithIdRecursive(long id);


		/** Enumeration denoting the spaces which a transform can be relative to.
        */
        enum TransformSpace
        {
            /// Transform is relative to the local space
            TS_LOCAL,
            /// Transform is relative to the space of the parent node
            TS_PARENT,
            /// Transform is relative to world space
            TS_WORLD
        };

		// Add Node listeners functionality??

		// orientations doesn't necessarily inherit from parents.
		// determined by the value of inheritOrientation
		const Quaternion & GetOrientation() const;
		Transform::Ptr SetOrientation( const Quaternion& q);
		Transform::Ptr SetOrientation( float w, float x, float y, float z);
		Transform::Ptr ResetOrientation();
		Transform::Ptr SetInheritOrientation(bool inherit);
		bool GetInheritOrientation(void) const;

		// relative to parent
		const Vector3 & GetPosition() const;
		Transform::Ptr SetPosition( const Vector3& pos);
		Transform::Ptr SetPosition( float x, float y, float z);

		// scale also doesn't necessarily inherit from parents
		// determined by the value of inheritScale
		const Vector3 & GetScale(void) const;
		Transform::Ptr SetScale( const Vector3& sscale);
		Transform::Ptr SetScale( float x, float y, float z);
		Transform::Ptr SetInheritScale(bool inherit);
		bool GetInheritScale(void) const;

		Transform::Ptr Scaled( const Vector3& scale);
		Transform::Ptr Scaled( float x, float y, float z);

		Transform::Ptr Translate( const Vector3& d, TransformSpace relativeTo = TS_PARENT);
		Transform::Ptr Translate( float x, float y, float z, TransformSpace relativeTo = TS_PARENT);
		Transform::Ptr Translate( const Matrix3& axes, const Vector3& move, TransformSpace relativeTo = TS_PARENT);
		Transform::Ptr Translate( const Matrix3& axes, float x, float y, float z, TransformSpace relativeTo = TS_PARENT);

		Transform::Ptr Roll( const float& angle, TransformSpace relativeTo = TS_PARENT);
		Transform::Ptr Pitch( const float& angle, TransformSpace relativeTo = TS_PARENT);
		Transform::Ptr Yaw( const float& angle, TransformSpace relativeTo = TS_PARENT);

		Transform::Ptr Rotate(const Vector3& axis, const float& angle, TransformSpace relativeTo = TS_LOCAL);
		Transform::Ptr Rotate(const Quaternion& q, TransformSpace relativeTo = TS_LOCAL);

		Matrix3 GetLocalAxes(void) const;

      /** Gets the orientation of the node as derived from all parents.
        */
        virtual const Quaternion & _getDerivedOrientation(void);

        /** Gets the position of the node as derived from all parents.
        */
        virtual const Vector3 & _getDerivedPosition(void);

        /** Gets the scaling factor of the node as derived from all parents.
        */
        virtual const Vector3 & _getDerivedScale(void);

		/** Gets the full transformation matrix for this node.
            @remarks
                This method returns the full transformation matrix
                for this node, including the effect of any parent node
                transformations, provided they have been updated using the Node::_update method.
                This should only be called by a SceneManager which knows the
                derived transforms have been updated before calling this method.
                Applications using Ogre should just use the relative transforms.
        */
        virtual const Matrix4& _getFullTransform(void);

		/** Internal method to update the Node.
            @note
                Updates this node and any relevant children to incorporate transforms etc.
                Don't call this yourself unless you are writing a SceneManager implementation.
            @param
                updateChildren If true, the update cascades down to all children. Specify false if you wish to
                update children separately, e.g. because of a more selective SceneManager implementation.
            @param
                parentHasChanged This flag indicates that the parent xform has changed,
                    so the child should retrieve the parent's xform and combine it with its own
                    even if it hasn't changed itself.
        */
        virtual void _update(bool updateChildren, bool parentHasChanged);

		/** To be called in the event of transform changes to this node that require it's recalculation.
        @remarks
            This not only tags the node state as being 'dirty', it also requests it's parent to 
            know about it's dirtiness so it will get an update next time.
		@param forceParentUpdate Even if the node thinks it has already told it's
			parent, tell it anyway
        */
        virtual void needUpdate(bool forceParentUpdate = false);

        /** Called by children to notify their parent that they need an update. 
		@param forceParentUpdate Even if the node thinks it has already told it's
			parent, tell it anyway
		*/
        virtual void requestUpdate(Transform::Ptr child, bool forceParentUpdate = false);
        /** Called by children to notify their parent that they no longer need an update. */
        virtual void cancelUpdate(Transform::Ptr child);

	protected:

		void _updateFromParent(void);

		/** Class-specific implementation of _updateFromParent.
		@remarks
			Splitting the implementation of the update away from the update call
			itself allows the detail to be overridden without disrupting the 
			general sequence of updateFromParent (e.g. raising events)
		*/
		virtual void updateFromParentImpl(void);

		
	private:
		TransformListPtr children;
		Transform::Ptr parent;
		
		bool inheritOrientation;
		bool inheritScale;

		Vector3 position;
		Quaternion orientation;
		Vector3 scale;

		Vector3 derivedPosition;
		Quaternion derivedOrientation;
		Vector3 derivedScale;

		Matrix4 cachedTransform;
		bool cachedTransformOutOfDate;
		
		/// Flag to indicate own transform from parent is out of date
        mutable bool needParentUpdate;
		/// Flag indicating that all children need to be updated
		mutable bool needChildUpdate;
		/// Flag indicating that parent has been notified about update request
	    mutable bool parentNotified ;
        
		TransformListPtr childrenToUpdate;
	};

}