#pragma once
//
//  SceneNode.h
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//

#include <list>
#include <memory>
#include <string>

// Bases
#include "scene/SceneBase.h"
#include "scene/Node.h"

// Components
#include "render/Camera.h"
#include "render/Light.h"
#include "render/Renderer.h"
#include "script/ScriptBehaviour.h"
#include "physics/RigidBody.h"

#include "audio/AudioSource.h"
#include "audio/AudioListener.h"

namespace epsilon
{
	class SceneNode;

    typedef std::list< std::shared_ptr<SceneNode> > SceneNodeList;
    typedef std::shared_ptr<SceneNodeList> SceneNodeListPtr;
    
    class SceneNode : public Node
    {
		friend class Scene;
        protected:
            struct private_struct {};
            
        public:
            // Instantisation
            typedef std::shared_ptr<SceneNode> Ptr;
            
            static SceneNode::Ptr Create();
            static SceneNode::Ptr Create(std::string name);
            
            explicit SceneNode(const private_struct &);
            SceneNode(const private_struct &, std::string name);
            ~SceneNode(void);
        
            // Get the Scene that this Node is a part of
			SceneBase::Ptr GetScene() { return sceneOwner; }
        
            // Node component accessors;
            Transform::Ptr			GetTransform() { return transform; }
            Light::Ptr				GetLight() { return light; }
            Camera::Ptr				GetCamera() { return camera; }
            Renderer::Ptr			GetRenderer() { return renderer; }
			ScriptBehaviourList		GetScripts() { return scripts; }
			RigidBody::Ptr			GetRigidBody() { return rigidBody; }
            AudioSources            GetAudioSources() { return audioSources; }
        
            AudioSource::Ptr        GetAudioSource(std::string path);
            AudioListener::Ptr      GetAudioListener() { return audioListener; }
        

            // Create a child node.
            SceneNode::Ptr			CreateChild(std::string name = "");
			void					AddChild(SceneNode::Ptr newChild);
        
            // Remove a child node
            void                    RemoveChild(std::string name);
            void                    RemoveChild(SceneNode::Ptr child);
        
            void                    RemoveAllChildren();
        
            // Helper functions for creating components on the SceneNode!
        
            // The following function are only used internally by the C++
            Transform::Ptr			CreateTransform();
            Light::Ptr				CreateLight(std::string name = "");
            Camera::Ptr				CreateCamera(std::string name = "");
            Renderer::Ptr			CreateRenderer();
			ScriptBehaviour::Ptr	CreateBehaviour(std::string filename);
			RigidBody::Ptr			CreateRigidBody(float mass = 0.f, Vector3 inertia = Vector3::ZERO, bool kinematic = false);
        
            AudioSource::Ptr        CreateAudioSource(std::string path = "");
            AudioListener::Ptr      SetAudioListener();

			// The following functions are exposed to Python and will result in the active script being
            // set as the owner of the resulting object
			
			// Create a child node.
			SceneNode::Ptr			ScriptCreateChild(std::string name = "");
			void					ScriptAddChild(SceneNode::Ptr newChild);

            Transform::Ptr			ScriptCreateTransform();
            Light::Ptr				ScriptCreateLight(std::string name = "");
            Camera::Ptr				ScriptCreateCamera(std::string name = "");
            Renderer::Ptr			ScriptCreateRenderer();
			ScriptBehaviour::Ptr	ScriptCreateBehaviour(std::string filename);
			RigidBody::Ptr			ScriptCreateRigidBody(float mass = 0.f, Vector3 inertia = Vector3::ZERO, bool kinematic = false);
        
            AudioSource::Ptr        ScriptCreateAudioSource(std::string path = "");
            AudioListener::Ptr      ScriptSetAudioListener();
			bool					ScriptIsAudioListener();
			
			// Find an attached script by class name
			ScriptBehaviour::Ptr	GetScriptByClassname(std::string classname);
			ScriptBehaviourList		GetScriptsByClassname(std::string classname);
        
            // Find an attached AudioSource by name
            AudioSource::Ptr        GetAudioSourceByName(std::string name); 

        protected:
            void SetScene(SceneBase::Ptr newScene) { sceneOwner = newScene; }
			void HandleScriptOwner(NodeComponent::Ptr newComponent);

        private:
			SceneBase::Ptr  sceneOwner;
        
            Transform::Ptr		transform;
            Light::Ptr			light;
            Camera::Ptr			camera;
            Renderer::Ptr		renderer;
			ScriptBehaviourList scripts;
			RigidBody::Ptr		rigidBody;
        
            AudioListener::Ptr  audioListener;
        
            AudioSources        audioSources;
    };
}