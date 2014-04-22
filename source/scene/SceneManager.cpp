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
        
        globalMatrices = ShaderManager::GetInstance().GetUniformBuffer("GlobalMatrices");
        viewMatrixUnf = ShaderUniform::Create(0, GL_FLOAT_MAT4);
        projMatrixUnf = ShaderUniform::Create(1, GL_FLOAT_MAT4);
        globalMatrices->AddUniform(viewMatrixUnf)
                      ->AddUniform(projMatrixUnf);
        
        // Process lights
        lights = ShaderManager::GetInstance().GetUniformBuffer("Lights");
        
        // Create a control for the number of active lights
        int index = 0;
        numLights = ShaderUniform::Create(index++, GL_INT);
        lights->AddUniform(numLights);
        
        for (int i = 0; i < Light::MAX_LIGHTS; i++ )
        {
            // Create Uniforms for the light
            LightUniforms lightData;
            lightData.position      = ShaderUniform::Create(index++, GL_FLOAT_VEC3);
            lightData.direction     = ShaderUniform::Create(index++, GL_FLOAT_VEC3);
            lightData.diffuse       = ShaderUniform::Create(index++, GL_FLOAT_VEC4);
            lightData.attenuation   = ShaderUniform::Create(index++, GL_FLOAT_VEC4);
            lightData.strength      = ShaderUniform::Create(index++, GL_FLOAT);
            
            lightProperties.push_back(lightData);
            
            // Push the uniforms
            lights->AddUniform(lightData.position)
                  ->AddUniform(lightData.direction)
                  ->AddUniform(lightData.diffuse)
                  ->AddUniform(lightData.attenuation)
                  ->AddUniform(lightData.strength);
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
        
        numLights->SetInt(sceneLights.size());
        
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