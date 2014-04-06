
#pragma once

#include "EpsilonCore.h"

#include "render/Colour.h"
#include "render/material/Shader.h"
#include "render/material/ShaderUniform.h"
#include "render/RenderState.h"

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

		bool Enable(RenderStateStack::Ptr stateStack);
		void Disable();

		bool HasRefreshed() { return hasRefreshed; }

		void OnFrameStart();

	private:
		std::string name;
		Shader::Ptr shader;
        bool        shaderReady;
		bool		hasRefreshed;
		int			shaderCompileVersion;

		ShaderUniform::Ptr ambientUniform;
		ShaderUniform::Ptr diffuseUniform;
		ShaderUniform::Ptr specularUniform;
		ShaderUniform::Ptr reflectanceUniform;

		struct LightUniforms
		{
			ShaderUniform::Ptr position;
			ShaderUniform::Ptr direction;
			ShaderUniform::Ptr diffuse;
			ShaderUniform::Ptr attenuation;
		};

		const int MAX_LIGHTS = 4;

		std::vector<LightUniforms> lightUniforms;
	};

}