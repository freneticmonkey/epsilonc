#pragma once

#include "logging/Logging.h"
#include "scene/Scene.h"
#include "scene/Node.h"
#include "scene/Transform.h"

namespace epsilon
{
	using namespace std;

	typedef list<Scene::Ptr> SceneList;
	typedef list<Renderer::Ptr> RenderList;

	class SceneManager
	{
	public:
		SceneManager(void);
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