#pragma once
#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"
#include "render/material/Material.h"
#include "render/mesh/Mesh.h"

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

		// Standard Draw Function
		void Draw();

		// Draw Function with material override.
		void Draw(Material::Ptr drawMaterial);
        void Destroy();
        
		void SetMesh(Mesh::Ptr newMesh);
		void SetMeshByName(std::string meshName);

		Mesh::Ptr GetMesh();

		void SetMaterial(Material::Ptr newMaterial);
		Material::Ptr GetMaterial();
	
	protected:
		void UpdateBounds();

	private:
		Material::Ptr material;
		Mesh::Ptr mesh;
	};
	
	// Declaring container
	typedef std::vector<Renderer::Ptr> Renderers;
}