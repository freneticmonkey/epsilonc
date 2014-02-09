#pragma once

#include "math/Vector.h"
#include "math/Rotation.h"
#include "math/Matrix.h"

class Matrix4;

class Quaternion
{
public:
    float x;
    float y;
    float z;
    float w;
    
    // Constructors
    Quaternion();
    Quaternion(const Quaternion& quat);
    Quaternion(float ix, float iy, float iz, float iw);

    // From Axis Angle
    Quaternion(Vector3 axis, float angle);
    //Quaternion(AxisAngle rotation) : Quaternion(rotation.axis, rotation.angle) {}

    // From Euler
    Quaternion(float heading, float attitude, float bank);
    //Quaternion(Euler rotation) : Quaternion( rotation.heading, rotation.attitude, rotation.bank) {}

    // From Matrix4
    Quaternion(Matrix4 rotation);

    // Comparison
    inline bool operator==(const Quaternion& other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

    inline bool operator!=(const Quaternion& other) const
	{
		return x != other.x && y != other.y && z != other.z && w != other.w;
	}

    // Arithmetic operations
    /*
    inline friend Quaternion operator* (Quaternion other);
    inline friend Quaternion operator* (Quaternion other, const Quaternion& op1);
    
    inline Vector3 operator* (const Vector3 v) const;

    inline Quaternion& operator*=( const Quaternion& other );
    */
    Quaternion operator* (const Quaternion& other) const
    {
        float ax, ay, az, aw;
        float bx, by, bz, bw;
        ax = x;
        ay = y;
        az = z;
        aw = w;
        bx = other.x;
        by = other.y;
        bz = other.z;
        bw = other.w;
		return Quaternion(
			aw * bx + ax * bw + ay * bz - az * by, //x
			aw * by + ay * bw + az * bx - ax * bz, //y
			aw * bz + az * bw + ax * by - ay * bx, //z

			aw * bw - ax * bx - ay * by - az * bz); //w
		/*
			return Quaternion( ax * bw + ay * bz - az * by + aw * bx,   //x
                          -ax * bz + ay * bw + az * bx + aw * by,   //y
                          ax * by - ay * bx + az * bw + aw + bz,   //z
                          -ax * bx - ay * by - az * bz + aw * bw ); //w
		*/
        
        
    }
    /*
    inline Quaternion friend operator* (const Quaternion other, Quaternion op1)
    {
        float ax, ay, az, aw;
        float bx, by, bz, bw;
        ax = op1.x;
        ay = op1.y;
        az = op1.z;
        aw = op1.w;
        bx = other.x;
        by = other.y;
        bz = other.z;
        bw = other.w;
        return Quaternion( ax * bw + ay * bz - az * by + aw * bx,   //x
                          -ax * bz + ay * bw + az * bx + aw * by,   //y
                          ax * by - ay * bx + az * bw + aw + bz,   //z
                          -ax * bx - ay * by - az * bz + aw * bw ); //w
    }
     */
    
    
    Vector3 operator* (const Vector3 v) const
    {
        Vector3 uv, uuv;
        Vector3 qvec(x, y, z);
        uv = qvec.Cross(v);
        uuv = qvec.Cross(uv);
        uv *= ( 2.f * w );
        uuv *= 2.f;
        
        return v + uv + uuv;
    }
    
    Quaternion& operator*= (const Quaternion& other)
    {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }
    
    inline float Length()
	{
		return std::sqrtf( x * x + y * y + z * z + w * w);
	}

	inline float LengthSquared()
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float Normalise()
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

	inline Quaternion Normalised()
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

	inline void Conjugate()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	inline Quaternion Conjugated()
	{
		return Quaternion(-x, -y, -z, w);
	}
    
    Quaternion Inverse() const;

    // Rotation
    Quaternion RotateAxis(Vector3 axis, float angle);
    Quaternion RotateAxis(AxisAngle orientation);

    Quaternion RotateEuler(float heading, float attitude, float bank);
    Quaternion RotateEuler(Euler orientation);

    void RotateMatrix(Matrix4 orientation);
    
    
    Vector3 Rotate(const Vector3& vec) const;

    AxisAngle GetAngleAxis();
    Euler GetEuler();
    Matrix4 GetMatrix() const;

    Quaternion Interpolate(Quaternion start, Quaternion end, float time);
    
    static const Quaternion IDENTITY;
    static const Quaternion ZERO;
};