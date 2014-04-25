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

		static Mesh::Ptr Create();

		static Mesh::Ptr CreateFromFile(std::string filename);

		explicit Mesh(const private_struct &);
		explicit Mesh(const private_struct &, std::string filename);

		~Mesh(void);

		VertexData::Ptr VertexData() { return vd; }

		bool Draw();
		bool DrawOk() { return drawOk; }
        void Destroy();

		bool SetMeshLoader(MeshLoaderInterface::Ptr iMeshLoader);

		void LoadMesh();
		void RefreshFromFile();

		// Refresh OpenGL mesh data
		void RefreshBuffers();
		void ProcessMesh();

	private:
		VertexData::Ptr vd;
		VertexData::Ptr reloadVd;
		bool			drawOk;
		bool			needGPUCopy;

		MeshLoaderInterface::Ptr meshLoader;
	};

}