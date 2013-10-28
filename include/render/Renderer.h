#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"

#include "render/Mesh.h"
#include "render/Shader.h"

namespace epsilon
{
	class Renderer :
		public NodeComponent
	{
	private:
		struct private_struct {};

	public:
		typedef shared_ptr<Renderer> Ptr;

		static Renderer::Ptr Create();
		static Renderer::Ptr Create(Mesh::Ptr newMesh);
		
		explicit Renderer(const private_struct &);
		~Renderer(void);

		void Draw(Matrix4 viewMatrix, Matrix4 projMatrix);

		void SetMesh(Mesh::Ptr newMesh);
		Mesh::Ptr GetMesh();

		void SetShader(Shader::Ptr newShader);
		Shader::Ptr GetShader();

	private:
		Mesh::Ptr mesh;
		Shader::Ptr shader;
	};

}