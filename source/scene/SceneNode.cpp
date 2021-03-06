//
//  SceneNode.cpp
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//

#include "scene/SceneNode.h"
#include "resource/ResourceManager.h"
#include "script/ScriptManager.h"
#include "render/RenderManager.h"
#include "physics/PhysicsManager.h"
#include "audio/AudioManager.h"

#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{
	SceneNode::Ptr SceneNode::Create()
	{
        SceneNode::Ptr newNode = std::make_shared<SceneNode>(private_struct());
        newNode->CreateTransform();
        return newNode;
	}

	SceneNode::Ptr SceneNode::Create(std::string name)
	{
        SceneNode::Ptr newNode = std::make_shared<SceneNode>(private_struct(), name);
        newNode->CreateTransform();
        return newNode;
	}

	SceneNode::SceneNode(const private_struct &) : Node(Node::private_struct()),
												   sceneOwner(0)
	{
	}

	SceneNode::SceneNode(const private_struct &, std::string name) : Node(Node::private_struct(), name),
																	 sceneOwner(0)
	{
	}

	SceneNode::~SceneNode(void)
	{
	}

	void SceneNode::OnDestroy()
	{
		// Remove the components when the node is destroyed and notify their managers
		if (light)
		{
			sceneOwner->RemoveLight(light);
			RemoveComponent(light);
			(&RenderManager::GetInstance())->DestroyLight(light);
		}

		if (camera)
		{
			sceneOwner->RemoveCamera(camera);
			RemoveComponent(camera);
		}

		// renderer
		if (renderer)
		{
			RemoveComponent(renderer);
			(&RenderManager::GetInstance())->DestroyRenderer(renderer);
		}

		// script
		if (scripts.size() > 0)
		{
			// Destroy each script via the ScriptManager
			for (Script::Ptr script : scripts)
			{
				RemoveComponent(script);
				(&ScriptManager::GetInstance())->DestroyScript(script);
			};

			// Then remove all scripts attached to this node
			scripts.clear();
		}

		// rigidbody
		if (rigidBody)
		{
			RemoveComponent(rigidBody);
			PhysicsManager::GetInstance().DestroyRigidBody(rigidBody);
		}

		// audiosource
		if (audioSources.size() > 0 )
		{
			for (AudioSource::Ptr audioSource : audioSources)
			{
				RemoveComponent(audioSource);
				AudioManager::GetInstance().DestroyAudioSource(audioSource);
			}

			// Then remove all audiosources attached to this node
			audioSources.clear();
		}

		// Finally propagate event to children
		RemoveComponent(transform);
		transform->OnDestroy();
	}
    
	// Add / Move a child node from one parent to another
	void SceneNode::AddChild(SceneNode::Ptr newChild)
	{
		GetTransform()->AddChild( newChild->GetComponent<Transform>() );
		newChild->SetScene(sceneOwner);
	}
    
	// Create a child node.
	SceneNode::Ptr SceneNode::CreateChild(std::string name)
	{
		SceneNode::Ptr newChild = SceneNode::Create(name);
		AddChild(newChild);
		return newChild;
	}
    
    void SceneNode::RemoveChild(std::string name)
    {
        Transform::Ptr childTransform = transform->FindChildWithName(name);
        if ( childTransform )
        {
            transform->RemoveChild(childTransform);
        }
    }
    
    void SceneNode::RemoveChild(SceneNode::Ptr child)
    {
        transform->RemoveChild(child->transform);
    }
    
    void SceneNode::RemoveAllChildren()
    {
        transform->RemoveAllChildren();
    }

	// The following are the internal component functions.
	Transform::Ptr SceneNode::CreateTransform()
	{
		transform = Transform::Create();
		AddComponent(transform);
        return transform;
	}

	Light::Ptr SceneNode::CreateLight(std::string name)
	{
		// Set default name if none specified
		if (name == "")
		{
            name = boost::str(format("Light_%d") % GetId() );
		}

		RenderManager * rm = &RenderManager::GetInstance();
		light = rm->CreateLight(name);

		AddComponent(light);
		sceneOwner->AddLight(light);
		return light;
	}

	Camera::Ptr SceneNode::CreateCamera(std::string name)
	{
		// Set the default name if none specified.
		if (name == "")
		{
            name = boost::str(format("Camera_%d") % GetId() );
		}

		RenderManager * rm = &RenderManager::GetInstance();
		camera = rm->CreateCamera(name);
		
		if (camera)
		{
			AddComponent(camera);
			sceneOwner->AddCamera(camera);
		}
		return camera;
	}

	Renderer::Ptr SceneNode::CreateRenderer()
	{
        RenderManager * rm = &RenderManager::GetInstance();
		renderer = rm->CreateRenderer();
		if (renderer)
		{
			AddComponent(renderer);
		}
		return renderer;
	}

	ScriptBehaviour::Ptr SceneNode::CreateBehaviour(std::string filename)
	{
		ScriptBehaviour::Ptr newScript = ScriptBehaviour::CreateFromFile(filename);

		if (newScript)
		{
			// Register the new script with the ResourceManager
			ScriptManager * sm = &ScriptManager::GetInstance();
			sm->AddBehaviour(newScript);
			AddComponent(newScript);
			scripts.push_back(newScript);
		}
		return newScript;
	}

	RigidBody::Ptr	SceneNode::CreateRigidBody(float mass, Vector3 inertia, bool kinematic)
	{
		rigidBody = PhysicsManager::GetInstance().CreateRigidBody(mass, inertia, kinematic);
		if (rigidBody)
		{
			AddComponent(rigidBody);
		}
		return rigidBody;
	}
    
    AudioSource::Ptr SceneNode::CreateAudioSource(std::string path)
    {
        AudioSource::Ptr audioSource = AudioManager::GetInstance().CreateAudioSourceByPath(path);
		if (audioSource)
		{
			AddComponent(audioSource);
			audioSources.push_back(audioSource);
		}
        return audioSource;
    }
    
    AudioListener::Ptr SceneNode::SetAudioListener()
    {
        audioListener = AudioManager::GetInstance().GetListener();
		if (audioListener)
		{
			AddComponent(audioListener);
		}
        return audioListener;
    }

	// The following functions are exposed to Python and will result in the active script being
	// set as the owner of the resulting object (this assumes that python is single-threaded and
	// so there is only ever going to be a single script active at a time).

	void SceneNode::HandleScriptOwner(NodeComponent::Ptr newComponent)
	{
		// If the new component isn't null.
		if (newComponent)
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
	}

	// Add / Move a child node from one parent to another
	void SceneNode::ScriptAddChild(SceneNode::Ptr newChild)
	{
		// Ensure that the node's transform is notified of the change
		AddChild(newChild);

		// Set the owner of the node to 
		HandleScriptOwner(newChild);
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

	ScriptBehaviour::Ptr SceneNode::ScriptCreateBehaviour(std::string filename)
	{
		ScriptBehaviour::Ptr newScript = CreateBehaviour(filename);
		HandleScriptOwner(newScript);
		return newScript;
	}

	RigidBody::Ptr SceneNode::ScriptCreateRigidBody(float mass, Vector3 inertia, bool kinematic)
	{
		RigidBody::Ptr newRB = CreateRigidBody(mass, inertia, kinematic);
		HandleScriptOwner(newRB);
		return newRB;
	}

	// Script Accessor functions
	ScriptBehaviour::Ptr SceneNode::GetScriptByClassname(std::string classname)
	{
		ScriptBehaviour::Ptr theScript;

		ScriptBehaviourList::iterator foundScript;
		foundScript = std::find_if(scripts.begin(), scripts.end(), [&](ScriptBehaviour::Ptr script){
			return classname == script->GetClassname();
		});

		if (foundScript != scripts.end())
		{
			theScript = *foundScript;
		}

		return theScript;
	}

	ScriptBehaviourList SceneNode::GetScriptsByClassname(std::string classname)
	{
		ScriptBehaviourList theScripts;

		std::for_each(scripts.begin(), scripts.end(), [&](ScriptBehaviour::Ptr script){
			if (classname == script->GetClassname())
			{
				theScripts.push_back(script);
			}
		});

		return theScripts;
	}
    
    AudioSource::Ptr  SceneNode::ScriptCreateAudioSource(std::string path)
    {
		AudioSource::Ptr audio = CreateAudioSource(path);
		HandleScriptOwner(audio);
		return audio;
    }
    
    AudioListener::Ptr SceneNode::ScriptSetAudioListener()
    {
        return SetAudioListener();
    }

	bool SceneNode::ScriptIsAudioListener()
	{ 
		if (audioListener != nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}		
	}
    
    AudioSource::Ptr SceneNode::GetAudioSourceByName(std::string name)
    {
        AudioSource::Ptr audioSource;
        
        AudioSources::iterator foundAS;
        foundAS = std::find_if(audioSources.begin(), audioSources.end(), [&](AudioSource::Ptr source){
            return source->GetName() == name;
        });
        
        if ( foundAS != audioSources.end() )
        {
            audioSource = *foundAS;
        }
        
        return audioSource;
    }
}


