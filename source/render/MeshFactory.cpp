#include "render/MeshFactory.h"
#include "math/Defines.h"

#include <cmath>

namespace epsilon
{
	ParametricData Parametric::Sphere(int slices, int stacks)
	{
		ParametricData data;
		float theta, phi;
		int v = 0;
		int next = 0;

		// Generate Vertices
		for (int x = 0; x < (slices+1); x++)
		{
			theta = x * PI / slices;
			for( int y = 0; y < stacks; y++)
			{
				phi = y * TWOPI / stacks;
				float px = sin(theta) * cos(phi);
				float py = cos(theta);
				float pz = -sin(theta) * sin(phi);
				data.vertices.push_back( Vector3( px, py, pz) );
				data.normals.push_back( Vector3( px, py, pz).Normalised() );
			}
		}

		// Generate Faces/Indices
		for (int x = 0; x < slices; x++ )
		{
			for (int y = 0; y < stacks; y++ )
			{
				next = ( y + 1 ) % stacks;
				// first triangle
				data.indices.push_back( v + y + stacks );
				data.indices.push_back( v + next );
				data.indices.push_back( v + y );

				// second triangle
				data.indices.push_back( v + y + stacks );
				data.indices.push_back( v + next + stacks);
				data.indices.push_back( v + next );
			}
			v += stacks;
		}

		return data;
	}

	ParametricData Parametric::Plane(int rows, int columns)
	{
		ParametricData data;

		float c, r;
		int tl, tr, bl, br;
		int v = 0;

		// Generate Vertices
		for ( int x = 0; x < (columns + 1); x++ )
		{
			c = x * ( 1.0f / columns );
			for ( int y = 0; y < (rows + 1); y++ )
			{
				r = y * ( 1.0f / rows );
				data.vertices.push_back( Vector3( (-0.5f + c),
												  0.0f,
												  (-0.5f + r) ) );
			}
		}

		// Generate Normals
		for ( int n = 0; n < ((rows + 1) * (columns + 1)); n++ )
		{
			//data.normals.push_back(Vector3::UP);
			data.normals.push_back(Vector3(1.0f, 0.5f, 0.0f));
			data.colours.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// Generate Faces/Indices
		for ( int x = 0; x < columns; x++ )
		{
			for ( int y = 0; y < rows; y++ )
			{
				bl = v + x + y;
				tl = bl + 1;
				tr = tl + rows + 1;
				br = tr - 1;
				data.indices.push_back(bl);
				data.indices.push_back(tl);
				data.indices.push_back(tr);

				data.indices.push_back(bl);
				data.indices.push_back(tr);
				data.indices.push_back(br);
			}
			v += rows;
		}
		
		// Generate Texture Coordinates
		for (int x = 0; x < (columns + 1); x++ )
		{
			c = x * ( 1.0f / columns);
			for ( int y = 0; y < (rows + 1); y++ )
			{
				r = y * ( 1.0f / rows );
				data.texCoords.push_back(Vector2(c, r));
			}
		}

		return data;
	}

	MeshFactory::MeshFactory(void)
	{
	}

	MeshFactory::~MeshFactory(void)
	{
	}
	Mesh::Ptr MeshFactory::GenerateGrid(int size, int resolution)
	{
		Mesh::Ptr newGrid = Mesh::Create();

		VerticesAttrib::List verts;
		//ColourAttrib::List
		float halfWidth = size / 2.0f;
		

		// if the resolution doesn't evenly divide into the size
		if ( (size % resolution) > 0.0f )
		{
			float diff = size % resolution;

			// modify the size so that the resolution evenly fits
			halfWidth -= diff / 2.0f;
		}

		// Impossible to have a grid smaller than its resolution
		if ( size > resolution )
		{
			float resStep =  resolution;// / halfWidth;

			// 'Vertical' Lines first
			for ( float x = -halfWidth; x < halfWidth; x += resStep)
			{
				verts.push_back(Vector3(x,0,-halfWidth));
				verts.push_back(Vector3(x,0,halfWidth));
			}

			// 'Horizontal' lines
			for ( float z = -halfWidth; z < halfWidth; z += resStep)
			{
				verts.push_back(Vector3(-halfWidth,0,z));
				verts.push_back(Vector3(halfWidth,0,z));
			}

			// Border
			// TL -> TR
			verts.push_back(Vector3(-halfWidth,0,halfWidth));
			verts.push_back(Vector3(halfWidth,0,halfWidth));
			// TR - BR
			verts.push_back(Vector3(halfWidth,0,halfWidth));
			verts.push_back(Vector3(halfWidth,0,-halfWidth));
			// BR -> BL
			verts.push_back(Vector3(halfWidth,0,-halfWidth));
			verts.push_back(Vector3(-halfWidth,0,-halfWidth));
			// BL -> TL
			verts.push_back(Vector3(-halfWidth,0,-halfWidth));
			verts.push_back(Vector3(-halfWidth,0,halfWidth));
		}

		newGrid->VertexData()
			   ->SetVertices(verts);
			   //->BuildBuffers();

		return newGrid;
	}

	Mesh::Ptr MeshFactory::GenerateCube()
	{
		Mesh::Ptr newCube =  Mesh::Create();

		VerticesAttrib::List verts;
		//NormalAttrib::List norms;
		//TexCoordAttrib::List texCoords;
		VertexIndicesBuffer::List faces;

		verts.push_back(Vector3(-0.5, -0.5, -0.5));
		verts.push_back(Vector3(-0.5, -0.5, 0.5));
		verts.push_back(Vector3(-0.5, 0.5, -0.5));
		verts.push_back(Vector3(-0.5, 0.5, 0.5));
		verts.push_back(Vector3(0.5, -0.5, -0.5));
		verts.push_back(Vector3(0.5, -0.5, 0.5));
		verts.push_back(Vector3(0.5, 0.5, -0.5));
		verts.push_back(Vector3(0.5, 0.5, 0.5));

		// left
		faces.push_back(0);
		faces.push_back(1);
		faces.push_back(3);
		faces.push_back(3);
		faces.push_back(2);
		faces.push_back(0); 

		// right
		faces.push_back(4);
		faces.push_back(6);
		faces.push_back(7);
		faces.push_back(7);
		faces.push_back(5);
		faces.push_back(4);

		// front
		faces.push_back(7);
		faces.push_back(3);
		faces.push_back(1);
		faces.push_back(1);
		faces.push_back(5);
		faces.push_back(7);

		// back
		faces.push_back(0);
		faces.push_back(2);
		faces.push_back(6);
		faces.push_back(6);
		faces.push_back(4);
		faces.push_back(0);

		// top
		faces.push_back(3);
		faces.push_back(7);
		faces.push_back(2); 
		faces.push_back(7);
		faces.push_back(6);
		faces.push_back(2);

		// bottom
		faces.push_back(5);
		faces.push_back(1);
		faces.push_back(4); 
		faces.push_back(1);
		faces.push_back(0);
		faces.push_back(4);
		
		newCube->VertexData()
			   ->SetVertices(verts)
			   ->SetIndices(faces);
		
		return newCube;
	}

	Mesh::Ptr MeshFactory::GenerateWireCube()
	{
		Mesh::Ptr newCube = Mesh::Create();

		VerticesAttrib::List verts;

		verts.push_back(Vector3(-0.5, -0.5, -0.5));
		verts.push_back(Vector3(-0.5, -0.5, 0.5));

		verts.push_back(Vector3(-0.5, -0.5, 0.5));
		verts.push_back(Vector3( 0.5, -0.5, 0.5));

		verts.push_back(Vector3(-0.5, 0.5, -0.5));
		verts.push_back(Vector3(-0.5, 0.5, 0.5));

		verts.push_back(Vector3(-0.5, 0.5, 0.5));
		verts.push_back(Vector3( 0.5, 0.5, 0.5));

		verts.push_back(Vector3(0.5, -0.5, -0.5));
		verts.push_back(Vector3(0.5, -0.5, 0.5));

		verts.push_back(Vector3(-0.5, 0.5, -0.5));
		verts.push_back(Vector3( 0.5, 0.5, -0.5));

		verts.push_back(Vector3(0.5, 0.5, -0.5));
		verts.push_back(Vector3(0.5, 0.5, 0.5));

		verts.push_back(Vector3(-0.5, -0.5, -0.5));
		verts.push_back(Vector3(0.5,  -0.5, -0.5));
			
		// Corner Verticals
		verts.push_back(Vector3(-0.5, -0.5, -0.5));
		verts.push_back(Vector3(-0.5,  0.5, -0.5));

		verts.push_back(Vector3(0.5, -0.5, -0.5));
		verts.push_back(Vector3(0.5,  0.5, -0.5));

		verts.push_back(Vector3(0.5, -0.5, 0.5));
		verts.push_back(Vector3(0.5, 0.5,  0.5));

		verts.push_back(Vector3(-0.5, -0.5, 0.5));
		verts.push_back(Vector3(-0.5,  0.5, 0.5));


		newCube->VertexData()
			->SetVertices(verts);

		return newCube;
	}

	Mesh::Ptr MeshFactory::GenerateSphere(int slices, int stacks)
	{
		VerticesAttrib::List verts;
		NormalAttrib::List norms;
		ColourAttrib::List colours;

		TexCoordAttrib::List texCoords;
		VertexIndicesBuffer::List faces;

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
				colours.push_back( Vector4(1.0f) );

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

		//newSphere->SetMeshData(verts, norms, texCoords, faces);
		newSphere->VertexData()
				 ->SetVertices(verts)
				 ->SetNormals(norms)
				 ->SetColours(colours)
				 ->SetTexCoords(texCoords)
				 ->SetIndices(faces);
				 //->BuildBuffers();

		return newSphere;
	}

	Mesh::Ptr MeshFactory::GenerateWireSphere()
	{
		Mesh::Ptr newSphere = Mesh::Create();
		VerticesAttrib::List verts;

		ParametricData data;
		int slices = 8;
		int stacks = 8;
		float theta, phi;
		int v = 0;
		int next = 0;

		Vector3 last;
		Vector3 now;
		Vector3 first;
		// Generate Vertices

		for (int x = 0; x < (slices + 1); x++)
		{
			theta = x * PI / slices;
			for (int y = 0; y < stacks; y++)
			{
				phi = y * TWOPI / stacks;
				float px = sin(theta) * cos(phi);
				float py = cos(theta);
				float pz = -sin(theta) * sin(phi);

				now = Vector3(px, py, pz);
				
				if (y == 0)
				{
					first = now;
				}
				else if ( y % 2 == 0)
				{
					verts.push_back(last);
					verts.push_back(now);
				}
				else if (y == (stacks - 1))
				{
					verts.push_back(now);
					verts.push_back(first);
				}
				verts.push_back(now);

				last = now;
			}
		}

		newSphere->VertexData()
				 ->SetVertices(verts);

		return newSphere;
	}

	Mesh::Ptr MeshFactory::GeneratePlane(int widthSegments, int heightSegments)
	{
		Mesh::Ptr newPlane = Mesh::Create();
		
		ParametricData planeData = Parametric::Plane(heightSegments, widthSegments);

		newPlane->VertexData()
				->SetVertices(planeData.vertices)
				->SetNormals(planeData.normals)
				->SetColours(planeData.colours)
				->SetTexCoords(planeData.texCoords)
				->SetIndices(planeData.indices);
				//->BuildBuffers();

		return newPlane;

		//// Create a triangle
		//VerticesAttrib::List verts;
		//NormalAttrib::List norms;
		//ColourAttrib::List colours;
		//TexCoordAttrib::List tc;
		//VertexIndicesBuffer::List inds;

		//float width = 1.0f;
		//float height = 1.0f;

		//float halfWidth = width/2.0f;
		//float halfHeight = height/2.0f;

		//float widthInc = width/widthSegments;
		//float heightInc = height/heightSegments;
		//float normalDir = 1.0f;
		//
		//for (float x = -halfWidth; x < halfWidth; x+=widthInc)
		//{
		//	float x2 = x + widthInc;

		//	for (float y = -halfHeight; y < halfHeight; y+=heightInc)
		//	{	
		//		float y2 = y + heightInc;

		//		// TL Triangle
		//		verts.push_back(Vector3(x,y,0));
		//		verts.push_back(Vector3(x,y2,0));
		//		verts.push_back(Vector3(x2,y2,0));

		//		// BR Triangle
		//		verts.push_back(Vector3(x,y,0));
		//		verts.push_back(Vector3(x2,y2,0));
		//		verts.push_back(Vector3(x2,y,0));
		//		
		//		norms.push_back(Vector3(0,0,normalDir));
		//		norms.push_back(Vector3(0,0,normalDir));
		//		norms.push_back(Vector3(0,0,normalDir));

		//		norms.push_back(Vector3(0,0,normalDir));
		//		norms.push_back(Vector3(0,0,normalDir));
		//		norms.push_back(Vector3(0,0,normalDir));
		//		
		//		colours.push_back(Vector4(1,0,0,1));
		//		colours.push_back(Vector4(0,1,0,1));
		//		colours.push_back(Vector4(0,0,1,1));
		//		
		//		colours.push_back(Vector4(1,0,0,1));
		//		colours.push_back(Vector4(0,1,0,1));
		//		colours.push_back(Vector4(0,0,1,1));

		//		tc.push_back(Vector2(x/width,y/height));
		//		tc.push_back(Vector2(x/width,y2/height));
		//		tc.push_back(Vector2(x2/width,y2/height));

		//		tc.push_back(Vector2(x/width,y/height));
		//		tc.push_back(Vector2(x2/width,y2/height));
		//		tc.push_back(Vector2(x2/width,y/height));

		//		inds.push_back(inds.size());
		//		inds.push_back(inds.size());
		//		inds.push_back(inds.size());
		//		
		//		inds.push_back(inds.size());
		//		inds.push_back(inds.size());
		//		inds.push_back(inds.size());
		//	}
		//}

		////newPlane->SetMeshData(verts, norms, colours, tc, inds);
		//newPlane->VertexData()
		//		 ->SetVertices(verts)
		//		 .SetNormals(norms)
		//		 .SetTexCoords(tc)
		//		 .SetIndices(inds)
		//		 .BuildBuffers();

		//return newPlane;
	}

	Mesh::Ptr MeshFactory::GenerateTriangle()
	{
		// Create a triangle
		VerticesAttrib::List verts;
		NormalAttrib::List norms;
		ColourAttrib::List colours;
		TexCoordAttrib::List tc;
		VertexIndicesBuffer::List inds;

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

		//newTriangle->SetMeshData(verts, norms, colours, tc, inds);
		newTriangle->VertexData()
				   ->SetVertices(verts)
				   ->SetNormals(norms)
				   ->SetTexCoords(tc)
				   ->SetIndices(inds);
				   //->BuildBuffers();

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

	NormalAttrib::List MeshFactory::GenerateNormals(VerticesAttrib::List verts, VertexIndicesBuffer::List indices)
	{
		NormalAttrib::List norms;// = VecVec(verts.size());
		std::vector<VecVec> vertNorms = std::vector<VecVec>(verts.size());
		VerticesAttrib::List faceNormals;
		Vector3 faceNormal, v0, v1, v2, a, b;
		int vec0, vec1, vec2;
		bool found;

		// Generate face normals
		for (VertexIndicesBuffer::List::iterator ind = indices.begin(); ind != indices.end(); ind++ )
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