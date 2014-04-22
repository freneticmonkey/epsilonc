#pragma once
#include "render/material/Material.h"
#include "render/material/Shader.h"

#include "render/material/ShaderManager.h"

namespace epsilon
{
	class MaterialManager
	{
	private:
		MaterialManager();

	public:
		typedef std::map<std::string, Material::Ptr>MaterialList;

		static MaterialManager & GetInstance()
		{
			static MaterialManager instance;
			return instance;
		}
		~MaterialManager();

		void Setup();

		Material::Ptr CreateMaterial();
		Material::Ptr CreateMaterial(std::string name);

		Material::Ptr GetMaterialByName(std::string name);

		void OnFrameStart();

	private:
		MaterialList    materials;
        Shader::Ptr     defaultShader;

	};
}