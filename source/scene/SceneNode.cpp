//
//  SceneNode.cpp
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "scene/SceneNode.h"
#include "resource/ResourceManager.h"
#include "script/ScriptManager.h"

namespace epsilon
{
	SceneNode::Ptr SceneNode::Create()
	{
		return std::make_shared<SceneNode>(private_struct());
	}

	SceneNode::Ptr SceneNode::Create(std::string matName)
	{
		return std::make_shared<SceneNode>(private_struct(), matName);
	}

	SceneNode::SceneNode(const private_struct &) : Node(Node::private_struct()),
												   sceneOwner(0)
	{
	}

	SceneNode::SceneNode(const private_struct &, std::string name) : Node(Node::private_struct()),
																	 sceneOwner(0)
	{
	}

	SceneNode::~SceneNode(void)
	{
	}

	void SceneNode::Destroy()
	{
		// Destroy Components here?
		// for each component destroy or something....
	}

	// Create a child node.
	SceneNode::Ptr SceneNode::CreateChild(std::string name)
	{
		SceneNode::Ptr newChild = SceneNode::Create(name);
		AddChild(newChild);
		return newChild;
	}

	// The following are the internal component functions.
	Transform::Ptr SceneNode::CreateTransform()
	{
		Transform::Ptr newTrans = Transform::Create();
		AddComponent(newTrans);
		return newTrans;
	}

	Light::Ptr SceneNode::CreateLight(std::string name)
	{
		// Set default name if none specified
		if (name == "")
		{
			name = "Light_" + GetId();
		}

		Light::Ptr newLight = Light::Create(name);
		AddComponent(newLight);
		sceneOwner->AddLight(newLight);
		return newLight;
	}

	Camera::Ptr SceneNode::CreateCamera(std::string name)
	{
		// Set the default name if none specified.
		if (name == "")
		{
			name = "Camera_" + GetId();
		}

		Camera::Ptr newCamera = Camera::Create(name);
		AddComponent(newCamera);
		sceneOwner->AddCamera(newCamera);
		return newCamera;
	}

	Renderer::Ptr SceneNode::CreateRenderer()
	{
		Renderer::Ptr renderer = Renderer::Create();
		AddComponent(renderer);
		return renderer;
	}


	// The following functions are exposed to Python and will result in the active script being
	// set as the owner of the resulting object

	void SceneNode::HandleScriptOwner(NodeComponent::Ptr newComponent)
	{
		// Register Ownership with ResourceManager
		ScriptManager * sm = &ScriptManager::GetInstance();

		// If a script is currently running
		if (sm->IsActive())
		{
			long activeScriptId = sm->GetActiveResource();

			// If the active script isn't the owner of this SceneNode.
			if (activeScriptId != GetResourceOwner())
			{
				// the new component is owned by the new script
				newComponent->SetResourceOwner(activeScriptId);

				// If this node (the node the component is being attached to) is owned by a resource
				if (GetResourceOwner() != 0)
				{
					// Indicate to the Resource manager there is a relationship between the 
					// resource that owns this SceneNode and the owner of the new component
					ResourceManager::GetInstance().AddDependency(GetResourceOwner(), activeScriptId);
				}
			}
		}
	}

	// Create a child node.
	SceneNode::Ptr SceneNode::ScriptCreateChild(std::string name)
	{
		SceneNode::Ptr newChild = SceneNode::Create(name);
		AddChild(newChild);

		// Set the owner of the node to 
		HandleScriptOwner(newChild);
		
		return newChild;
	}

	Transform::Ptr SceneNode::ScriptCreateTransform()
	{
		Transform::Ptr newTrans = CreateTransform();

		HandleScriptOwner(newTrans);

		return newTrans;
	}

	Light::Ptr SceneNode::ScriptCreateLight(std::string name)
	{
		Light::Ptr newLight = CreateLight(name);

		HandleScriptOwner(newLight);

		return newLight;
	}

	Camera::Ptr SceneNode::ScriptCreateCamera(std::string name)
	{
		Camera::Ptr newCamera = CreateCamera(name);
		HandleScriptOwner(newCamera);
		return newCamera;
	}

	Renderer::Ptr SceneNode::ScriptCreateRenderer()
	{
		Renderer::Ptr renderer = CreateRenderer();
		HandleScriptOwner(renderer);
		return renderer;

	}
}


