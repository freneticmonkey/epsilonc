#pragma once

#include "logging/Logging.h"
#include "scene/Scene.h"
#include "scene/SceneNode.h"
#include "scene/Transform.h"

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
        
		void Draw();

	private:

		void CullNodeChildren(SceneNode::Ptr node);

		SceneList scenes;
		Scene::Ptr currentScene;
		RenderList renderList;
	};

}