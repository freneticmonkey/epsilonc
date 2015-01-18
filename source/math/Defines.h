//
//  Defines.h
//  Epsilon
//
//  Created by Scott Porter on 30/12/12.
//

#ifndef Epsilon_Defines_h
#define Epsilon_Defines_h

#define PI 3.14159265359f
#define TWOPI 6.28318530718f

namespace epsilon
{
	class Math
	{
	public:
		inline static float DegreesToRadians(float degree)
		{
			return degree / 57.2957795f;
		}
	};
}


#endif
