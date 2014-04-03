//
//  Light.cpp
//  Epsilon
//
//  Created by Scott Porter on 2/04/2014.
//  Copyright (c) 2014 Scott Porter. All rights reserved.
//

#include "render/Light.h"

namespace epsilon
{
	Light::Ptr Light::Create()
	{
		Light::Ptr newLight = std::make_shared<Light>(private_struct());
		return newLight;
	}
    
	Light::Ptr Light::Create(std::string name)
	{
		Light::Ptr newLight = std::make_shared<Light>(private_struct(), name);
		return newLight;
	}
    
	Light::Light(const private_struct &) : NodeComponent("Light")
	{
	}
    
	Light::Light(const private_struct &, std::string name) : NodeComponent(name, "Light")
	{
	}
    
	void Light::Setup()
	{
		
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
    
}
