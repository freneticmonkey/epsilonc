#pragma once 
#include "render/mesh/MeshLoaderInterface.h"
#include "render/mesh/VertexData.h"

namespace epsilon
{
	struct PackedVertex
	{
		Vector3 position;
		Vector2 uv;
		Vector3 normal;
		bool operator<(const PackedVertex that) const
		{
			return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
		};
	};

	class MeshLoaderOBJ : public MeshLoaderInterface
	{
	private:
		struct private_struct {};

	public:
		typedef std::shared_ptr<MeshLoaderOBJ> Ptr;

		static MeshLoaderOBJ::Ptr Create();
		explicit MeshLoaderOBJ(const private_struct &);

		void LoadMesh(std::string filename, VertexData::Ptr vertexData);
		void RefreshMesh(std::string filename, VertexData::Ptr vertexData);

	private:
		bool LoadOBJ(std::string filename, VertexData::Ptr vertexData);

		bool GetSimilarVertexIndex_fast(PackedVertex & packed,
			std::map<PackedVertex, unsigned short> & VertexToOutIndex,
			unsigned short & result
			);

		void IndexVBO(
			std::vector<Vector3> & in_vertices,
			std::vector<Vector2> & in_uvs,
			std::vector<Vector3> & in_normals,

			std::vector<GLushort> & out_indices,
			std::vector<Vector3> & out_vertices,
			std::vector<Vector2> & out_uvs,
			std::vector<Vector3> & out_normals
			);
	};

	
}