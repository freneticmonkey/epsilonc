#pragma once

#include "EpsilonCore.h"
#include "render/Mesh.h"

namespace epsilon
{
	typedef std::vector<Vector3> VecVec;

	struct ParametricData
	{
		VectorList vertices;
		IndicesList indices;
	};

	class Parametric
	{
	public:
		static ParametricData Sphere(int slices = 8, int stacks = 8);
	};

	class MeshFactory
	{
	public:
		MeshFactory(void);
		~MeshFactory(void);

		static Mesh::Ptr GenerateGrid(int spacers);
		static Mesh::Ptr GenerateCube();
		static Mesh::Ptr GenerateSphere(int slices = 8, int stacks = 8);
		static Mesh::Ptr GeneratePlane(int widthSegments = 1, int heightSegments = 1);
		static Mesh::Ptr GenerateTriangle();
		static Mesh::Ptr GenerateIcoHedron();
		static Mesh::Ptr GenerateOctohedron();

		static VectorList GenerateNormals(VecVec verts, IndicesList indices);
		
	};

}