#pragma once

#include "EpsilonCore.h"
#include "render/mesh/Mesh.h"

namespace epsilon
{
	typedef std::vector<Vector3> VecVec;

	enum ParametericType
	{
		SPHERE = 0,
		PLANE,
		CUBE,
	};

	struct ParametricData
	{
		VerticesAttrib::List vertices;
		NormalAttrib::List normals;
		ColourAttrib::List colours;
		VertexIndicesBuffer::List indices;
		TexCoordAttrib::List texCoords;
	};

	class Parametric
	{
	public:
		static ParametricData Sphere(int slices = 8, int stacks = 8);
		static ParametricData Plane(int rows = 8, int columns = 8);
	};

	class MeshFactory
	{
	public:
		MeshFactory(void);
		~MeshFactory(void);

		static Mesh::Ptr GenerateGrid(int size, int resolution);
		static Mesh::Ptr GenerateCube();
		static Mesh::Ptr GenerateWireCube();
		static Mesh::Ptr GenerateSphere(int slices = 8, int stacks = 8);
		static Mesh::Ptr GenerateWireSphere();
		static Mesh::Ptr GeneratePlane(int widthSegments = 1, int heightSegments = 1);
		static Mesh::Ptr GenerateTriangle();
		static Mesh::Ptr GenerateIcoHedron();
		static Mesh::Ptr GenerateOctohedron();

		static NormalAttrib::List GenerateNormals(VerticesAttrib::List verts, 
												  VertexIndicesBuffer::List indices);
		
	};

}