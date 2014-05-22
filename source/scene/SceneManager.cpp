#include "scene/SceneManager.h"

namespace epsilon
{
	SceneManager::SceneManager(void)
	{

	}

	SceneManager::~SceneManager(void)
	{
		scenes.clear();
	}

	void SceneManager::Setup(void)
	{
		Log("Initialising Scene Manager");
		
		// Add a default scene
		currentScene = Scene::Create("default");
		scenes.push_back(currentScene);
	}

	void SceneManager::SetScene(Scene::Ptr newScene)
	{
		bool found = false;
		for (SceneList::iterator s = scenes.begin(); s != scenes.end(); s++ )
		{
			if (newScene == (*s))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			scenes.push_back(newScene);	
		}
		currentScene = newScene;
	}

	void SceneManager::SetScene(std::string name)
	{
		bool found = false;
		for (SceneList::iterator s = scenes.begin(); s != scenes.end(); s++ )
		{
			if ((*s)->GetName() == name)
			{
				currentScene = (*s);
				break;
			}
		}
		if (!found)
		{
			std::string err("ERROR: SceneManager::setScene() invalid scene name: ");
			err += name;
			Log(err.c_str());
		}
	}

	void SceneManager::Update(float el)
	{
		if (currentScene)
		{
			currentScene->Update(el);
		}
	}

	void SceneManager::Cull(void)
	{
		// Just get the root for now
		renderItems.clear();

		// Add Root's children
		CullNodeChildren(currentScene->Root());
	}

	void SceneManager::CullNodeChildren(SceneNode::Ptr node)
	{
        // If the node is disabled it's excluded from rendering
        if (node->IsEnabled())
        {
            Transform::Ptr nodeTrans = node->GetTransform();
            if ( nodeTrans->HasChildren() )
            {
                TransformList children = nodeTrans->GetChildren();

				std::for_each(children.begin(), children.end(), [&](Transform::Ptr transform){
					
					SceneNode::Ptr childNode = std::dynamic_pointer_cast<epsilon::SceneNode>(transform->GetParent());

					if (childNode->GetRenderer())
					{
						renderItems.push_back(childNode->GetRenderer());
					}

					if (transform->HasChildren())
					{
						CullNodeChildren(childNode);
					}
				});
            }
        }
	}
}