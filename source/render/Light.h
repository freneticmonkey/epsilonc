#pragma once
//
//  Light.h
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "EpsilonCore.h"

#include "scene/NodeComponent.h"
#include "scene/Transform.h"
#include "math/Matrix.h"
#include "render/Colour.h"

#include "render/Renderer.h"
#include "render/material/MaterialManager.h"

#include "render/material/ShaderManager.h"
#include "render/material/ShaderUniform.h"

namespace epsilon
{
	class Light;
    
	typedef std::vector< std::shared_ptr<Light> > LightList;

	class Light : public NodeComponent
	{
	protected:
		struct private_struct {};
        
	public:

		enum Type
		{
			POINT = 0,
			SPOT,
			DIRECTIONAL,
			SUN
		};

		enum ShadowType
		{
			NONE = 0,
			HARD,
			SOFT
		};

        static const int MAX_LIGHTS = 4;

		typedef std::shared_ptr<Light> Ptr;
        
		static Light::Ptr Create(int id);
		static Light::Ptr Create(int id, std::string name);
        
		explicit Light(const private_struct &, int id);
		Light(const private_struct &, int id, std::string name);
		~Light(void) {}

		int GetId() { return id; }
        
        // Listen to parent changes
        void OnSetParent();

		Vector3 GetPosition();
		Vector3 GetDirection();

		Colour  ambient;
        Colour  diffuse;
		Colour  specular;
        Vector3 attenuation;

        float   strength;
		float	spotCutoff;
		float	spotExponent;

		int		type;
		int		shadowType;
        
		void	PreRender(Renderers renderItems);

		void	PostRender();

	private:
		void	Setup();
		void	SetupShadows();
        
		int		id;
        Transform::Ptr transform;

		// Uniforms
		ShaderUniform::Ptr positionUnf;
		ShaderUniform::Ptr directionUnf;

		ShaderUniform::Ptr ambientUnf;
		ShaderUniform::Ptr diffuseUnf;
		ShaderUniform::Ptr specularUnf;
		
		ShaderUniform::Ptr attenuationUnf;
		
		ShaderUniform::Ptr strengthUnf;
		ShaderUniform::Ptr spotCutoffUnf;
		ShaderUniform::Ptr spotExponentUnf;

		ShaderUniform::Ptr typeUnf;

		// Shadow members
		ShaderUniform::Ptr	shadowMatrixUnf;
		ShaderUniform::Ptr	shadowDepthBiasUnf;
			
		// Global Matrices
		ShaderUniform::Ptr	viewMatrixUnf;
		ShaderUniform::Ptr	projMatrixUnf;

		ShaderUniform::Ptr  shadowDepthTexture;

		Material::Ptr		shadowMaterial;

		bool				shadowsSetup;
		bool				shadowsFailed;

		MaterialManager *	materialManager;

		GLuint FramebufferName;
		GLuint depthTexture;
	};
}