#pragma once
#include "render/material/Material.h"
#include "render/material/Shader.h"
#include "resource/ResourceManager.h"

namespace epsilon
{
	class MaterialManager
	{
	private:
		MaterialManager();

	public:
		typedef std::map<std::string, Material::Ptr>MaterialList;
		typedef std::map<std::string, Shader::Ptr>	ShaderList;

		static MaterialManager & GetInstance()
		{
			static MaterialManager instance;
			return instance;
		}
		~MaterialManager();

		void Setup();

		Material::Ptr GetMaterialByName(std::string name);
		Shader::Ptr	  GetShaderByName(std::string name);

	private:
		MaterialList materials;
		ShaderList   shaders;

	};
}