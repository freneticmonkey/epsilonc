#pragma warning(disable: 4996)

#include "render/mesh/MeshLoaderOBJ.h"
#include "EpsilonCore.h"
#include "math/Vector.h"

namespace epsilon
{
	MeshLoaderOBJ::Ptr MeshLoaderOBJ::Create()
	{
		return std::make_shared<MeshLoaderOBJ>(private_struct());
	}

	MeshLoaderOBJ::MeshLoaderOBJ(const private_struct &) :
		MeshLoaderInterface(MeshLoaderInterface::private_struct())
	{
		extensions.push_back(".obj");
		extensions.push_back(".OBJ");
	}

	void MeshLoaderOBJ::LoadMesh(std::string filename, VertexData::Ptr vertexData)
	{
		LoadOBJ(filename, vertexData);
	}

	void MeshLoaderOBJ::RefreshMesh(std::string filename, VertexData::Ptr vertexData)
	{
		LoadOBJ(filename, vertexData);
	}

	bool MeshLoaderOBJ::LoadOBJ(std::string filename, VertexData::Ptr vertexData)
	{
		bool result = false;

		// OBJ Loading algorithm sourced from 
		// https://code.google.com/p/opengl-tutorial-org/source/browse/common/objloader.cpp
		// Really good OpenGL tutorials, check them out. :)
		// http://www.opengl-tutorial.org/
		//

		std::vector<GLushort> vertexIndices, uvIndices, normalIndices;
		std::vector<Vector3> vertices;
		std::vector<Vector2> uvs;
		std::vector<Vector3> normals;

		FILE * file = fopen(filename.c_str(), "r");

		if ( !file )
		{
			Log("MeshLoaderOBJ", "Sadness couldn't open file: " + filename);
			result = false;
		}
		else
		{
			// Read the file contents
			while (true)
			{
				char lineHeader[128];

				// read the line
				int res = fscanf(file, "%s", lineHeader);

				// If we've reached the end-of-file
				if (res == EOF)
				{
					// Mark success
					result = true;

					// Exit loop
					break;
				}

				// If the line is a vertex
				if (strcmp(lineHeader, "v") == 0)
				{
					Vector3 vert;
					fscanf(file, "%f %f %f", &vert.x, &vert.y, &vert.z);
					vertices.push_back(vert);
				}
				// else if the line is a UV
				else if (strcmp(lineHeader, "vt") == 0)
				{
					Vector2 uv;
					fscanf(file, "%f %f", &uv.x, &uv.y);
					uvs.push_back(uv);
				}
				// else if the line is normal
				else if (strcmp(lineHeader, "vn") == 0)
				{
					Vector3 normal;
					fscanf(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
					normals.push_back(normal);
				}
				// else the line is a face
				else if (strcmp(lineHeader, "f") == 0)
				{
					// This assumes that faces are exported last..
					if (vertices.size() > 0)
					{
						unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

						if ((uvs.size() > 0) && (normals.size() > 0))
						{
							int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								&vertexIndex[1], &uvIndex[1], &normalIndex[1],
								&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
							if (matches != 9)
							{
								Log("MeshLoaderOBJ", "Faces Error: File can't be read by our simple parser : ( Try exporting with other options: " + filename);

								// Break out of the loop.
								result = false;
								break;
							}
							else
							{
								// Process the faces
								vertexIndices.push_back(vertexIndex[0]);
								vertexIndices.push_back(vertexIndex[1]);
								vertexIndices.push_back(vertexIndex[2]);
								uvIndices.push_back(uvIndex[0]);
								uvIndices.push_back(uvIndex[1]);
								uvIndices.push_back(uvIndex[2]);
								normalIndices.push_back(normalIndex[0]);
								normalIndices.push_back(normalIndex[1]);
								normalIndices.push_back(normalIndex[2]);
							}
						}
						// If there aren't any uv's
						else if (normals.size() > 0)
						{
							int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0],
																				&vertexIndex[1], &normalIndex[1],
																				&vertexIndex[2], &normalIndex[2]);
							if (matches != 6)
							{
								Log("MeshLoaderOBJ", "Faces Error: File can't be read by our simple parser : ( Try exporting with other options: " + filename);

								// Break out of the loop.
								result = false;
								break;
							}
							else
							{
								// Process the faces
								vertexIndices.push_back(vertexIndex[0]);
								vertexIndices.push_back(vertexIndex[1]);
								vertexIndices.push_back(vertexIndex[2]);
								normalIndices.push_back(normalIndex[0]);
								normalIndices.push_back(normalIndex[1]);
								normalIndices.push_back(normalIndex[2]);
							}
						}
					}
					else
					{
						Log("MeshLoaderOBJ", "Faces Error: No vertices have been found: " + filename);

						// Break out of the loop.
						result = false;
						break;
					}
				}
				else
				{
					// skip unsupported features and comments for now....
					char skipBuffer[1000];
					fgets(skipBuffer, 1000, file);
				}
			}

			// If the parsing was ok
			if (result)
			{

				std::vector<Vector3> verticesExpanded;
				std::vector<Vector2> uvsExpanded;
				std::vector<Vector3> normalsExpanded;

				// Expand the data so that we can compress it again :S
				// As OBJ stores separate vertices, normals, and UV's per vertex, per _face_
				if ((uvs.size() > 0) && (normals.size() > 0))
				{
					// Expand the extracted data using the processed indices
					for (unsigned int i = 0; i < vertexIndices.size(); i++)
					{
						verticesExpanded.push_back(vertices[vertexIndices[i] - 1]);
						uvsExpanded.push_back(uvs[uvIndices[i] - 1]);
						normalsExpanded.push_back(normals[normalIndices[i] - 1]);
					}
				}
				else if (normals.size() > 0)
				{
					// Expand the extracted data using the processed indices
					for (unsigned int i = 0; i < vertexIndices.size(); i++)
					{
						verticesExpanded.push_back(vertices[vertexIndices[i] - 1]);
						normalsExpanded.push_back(normals[normalIndices[i] - 1]);
					}
				}

				// Compress data

				// Get the receiving vectors ready
				vertices.clear();
				normals.clear();
				uvs.clear();
				vertexIndices.clear();

				// Process the data into an indexed structure that VertexData can work with
				IndexVBO(verticesExpanded, uvsExpanded, normalsExpanded, vertexIndices, vertices, uvs, normals);


				// Assign the vertex data to the VertexData object
				if (vertices.size() > 0)
				{
					vertexData->SetVertices(vertices)
							  ->SetIndices(vertexIndices);

					if (normals.size() > 0)
					{
						vertexData->SetNormals(normals);
					}

					if (uvs.size() > 0)
					{
						vertexData->SetTexCoords(uvs);
					}
				}  		  
			}
		}

		return result;
	}

	bool MeshLoaderOBJ::GetSimilarVertexIndex_fast(
		PackedVertex & packed,
		std::map<PackedVertex, unsigned short> & VertexToOutIndex,
		unsigned short & result
		)
	{
		std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
		if (it == VertexToOutIndex.end())
		{
			return false;
		}
		else
		{
			result = it->second;
			return true;
		}
	}

	void MeshLoaderOBJ::IndexVBO(
		std::vector<Vector3> & in_vertices,
		std::vector<Vector2> & in_uvs,
		std::vector<Vector3> & in_normals,

		std::vector<unsigned short> & out_indices,
		std::vector<Vector3> & out_vertices,
		std::vector<Vector2> & out_uvs,
		std::vector<Vector3> & out_normals
		)
	{
		std::map<PackedVertex, unsigned short> VertexToOutIndex;

		// Making this a little more flexible.
		bool hasNormals = (in_normals.size() > 0);
		bool hasUVs = (in_uvs.size() > 0);

		// For each input vertex
		for (unsigned int i = 0; i < in_vertices.size(); i++)
		{
			PackedVertex packed;
			packed.position = in_vertices[i];

			if (hasNormals)
				packed.normal = in_normals[i];

			if (hasUVs)
				packed.uv = in_uvs[i];

			// Try to find a similar vertex in out_XXXX
			unsigned short index;

			bool found = GetSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

			if (found)
			{ 
				// A similar vertex is already in the VBO, use it instead !
				out_indices.push_back(index);
			}
			else
			{ 
				// If not, it needs to be added in the output data.
				out_vertices.push_back(in_vertices[i]);

				if (hasNormals)
					out_normals.push_back(in_normals[i]);

				if (hasUVs)
					out_uvs.push_back(in_uvs[i]);
				
				unsigned short newindex = (unsigned short)out_vertices.size() - 1;
				out_indices.push_back(newindex);
				VertexToOutIndex[packed] = newindex;
			}
		}
	}
}