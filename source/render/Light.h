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

namespace epsilon
{
	class Light;

	typedef std::vector< std::shared_ptr<Light> > LightList;

	class Light : public NodeComponent
	{
	protected:
		struct private_struct {};
        
	public:
		typedef std::shared_ptr<Light> Ptr;
        
		static Light::Ptr Create();
		static Light::Ptr Create(std::string name);
        
		explicit Light(const private_struct &);
		Light(const private_struct &, std::string name);
		~Light(void) {}
        
        // Listen to parent changes
        void OnSetParent();
        
        Colour  diffuse;
        Vector3 attenuation;
        float   angle;
        
        Vector3 GetPosition();
        Vector3 GetDirection();
        
	private:
		void Setup();
        
        Transform::Ptr transform;
	};
}