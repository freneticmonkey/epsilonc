
#pragma once

#include "EpsilonCore.h"

#include "render/Colour.h"
#include "render/Shader.h"
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
		
		explicit Material(const private_struct &);
		~Material(void);

		Colour ambient;
		Colour diffuse;
		Colour specular;

		float reflectance;

		void SetShader(Shader::Ptr newShader);
		Shader::Ptr GetShader();

		void SetupShader();

		void Enable(RenderStateStack::Ptr stateStack);
		void Disable();

	private:
		Shader::Ptr shader;

		GLuint ambientId;
		GLuint diffuseId;
		GLuint specId;
		GLuint reflectId;
	};

}