#include "render/MeshFactory.h"
#include "math/Defines.h"

#include <cmath>

namespace epsilon
{
	MeshFactory::MeshFactory(void)
	{
	}

	MeshFactory::~MeshFactory(void)
	{
	}

	Mesh::Ptr MeshFactory::GenerateCube()
	{
		return Mesh::Create();

		VectorList verts;
		VectorList norms;
		TexCoordList texCoords;
		IndicesList faces;

		VecVec raverts;
		/*
		faces = [
            [0, 1, 3],
            [3, 2, 0],#, 2], # left
            [4, 6, 7],
            [7, 5, 4],
#            [4, 6, 7, 5], # right
            [7, 3, 1],
            [1, 5, 7],
#            [7, 3, 1, 5], # front
            [0, 2, 6],
            [6, 4, 0],
#            [0, 2, 6, 4], # back
            [3, 7, 2], # top
            [7, 6, 2],
            [5, 1, 4], # bottom
            [1, 0, 4]
        ]
		*/


	}

	Mesh::Ptr MeshFactory::GenerateSphere(int slices, int stacks)
	{
		VectorList verts;
		VectorList norms;
		TexCoordList texCoords;
		IndicesList faces;

		// This is used purely for normal generation as it requires random access
		VecVec raverts;

		Mesh::Ptr newSphere = Mesh::Create();
		
		int vi, next;
		float x, y, z, u, v, theta, phi, a;

		// Calculate Verts and Tex coords
		for ( int i = 0; i < (slices + 1); i++ )
		{
			theta = i * PI / stacks;
			for ( int j = 0; j < stacks; j++ )
			{
				phi = j * 2.0f * PI / stacks;

				// vertices
				x = std::sin(theta) * std::cos(phi);
				y = std::cos(theta);
				z = -std::sin(theta) * std::sin(phi);
				verts.push_back( Vector3(x, y, z) );

				raverts.push_back( Vector3(x, y, z) );
				// Tex Coord
				v = std::acos(z) / PI;
                
                a = x / (std::sin(PI*(v)));
				
                if (y >= 0)
                {
					u = std::acos( a ) / TWOPI;
                }
				else
                {
					u = (PI + std::acos( a ) ) / TWOPI;
                }
				texCoords.push_back( Vector2(u, v) );
			}
		}

		// Calculate Faces
		vi = 0;
		for ( int i = 0; i < slices; i++ )
		{
			for ( int j = 0; j < stacks; j++ )
			{
				next = (j + 1) % stacks;
				
				faces.push_back( vi + j + stacks);
				faces.push_back( vi + next);
				faces.push_back( vi + j );

				faces.push_back( vi + j + stacks);
				faces.push_back( vi + next + stacks);
				faces.push_back( vi + next);
			}
			vi += stacks;
		}

		norms = MeshFactory::GenerateNormals(raverts, faces);

		newSphere->SetMeshData(verts, norms, texCoords, faces);

		return newSphere;
	}

	Mesh::Ptr MeshFactory::GeneratePlane(int widthSegments, int heightSegments)
	{
		Mesh::Ptr newPlane = Mesh::Create();
		
		// Create a triangle
		VectorList verts;
		VectorList norms;
		ColourList colours;
		TexCoordList tc;
		IndicesList inds;

		float width = 1.0f;
		float height = 1.0f;

		float halfWidth = width/2.0f;
		float halfHeight = height/2.0f;

		float widthInc = width/widthSegments;
		float heightInc = height/heightSegments;
		float normalDir = 1.0f;

		for (float x = -halfWidth; x < halfWidth; x+=widthInc)
		{
			for (float y = -halfHeight; y < halfHeight; y+=heightInc)
			{
				float x2 = x + widthInc;
				float y2 = y + heightInc;

				// TL Triangle
				verts.push_back(Vector3(x,y,0));
				verts.push_back(Vector3(x,y2,0));
				verts.push_back(Vector3(x2,y2,0));

				// BR Triangle
				verts.push_back(Vector3(x,y,0));
				verts.push_back(Vector3(x2,y2,0));
				verts.push_back(Vector3(x2,y,0));

				norms.push_back(Vector3(0,0,normalDir));
				norms.push_back(Vector3(0,0,normalDir));
				norms.push_back(Vector3(0,0,normalDir));

				norms.push_back(Vector3(0,0,normalDir));
				norms.push_back(Vector3(0,0,normalDir));
				norms.push_back(Vector3(0,0,normalDir));

				colours.push_back(Vector4(1,0,0,1));
				colours.push_back(Vector4(0,1,0,1));
				colours.push_back(Vector4(0,0,1,1));
				colours.push_back(Vector4(1,0,0,1));
				colours.push_back(Vector4(0,1,0,1));
				colours.push_back(Vector4(0,0,1,1));

				tc.push_back(Vector2(x/width,y/height));
				tc.push_back(Vector2(x/width,y2/height));
				tc.push_back(Vector2(x2/width,y2/height));

				tc.push_back(Vector2(x/width,y/height));
				tc.push_back(Vector2(x2/width,y2/height));
				tc.push_back(Vector2(x2/width,y/height));

				inds.push_back(inds.size());
				inds.push_back(inds.size());
				inds.push_back(inds.size());
				inds.push_back(inds.size());
				inds.push_back(inds.size());
				inds.push_back(inds.size());
			}
		}

		newPlane->SetMeshData(verts, norms, colours, tc, inds);

		return newPlane;
	}

	Mesh::Ptr MeshFactory::GenerateTriangle()
	{
		// Create a triangle
		VectorList verts;
		VectorList norms;
		ColourList colours;
		TexCoordList tc;
		IndicesList inds;

		Mesh::Ptr newTriangle = Mesh::Create();

		verts.push_back(Vector3(-0.5,-0.5,0));
		verts.push_back(Vector3(0,0.5,0));
		verts.push_back(Vector3(0.5,-0.5,0));

		norms.push_back(Vector3(0,0,-1));
		norms.push_back(Vector3(0,0,-1));
		norms.push_back(Vector3(0,0,-1));

		colours.push_back(Vector4(1,0,0,1));
		colours.push_back(Vector4(0,1,0,1));
		colours.push_back(Vector4(0,0,1,1));

		tc.push_back(Vector2(0,0));
		tc.push_back(Vector2(0,0));
		tc.push_back(Vector2(0,0));

		inds.push_back(0);
		inds.push_back(1);
		inds.push_back(2);

		newTriangle->SetMeshData(verts, norms, colours, tc, inds);

		return newTriangle;
	}

	Mesh::Ptr MeshFactory::GenerateIcoHedron()
	{
		return Mesh::Create();
	}

	Mesh::Ptr MeshFactory::GenerateOctohedron()
	{
		return Mesh::Create();
	}

	VectorList MeshFactory::GenerateNormals(VecVec verts, IndicesList indices)
	{
		VectorList norms;// = VecVec(verts.size());
		std::vector<VecVec> vertNorms = std::vector<VecVec>(verts.size());
		VectorList faceNormals;
		Vector3 faceNormal, v0, v1, v2, a, b;
		int vec0, vec1, vec2;
		bool found;

		// Generate face normals
		for (IndicesList::iterator ind = indices.begin(); ind != indices.end(); ind++ )
		{
			vec0 = *ind++;
			vec1 = *ind++;
			vec2 = *ind++;

			v0 = verts[vec0];
			v1 = verts[vec1];
			v2 = verts[vec2];

			a = v0 - v1;
			b = v2 - v1;
			b.Cross(a).Normalise();
			
			// add face normal to the vec positions list in vec0, vec1, vec2
			found = false;
			for (VecVec::iterator vnIt = vertNorms[vec0].begin(); vnIt != vertNorms[vec0].end(); vnIt++ )
			{
				if ( *vnIt == b )
				{
					found = true;
					break;
				}
			}

			if ( !found )
			{
				vertNorms[vec0].push_back(b);
			}

			found = false;
			for (VecVec::iterator vnIt = vertNorms[vec1].begin(); vnIt != vertNorms[vec1].end(); vnIt++ )
			{
				if ( *vnIt == b )
				{
					found = true;
					break;
				}
			}

			if ( !found )
			{
				vertNorms[vec1].push_back(b);
			}

			found = false;
			for (VecVec::iterator vnIt = vertNorms[vec2].begin(); vnIt != vertNorms[vec2].end(); vnIt++ )
			{
				if ( *vnIt == b )
				{
					found = true;
					break;
				}
			}

			if ( !found )
			{
				vertNorms[vec2].push_back(b);
			}
		}

		// Calculate the average for all of the face normals for each of the vectors
		for ( std::vector<VecVec>::iterator vnIt = vertNorms.begin(); vnIt != vertNorms.end(); vnIt++ )
		{
			faceNormal = Vector3();

			for (VecVec::iterator vnFnIt = (*vnIt).begin(); vnFnIt != (*vnIt).end(); vnFnIt++ )
			{
				faceNormal += *vnFnIt;
			}
			faceNormal.Normalise();
			norms.push_back(faceNormal);
		}

		return norms;

		 /*normals = list( MeshUtilities.face_normal(vertices, face) for face in faces )
            
            # Normal Generation
            
            # Calculate the normals once for each unique vertex and after calcs done, 
            # rebuild the vertex list
            vertex_normals = []
            
            # for each vertex
            for v_inc in range(len(vertices)):
                face_normals = []
                vert_norm = Vector3()
                
                # for each face
                for f_inc in range(len(faces)):
                    
                    # if the face contains the current vertex
                    if v_inc in faces[f_inc]:
                        # and the faces normal isn't already stored i.e. ignore co-planar face normals
                        if normals[f_inc] not in face_normals:
                            # Store it
                            face_normals.append(normals[f_inc])
                
                # Calculate the average for all of the face normals found
             
                for f_norm in face_normals:
                    vert_norm += f_norm
                vert_norm.normalize()
                
                vertex_normals.append(vert_norm)
            
            for vi in range(len(self._vert_index)):
                v_ind = self._vert_index[vi]
                self._normals.append(vertex_normals[v_ind])*/
	}

}