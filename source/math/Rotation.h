//
//  Rotation.h
//  Epsilon
//
//  Created by Scott Porter on 31/12/12.
//  Copyright (c) 2012 Scott Porter. All rights reserved.
//

#ifndef Epsilon_Rotation_h
#define Epsilon_Rotation_h

#include <boost/format.hpp>

using namespace boost;

struct AxisAngle
{
    Vector3 axis;
    float   angle;
    
    AxisAngle(Vector3 iaxis, float iangle)
    {
        axis = iaxis;
        angle = iangle;
    }

	// To String
	inline std::string ToString()
	{
        return boost::str(format("Axis: (%s) Angle: %5.5f") % axis.ToString() % angle);
	}
};

struct Euler
{
    float heading;
    float attitude;
    float bank;
    Euler(float iheading, float iattitude, float ibank)
    {
        heading = iheading;
        attitude = iattitude;
        bank = ibank;
    }
};

#endif
