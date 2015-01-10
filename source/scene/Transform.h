#pragma once

#include "EpsilonCore.h"

#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Bounds.h"

#include "scene/NodeComponent.h"


namespace epsilon
{
	/* This class is based on the Ogre::Node object transform functionality
	   http://www.ogre3d.org for more info.
	 */
	class Transform;

	typedef std::list< std::shared_ptr<Transform> > TransformList;
	typedef std::shared_ptr<TransformList> TransformListPtr;

	class Transform : 
		public std::enable_shared_from_this<Transform>,
		public NodeComponent
	{
	private:
		struct private_struct {};

	public:
		// Debugging
		typedef std::shared_ptr<Transform> Ptr;

		explicit Transform(const private_struct &);
		~Transform(void);
		
		Transform::Ptr ThisPtr() { return shared_from_this(); }

		static Transform::Ptr Create();
		void OnDestroy();
		void RemoveAndDestroyAllChildren();

        // Handle Transmitting enabled state through the scene graph.
        void OnEnable();
        void OnDisable();
		
		// Children
		Transform::Ptr SetParentTransform(Transform::Ptr newParent);
		Transform::Ptr GetParentTransform();
		TransformList  GetChildren();
		Transform::Ptr AddChild(Transform::Ptr childNode);
		Transform::Ptr RemoveChild(Transform::Ptr childNode);
		Transform::Ptr RemoveAllChildren();

		bool HasChildren() { return !children.empty(); }

		Transform::Ptr FindChildWithName(std::string name);
		Transform::Ptr FindChildWithNameRecursive(std::string name);

		Transform::Ptr FindChildWithId(long id);
		Transform::Ptr FindChildWithIdRecursive(long id);

		// Transform to Vector Helpers
		const Vector3 & Forward();

		const Vector3 & Up();

		const Vector3 & Right();

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
			
		const Quaternion & GetLocalOrientation() const;
		const Quaternion & GetOrientation() const;

		Transform::Ptr SetLocalOrientation(const Quaternion& q);
		Transform::Ptr SetLocalOrientation(float w, float x, float y, float z);

		Transform::Ptr SetOrientation( const Quaternion& q);
		Transform::Ptr SetOrientation( float w, float x, float y, float z);

		Transform::Ptr ResetOrientation();
		Transform::Ptr SetInheritOrientation(bool inherit);
		bool GetInheritOrientation(void) const;

		// relative to parent
		const Vector3 & GetLocalPosition() const;
		const Vector3 & GetPosition() const;

		Transform::Ptr SetLocalPosition(const Vector3& pos);
		Transform::Ptr SetLocalPosition(float x, float y, float z);

		Transform::Ptr SetPosition( const Vector3& pos);
		Transform::Ptr SetPosition( float x, float y, float z);

		// scale also doesn't necessarily inherit from parents
		// determined by the value of inheritScale
		const Vector3 & GetLocalScale(void) const;
		const Vector3 & GetScale(void) const;

		Transform::Ptr SetLocalScale(const Vector3& sscale);
		Transform::Ptr SetLocalScale(float x, float y, float z);

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

		void LookAt(Vector3 target);
		void LookAt(Vector3 from, Vector3 to);
		void LookAt(float x, float y, float z,
					float lookAtX, float lookAtY, float lookAtZ);

		void FPS(Vector3 pos, float pitch, float yaw);

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

		/**  Set the Bounds contained by a mesh attached to the Transform's SceneNode
		*/
		void SetMeshBounds(const Bounds& imeshBounds) { meshBounds = imeshBounds; }

		/**  Get the Bounds contained by the Transform and all of it's children.
		*/
		const Bounds& GetOuterBounds() { return childrenBounds; }

		/**  Get the Bounds contained only by the Mesh of SceneNode the transform is attached to.
		*/
		const Bounds& GetMeshBounds() { return myBounds; }

	protected:

		void _updateFromParent(void);

		/** Class-specific implementation of _updateFromParent.
		@remarks
			Splitting the implementation of the update away from the update call
			itself allows the detail to be overridden without disrupting the 
			general sequence of updateFromParent (e.g. raising events)
		*/
		virtual void updateFromParentImpl(void);


		void UpdateBounds(void);

	private:
		TransformList children;
		TransformList childrenToUpdate;
        
		Transform::Ptr parent;
		
		bool inheritOrientation;
		bool inheritScale;

		Vector3 position;
		Quaternion orientation;
		Vector3 scale;

		Vector3 forward;
		Vector3 up;
		Vector3 right;

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
        

		// Bounds
		Bounds childrenBounds;
		Bounds myBounds;
		Bounds meshBounds;
	};

}