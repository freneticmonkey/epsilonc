//
//  Light.cpp
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/Light.h"
#include <boost/format.hpp>

namespace epsilon
{
	using namespace boost;

	Light::Ptr Light::Create(int newID)
	{
		Light::Ptr newLight = std::make_shared<Light>(private_struct(), newID);
		return newLight;
	}
    
	Light::Ptr Light::Create(int newID, std::string name)
	{
		Light::Ptr newLight = std::make_shared<Light>(private_struct(), newID, name);
		return newLight;
	}
    
	Light::Light(const private_struct &, int newID) : NodeComponent("Light"), angle(0), strength(10), id(newID)
	{
		Setup();
	}
    
	Light::Light(const private_struct &, int newID, std::string name) : NodeComponent(name, "Light"), angle(0), strength(10), id(newID)
	{
		Setup();
	}
    
	void Light::Setup()
	{
		UniformBuffer::Ptr lights = ShaderManager::GetInstance().GetUniformBuffer("Lights");

		// Grab this light's uniforms from the Uniform Buffer.
		positionUnf		= lights->GetUniform(str(format("lights[%d].position") % id));
		directionUnf	= lights->GetUniform(str(format("lights[%d].direction") % id));

		ambientUnf		= lights->GetUniform(str(format("lights[%d].ambient") % id));
		diffuseUnf		= lights->GetUniform(str(format("lights[%d].diffuse") % id));
		specularUnf		= lights->GetUniform(str(format("lights[%d].specular") % id));
		
		attenuationUnf	= lights->GetUniform(str(format("lights[%d].attenuation") % id));
		
		strengthUnf		= lights->GetUniform(str(format("lights[%d].strength") % id));
	}
    
    void Light::OnSetParent()
    {
        transform = componentParent->GetComponent<Transform>();
    }
    
    Vector3 Light::GetPosition()
    {
        return transform->GetPosition();
    }
    
    Vector3 Light::GetDirection()
    {
        return transform->Forward();
    }
    
	void Light::Update()
	{
		// Push Data to Uniform Buffer if uniforms were successfully found.

		if (positionUnf)
		{
			positionUnf->SetVector3(GetPosition());
		}

		if (directionUnf)
		{
			directionUnf->SetVector3(GetDirection());
		}

		if (ambientUnf)
		{
			ambientUnf->SetVector4(ambient.ToVector4());
		}

		if (diffuseUnf)
		{
			diffuseUnf->SetVector4(diffuse.ToVector4());
		}

		if (specularUnf)
		{
			specularUnf->SetVector4(specular.ToVector4());
		}

		if (attenuationUnf)
		{
			attenuationUnf->SetVector4(attenuation);
		}

		if (strengthUnf)
		{
			strengthUnf->SetFloat(strength);
		}
	}
}
