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
		
	}

	void SceneManager::Cull(void)
	{
		// Just get the root for now
		renderList.clear();


		Node::Ptr root = currentScene->Root();
		// Do not add root node which doesn't have a renderer to the renderList ~!!!!!!
		//renderList.push_back(root);

		// Add Root's children
		CullNodeChildren(root);
		
	}

	void SceneManager::CullNodeChildren(Node::Ptr node)
	{
		Transform::Ptr nodeTrans = node->GetComponent<Transform>();
		if ( nodeTrans->HasChildren() )
		{
			TransformListPtr children = nodeTrans->GetChildren();
			for ( TransformList::iterator transform = children->begin(); transform != children->end(); transform++)
			{
				Node::Ptr childNode = std::dynamic_pointer_cast<epsilon::Node>( (*transform)->GetParent() );
				if ( childNode && childNode->GetComponent<Renderer>() )
				{
					renderList.push_back( childNode->GetComponent<Renderer>() );
				}

				if ( (*transform)->HasChildren() )
				{
					CullNodeChildren( childNode );
				}
			}
		}

	}

	void SceneManager::Draw(RenderStateStack::Ptr stateStack)
	{
		Vector3 p;
		Quaternion o;
		Vector3 axis;
		//float angle;
		Vector3 s;

		// Push a new render state
		stateStack->Push();
		
		// Update the matrices
		stateStack->State()->view = currentScene->GetActiveCamera()->GetViewMatrix();
		stateStack->State()->projection = currentScene->GetActiveCamera()->GetProjectionMatrix();
		
		for ( RenderList::iterator renderer = renderList.begin(); renderer != renderList.end(); renderer++)
		{
			// Render
			(*renderer)->Draw(stateStack);//viewMatrix, projMatrix);
		}

		stateStack->Pop();
	}

}