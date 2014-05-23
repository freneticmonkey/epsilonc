#pragma once

#include "EpsilonCore.h"

#include "resource/Resource.h"
#include "scene/Object.h"
#include "render/mesh/VertexData.h"
#include "render/mesh/MeshLoaderInterface.h"

namespace epsilon
{
	class Mesh :
		public Resource,
		public Object
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Mesh> Ptr;

		static Mesh::Ptr Create(GLenum type = GL_TRIANGLES);

		static Mesh::Ptr CreateFromFile(std::string filename, GLenum type = GL_TRIANGLES);

		explicit Mesh(const private_struct &, GLenum type);
		explicit Mesh(const private_struct &, GLenum type, std::string filename);

		~Mesh(void);

		VertexData::Ptr VertexData() { return vd; }
        
        // Serialisation Hack until I implement a better way of separating meshes.
        void SetMeshType(std::string type) { meshType = type; }
        std::string GetMeshType() { return meshType; }
        
        void SetMeshParameters(std::string param) { meshParameters = param; }
        std::string GetMeshParameters() { return meshParameters; }

		bool Draw();
		bool DrawOk() { return drawOk; }
        void Destroy();

		bool SetMeshLoader(MeshLoaderInterface::Ptr iMeshLoader);

		void LoadMesh();
		void RefreshFromFile();

		// Refresh OpenGL mesh data
		void RefreshBuffers();
		void ProcessMesh();

		// Return the bounds of the mesh
		const Bounds& GetBounds() { return vd->GetBounds(); }

	private:
		VertexData::Ptr vd;
		VertexData::Ptr reloadVd;
		bool			drawOk;
		bool			needGPUCopy;
        
        std::string     meshType;
        std::string     meshParameters;

		MeshLoaderInterface::Ptr meshLoader;
	};

}