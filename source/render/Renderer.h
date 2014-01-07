#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"
#include "render/Material.h"
#include "render/Mesh.h"
#include "render/RenderState.h"

namespace epsilon
{
	class Renderer :
		public NodeComponent
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<Renderer> Ptr;

		static Renderer::Ptr Create();
		static Renderer::Ptr Create(Mesh::Ptr newMesh);
		static Renderer::Ptr Create(Mesh::Ptr newMesh, Material::Ptr newMaterial);
		
		explicit Renderer(const private_struct &);
		explicit Renderer(const private_struct &, Mesh::Ptr newMesh);
		explicit Renderer(const private_struct &, Mesh::Ptr newMesh, Material::Ptr newMaterial);
		~Renderer(void);

		void Draw(RenderStateStack::Ptr stateStack);//Matrix4 viewMatrix, Matrix4 projMatrix);

		void SetMesh(Mesh::Ptr newMesh);
		Mesh::Ptr GetMesh();

		void SetMaterial(Material::Ptr newMaterial);
		Material::Ptr GetMaterial();

	private:
		Material::Ptr material;
		Mesh::Ptr mesh;
	};

}