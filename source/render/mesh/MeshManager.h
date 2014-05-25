#pragma once
//
//  MeshManager.h
//  Epsilon
//
//  Created by Scott Porter on 24/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/mesh/Mesh.h"
#include "render/mesh/MeshLoaderInterface.h"
#include "resource/ResourceOwner.h"

namespace epsilon
{
	class MeshManager :
		public ResourceOwner
	{
	private:
		MeshManager();

	public:
		typedef std::map<std::string, Mesh::Ptr>        MeshMap;
		typedef std::vector<MeshLoaderInterface::Ptr>	MeshLoaders;
		static MeshManager & GetInstance()
		{
			static MeshManager instance;
			return instance;
		}
		~MeshManager();

		void        Setup();
		void		LoadMeshs();

		Mesh::Ptr GetMeshByName(std::string name);
		Mesh::Ptr GetMeshByPath(std::string path);
		Mesh::Ptr GetDefault() { return defaultMesh; }

		void RefreshResources(ResourceIdVector resources);

		// This will be called from the RenderManager
		void ProcessMeshs();

	private:
		MeshMap		meshs;
		Mesh::Ptr	defaultMesh;

		MeshLoaders	meshLoaders;
	};

	void Setup();
}
