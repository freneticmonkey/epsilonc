//
//  Quaternion.cpp
//  Epsilon
//
//  Created by Scott Porter on 30/12/12.
//  Copyright (c) 2012 Scott Porter. All rights reserved.
//

#include <cmath>

#include "math/Quaternion.h"
#include "math/Defines.h"

const Quaternion Quaternion::IDENTITY(0.f, 0.f, 0.f, 1.f);
const Quaternion Quaternion::ZERO(0.f, 0.f, 0.f, 0.f);

Quaternion::Quaternion()
{
    x = 0.f;
    y = 0.f;
    z = 0.f;
    w = 1.f;
}

Quaternion::Quaternion(const Quaternion& quat)
{
    x = quat.x;
    y = quat.y;
    z = quat.z;
    w = quat.w;
}

Quaternion::Quaternion(float ix, float iy, float iz, float iw)
{
    x = ix;
    y = iy;
    z = iz;
    w = iw;
}

// From Axis Angle
Quaternion::Quaternion(Vector3 axis, float angle)
{
    float s = std::sin(angle / 2.f);
    axis.Normalise();
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
    w = std::cos(angle / 2.f);
}

// From Euler
Quaternion::Quaternion(float heading, float attitude, float bank)
{
    float c1, s1, c2, s2, c3, s3;
    c1 = std::cos(heading / 2.f);
    s1 = std::sin(heading / 2.f);
    c2 = std::cos(attitude / 2.f);
    s2 = std::sin(attitude / 2.f);
    c3 = std::cos(bank / 2.f);
    s3 = std::sin(bank / 2.f);
    
    x = s1 * s2 * c3 + c1 * c2 * c3;
    y = s1 * c2 * c3 + c1 * s2 * s3;
    z = c1 * s2 * c3 - s1 * c2 * s3;
    w = c1 * c2 * c3 - s1 * c2 * s3;
}

// From Matrix4
Quaternion::Quaternion(Matrix4 rotation)
{
    
}


// Comparison
bool Quaternion::operator==(const Quaternion& other) const
{
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Quaternion::operator!=(const Quaternion& other) const
{
    return x != other.x && y != other.y && z != other.z && w != other.w;
}

// Arithmetic operations


float Quaternion::Length()
{
    return std::sqrtf( x * x + y * y + z * z + w * w);
}

float Quaternion::LengthSquared()
{
    return x * x + y * y + z * z + w * w;
}

float Quaternion::Normalise()
{
    float length = Length();
    
	if ( length > 0 )
	{
		float invLength = 1 / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;
        w *= invLength;
	}
	else
	{
		length = 0.f;
		x = 0.f;
		y = 0.f;
		z = 0.f;
        w = 0.f;
	}
	return length;
}

Quaternion Quaternion::Normalised()
{
    float length = Length();
    
	float nx = x;
	float ny = y;
	float nz = z;
    float nw = w;
    
	if ( length > 0 )
	{
		float invLength = 1 / length;
		nx *= invLength;
		ny *= invLength;
		nz *= invLength;
        nw *= invLength;
	}
	else
	{
		length = 0.f;
		nx = 0.f;
		ny = 0.f;
		nz = 0.f;
        nw = 0.f;
	}
	return Quaternion(nx, ny, nz, nw);
}

inline void Quaternion::Conjugate()
{
    x = -x;
    y = -y;
    z = -z;
}

inline Quaternion Quaternion::Conjugated()
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::Inverse() const
{
    float norm = x * x + y * y + z * z + w * w;
    if ( norm > 0.f )
    {
        float invNorm = 1.f / norm;
        return Quaternion(-x * invNorm, -y * invNorm, -z * invNorm, w * invNorm);
    }
    else
    {
        return ZERO;
    }
}

// Rotation
Quaternion Quaternion::RotateAxis(Vector3 axis, float angle)
{
    return *this *= Quaternion(axis, angle);
}

Quaternion Quaternion::RotateAxis(AxisAngle orientation)
{
    return RotateAxis(orientation.axis, orientation.angle );
}


Quaternion Quaternion::RotateEuler(float heading, float attitude, float bank)
{
    return *this *= Quaternion(heading, attitude, bank);
}

Quaternion Quaternion::RotateEuler(Euler orientation)
{
    return RotateEuler(orientation.heading, orientation.attitude, orientation.bank);
}

void Quaternion::RotateMatrix(Matrix4 orientation)
{
    
}

Vector3 Quaternion::Rotate(const Vector3& vec) const
{
    return GetMatrix() * vec;
}

AxisAngle Quaternion::GetAngleAxis()
{
    float angle, s;
    Quaternion quat;
    
    if ( w > 1 )
    {
        quat = Normalised();
    }
    else
    {
        quat = *this;
    }
    
    angle = 2.f * std::acos(w);
    s = std::sqrt(1 - w * w);
    
    if ( s < 0.001f )
    {
        return AxisAngle(Vector3(1.f, 0.f, 0.f), angle);
    }
    else
    {
        return AxisAngle(Vector3(x / s, y / s, z / s), angle);
    }
}

Euler Quaternion::GetEuler()
{
    float t, heading, attitude, bank;
    float sqx, sqy, sqz;
    
    t = x * y + z * w;
    
    if ( t > 0.4999f )
    {
        heading = 2.f * std::atan2(x, w);
        attitude = PI / 2.f;
        bank = 0.f;
    }
    else if ( t < -0.4999f )
    {
        heading = -2.f * std::atan2(x, w);
        attitude = PI / 2.f;
        bank = 0.f;
    }
    else
    {
        sqx = x * x;
        sqy = y * y;
        sqz = z * z;
        heading = std::atan2(2.f * y * w - 2.f * x * z,
                             1.f - 2.f * sqy - 2.f * sqz);
        attitude = std::asin(2.f * t);
        bank = std::atan2(2.f * x * w - 2.f * y * z,
                          1.f - 2.f * sqx - 2.f * sqz);
    }
    return Euler(heading, attitude, bank);
}

Matrix4 Quaternion::GetMatrix() const
{
    return Matrix4::CreateRotateQuaternion(*this);
}


Quaternion Quaternion::Interpolate(Quaternion start, Quaternion end, float time)
{
    return Quaternion();
}