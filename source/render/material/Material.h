
#pragma once

#include "EpsilonCore.h"
#include "render/RenderUtilities.h"

#include "render/Colour.h"
#include "render/material/Shader.h"
#include "render/material/ShaderUniform.h"
#include "render/texture/Texture.h"

namespace epsilon
{
	class Material
	{
	private:
		struct private_struct {} ;

	public:
		typedef std::shared_ptr<Material> Ptr;

		static Material::Ptr Create();
		static Material::Ptr Create(std::string matName);
		
		explicit Material(const private_struct &);
		explicit Material(const private_struct &, std::string matName);
		~Material(void);

		std::string GetName() { return name; }

		Colour ambient;
		Colour diffuse;
		Colour specular;

		float reflectance;

		void SetShader(Shader::Ptr newShader);
		Shader::Ptr GetShader();

		void SetupShader();

		bool Enable(const Matrix4 &modelMatrix);
		void Disable();

		bool HasRefreshed() { return hasRefreshed; }

		void OnFrameStart();

		void AddTexture(Texture::Ptr newTexture);
		void AddTextureByName(std::string textureName);
		void AddTextureByPath(std::string texturePath);

		Textures GetTextures() { return textures; }

	private:
		std::string name;
		Shader::Ptr shader;
        bool        shaderReady;
		bool		hasRefreshed;
		int			shaderCompileVersion;

		// Textures
		Textures			textures;
		ShaderUniforms		textureUniforms;

		// Material properties
		ShaderUniform::Ptr ambientUniform;
		ShaderUniform::Ptr diffuseUniform;
		ShaderUniform::Ptr specularUniform;
		ShaderUniform::Ptr reflectanceUniform;
        
        ShaderUniform::Ptr modelUniform;
		
	};

}