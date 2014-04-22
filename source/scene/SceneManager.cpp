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
        
        Log("Configuring the Uniform Buffer Objects");
		ShaderManager * shaderManager = &ShaderManager::GetInstance();
        
        globalMatrices = shaderManager->GetUniformBuffer("GlobalMatrices");
		viewMatrixUnf = globalMatrices->GetUniform("viewMatrix");
		projMatrixUnf = globalMatrices->GetUniform("projectionMatrix");
        
        // Process lights
		lights = shaderManager->GetUniformBuffer("Lights");
        
        // Get the control for the number of active lights
        int index = 0;
		numLights = lights->GetUniform("numLights");
        
        for (int i = 0; i < Light::MAX_LIGHTS; i++ )
        {
            // Get the Uniforms for the lights
            LightUniforms lightData;
			lightData.position		= lights->GetUniform("lights[" + std::to_string(i) + "].position");
			lightData.direction		= lights->GetUniform("lights[" + std::to_string(i) + "].direction");
			lightData.diffuse		= lights->GetUniform("lights[" + std::to_string(i) + "].diffuse");
			lightData.attenuation	= lights->GetUniform("lights[" + std::to_string(i) + "].attenuation");
			lightData.strength		= lights->GetUniform("lights[" + std::to_string(i) + "].strength");

            lightProperties.push_back(lightData);
        }
        
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
		renderList.clear();

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
                TransformListPtr children = nodeTrans->GetChildren();

				std::for_each(children->begin(), children->end(), [&](Transform::Ptr transform){
					
					SceneNode::Ptr childNode = std::dynamic_pointer_cast<epsilon::SceneNode>(transform->GetParent());

					if (childNode->GetRenderer())
					{
						renderList.push_back(childNode->GetRenderer());
					}

					if (transform->HasChildren())
					{
						CullNodeChildren(childNode);
					}
				});
            }
        }
	}
    
    void SceneManager::PreDraw()
    {
        // Push the matrices to the uniform buffer
        viewMatrixUnf->SetMatrix4(currentScene->GetActiveCamera()->GetViewMatrix());
        projMatrixUnf->SetMatrix4(currentScene->GetActiveCamera()->GetProjectionMatrix());
        
        // Push the lights to the uniform buffer
        LightList sceneLights = currentScene->GetLights();
        
		if (numLights)
		{
			numLights->SetInt(sceneLights.size());
		}
        
        // TODO: An optimisation here would be to track light changes and only push changed data.
		for ( int i = 0; i < sceneLights.size(); i++ )
        {
			lightProperties[i].position->SetVector3(sceneLights[i]->GetPosition());
            lightProperties[i].direction->SetVector3(sceneLights[i]->GetDirection());
            lightProperties[i].diffuse->SetVector4(sceneLights[i]->diffuse.ToVector4());
            lightProperties[i].attenuation->SetVector4(sceneLights[i]->attenuation);
            lightProperties[i].strength->SetFloat(sceneLights[i]->strength);
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
		
        
//		stateStack->State()->view = currentScene->GetActiveCamera()->GetViewMatrix();
//		stateStack->State()->projection = currentScene->GetActiveCamera()->GetProjectionMatrix();
//		stateStack->State()->lights = currentScene->GetLights();
		
		for ( RenderList::iterator renderer = renderList.begin(); renderer != renderList.end(); renderer++)
		{
			// Render
			(*renderer)->Draw(stateStack);//viewMatrix, projMatrix);
		}

		stateStack->Pop();
	}

}