//
//  MeshManager.cpp
//  Epsilon
//
//  Created by Scott Porter on 24/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/mesh/MeshManager.h"
#include "render/mesh/MeshLoaderOBJ.h"

#include "resource/ResourceManager.h"

#include <boost/format.hpp>

namespace epsilon
{
	using namespace boost;

	MeshManager::MeshManager()
	{

	}

	MeshManager::~MeshManager()
	{

	}

	void MeshManager::Setup()
	{
		// Register Mesh Loaders
		meshLoaders.push_back(MeshLoaderOBJ::Create());

		// build a regular expression of supported Mesh formats
		std::string meshRegex;
		std::vector<std::string> exts;

		std::for_each(meshLoaders.begin(), meshLoaders.end(), [&](MeshLoaderInterface::Ptr MeshLoader){
			FileExtensions ext = MeshLoader->GetExtensions();
			exts.insert(exts.end(), ext.begin(), ext.end());
		});

		// Pre-allocate the regex assuming a maximum extension length of 5
		meshRegex.reserve(exts.size() * 5);

		for (std::vector<std::string>::iterator e = exts.begin(); e != exts.end(); e++)
		{
			meshRegex += (*e);

			// If not the last extension, insert a pipe
			if (e < (exts.end() - 1))
			{
				meshRegex += "|";
			}
		}

		meshRegex = str(format(".*(%s)$") % meshRegex);

		// Search the ResourceManager for all files with supported Mesh extensions
		ResourceList results = ResourceManager::GetInstance().FindResources(meshRegex);

		// For each of the results
		std::for_each(results.begin(), results.end(), [&](Resource::Ptr resource){
			// Create a new Mesh
			Mesh::Ptr newMesh = Mesh::CreateFromFile(resource->GetFilepath().GetString());

			// Add it to the managed Meshs
			meshs[newMesh->GetName()] = newMesh;
			
			// Register it for change events
			RegisterResource(newMesh);

			// Assign it a Mesh loader
			std::find_if(meshLoaders.begin(), meshLoaders.end(), [&](MeshLoaderInterface::Ptr texLoader){
				return newMesh->SetMeshLoader(texLoader) == true;
			});

			//Log("MeshManager", str(format("Registered Mesh: %s") % newMesh->GetName()));
		});

		// For now we'll just immediately load Meshs
		// TODO: Add update function which loads necessary Meshs
		LoadMeshs();
	}

	void MeshManager::LoadMeshs()
	{
		// Breaking this out into a separate function as it might be a little heavy.
		std::for_each(meshs.begin(), meshs.end(), [](std::pair < std::string, Mesh::Ptr> mesh){
			mesh.second->LoadMesh();
		});
	}

	Mesh::Ptr MeshManager::GetMeshByName(std::string name)
	{
		Mesh::Ptr foundMesh;
		if (meshs.find(name) != meshs.end())
		{
			foundMesh = meshs[name];
		}
		return foundMesh;
	}

	void MeshManager::RefreshResources(ResourceIdVector resources)
	{
		// Reload any changed files from disk.  This will not push them to the GPU.
		std::for_each(meshs.begin(), meshs.end(), [](std::pair < std::string, Mesh::Ptr> mesh){
			mesh.second->RefreshFromFile();
		});
	}

	void MeshManager::ProcessMeshs()
	{
		// Push any changed Mesh data to the GPU.
		std::for_each(meshs.begin(), meshs.end(), [](std::pair < std::string, Mesh::Ptr> mesh){
			mesh.second->RefreshBuffers();
		});
	}
}