#pragma once
//
//  Perlin.h
//  Epsilon
//
//  Created by Scott Porter on 26/05/2014.
//

// This is a wrapper function for the stb_perlin.h perlin functions

#define STB_PERLIN_IMPLEMENTATION

#include "tools/procedural/stb_perlin.h"
#include "math/Vector.h"

namespace epsilon
{

    class Perlin
    {
    public:
        
        static float Noise3D(float x, float y, float z, int x_wrap=0, int y_wrap=0, int z_wrap=0)
        {
            return stb_perlin_noise3(x, y, z, x_wrap, y_wrap, z_wrap);
        }
        
        static float Noise1D(float x, int wrap_x=0)
        {
            return stb_perlin_noise3(x, 0, 0, wrap_x, 0, 0);
        }
        
        static float Noise2D(Vector2 p, int wrap_x=0, int wrap_y=0)
        {
            return stb_perlin_noise3(p.x, p.y, 0, wrap_x, wrap_y, 0);
        }
        
        static float Noise2Dv(Vector2 p, Vector2 wrap)
        {
            return stb_perlin_noise3(p.x, p.y, 0, wrap.x, wrap.y, 0);
        }
        
        static float Noise3Dvi(Vector3 p, int wrap_x=0, int wrap_y=0, int wrap_z=0)
        {
            return stb_perlin_noise3(p.x, p.y, p.z, wrap_x, wrap_y, wrap_z);
        }
        
        static float Noise3Dv(Vector3 p, Vector3 wrap = Vector3::ZERO)
        {
            return stb_perlin_noise3(p.x, p.y, p.z, wrap.x, wrap.y, wrap.z);
        }
        
    };

}