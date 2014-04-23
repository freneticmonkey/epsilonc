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

#include "render/material/ShaderManager.h"
#include "render/material/ShaderUniform.h"

namespace epsilon
{
	class Light;

//#define MAX_LIGHTS 10
    
//    struct LightInfo
//    {
//        Vector3 position;
//        Vector3 direction;
//        Vector4 diffuse;
//        Vector4 attentuation;
//        float   padding[2];
//    };
//    
//    struct LightBuffer
//    {
//        int         numLights;
//        LightInfo   lights[MAX_LIGHTS];
//    };
    
    struct LightUniforms
    {
        ShaderUniform::Ptr position;
        ShaderUniform::Ptr direction;
        ShaderUniform::Ptr diffuse;
        ShaderUniform::Ptr attenuation;
        ShaderUniform::Ptr strength;
        
    };
    
	typedef std::vector< std::shared_ptr<Light> > LightList;

	class Light : public NodeComponent
	{
	protected:
		struct private_struct {};
        
	public:
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
        Vector4 attenuation;

        float   angle;
        float   strength;
        
		void	Update();
        
	private:
		void	Setup();
        
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
	};
}