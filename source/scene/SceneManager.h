#pragma once

#include "logging/Logging.h"
#include "scene/Scene.h"
#include "scene/SceneNode.h"
#include "scene/Transform.h"
#include "render/RenderState.h"

#include "render/material/ShaderManager.h"
#include "render/material/UniformBuffer.h"
#include "render/Light.h"

namespace epsilon
{
	class Renderer;

	typedef std::list<Scene::Ptr> SceneList;
	typedef std::list<Renderer::Ptr> RenderList;

	// Making this a Singleton
	class SceneManager
	{
	private:
		SceneManager(void);
		SceneManager(SceneManager const&);
		void operator=(SceneManager const&);

	public:
		static SceneManager & GetInstance()
		{
			static SceneManager instance;
			return instance;
		};

		~SceneManager(void);

		Scene::Ptr CurrentScene() { return currentScene; }

		void Setup(void);
		void SetScene(Scene::Ptr newScene);
		void SetScene(std::string name);

		void Update(float el);
		void Cull(void);
        
        void PreDraw();
        
		void Draw(RenderStateStack::Ptr stateStack);

		Matrix4 GetCurrentViewMatrix();
		Matrix4 GetCurrentProjMatrix();

	private:

		void CullNodeChildren(SceneNode::Ptr node);

		SceneList scenes;
		Scene::Ptr currentScene;
		RenderList renderList;

		Matrix4 viewMatrix;
		Matrix4 projMatrix;
        
        ShaderUniform::Ptr viewMatrixUnf;
        ShaderUniform::Ptr projMatrixUnf;
        
        UniformBuffer::Ptr globalMatrices;
        UniformBuffer::Ptr lights;
        ShaderUniform::Ptr numLights;
        std::vector<LightUniforms> lightProperties;
	};

}