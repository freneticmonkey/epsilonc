#pragma once
//
//  SceneNode.h
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include <list>
#include <memory>
#include <string>

// Bases
#include "scene/Scene.h"
#include "scene/Node.h"

// Components
#include "render/Camera.h"
#include "render/Light.h"
#include "render/Renderer.h"


namespace epsilon
{
    class Scene;
    class SceneNode;
    
    typedef std::list< std::shared_ptr<SceneNode> > SceneNodeList;
    typedef std::shared_ptr<SceneNodeList> SceneNodeListPtr;
    
    class SceneNode : public Node
    {
		friend Scene;
        protected:
            struct private_struct {};
            
        public:
            // Instantisation
            typedef std::shared_ptr<SceneNode> Ptr;
            
            static SceneNode::Ptr Create();
            static SceneNode::Ptr Create(std::string name);
            
            void Destroy();
            
            explicit SceneNode(const private_struct &);
            SceneNode(const private_struct &, std::string name);
            ~SceneNode(void);
        
            // Get the Scene that this Node is a part of
            Scene::Ptr GetScene() { return sceneOwner; }
        
            // Create a child node.
            SceneNode::Ptr CreateChild(std::string name = "");
        
            // Helper functions for creating components on the SceneNode!
        
            // The following function are only used internally by the C++
            Transform::Ptr  CreateTransform();
            Light::Ptr      CreateLight(std::string name = "");
            Camera::Ptr     CreateCamera(std::string name = "");
            Renderer::Ptr   CreateRenderer();
        
            // The following functions are exposed to Python and will result in the active script being
            // set as the owner of the resulting object
			
			// Create a child node.
			SceneNode::Ptr ScriptCreateChild(std::string name = "");

            Transform::Ptr  ScriptCreateTransform();
            Light::Ptr      ScriptCreateLight(std::string name = "");
            Camera::Ptr     ScriptCreateCamera(std::string name = "");
            Renderer::Ptr   ScriptCreateRenderer();
        
        protected:
        
            void SetScene(Scene::Ptr newScene) { sceneOwner = newScene; }
			void HandleScriptOwner(NodeComponent::Ptr newComponent);

        private:
            Scene::Ptr  sceneOwner;
    };
}