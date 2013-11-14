#pragma once

#include "logging/Logging.h"
#include "scene/Scene.h"
#include "scene/Node.h"
#include "scene/Transform.h"

namespace epsilon
{
	using namespace std;

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
		void SetScene(string name);

		void Update(float el);
		void Cull(void);
		void Draw(void);

	private:

		void CullNodeChildren(Node::Ptr node);

		SceneList scenes;
		Scene::Ptr currentScene;
		RenderList renderList;
	};

}