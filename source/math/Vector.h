#pragma once

#include <cmath>
#include <list>
#include "utilities/Utilities.h"

using namespace epsilon;

class Vector2
{
public:
	float x;
	float y;

	// Constructors
	Vector2(float v = 0.0f);
	Vector2(const Vector2& vec);
	Vector2(float ix, float iy);

	// To String
	inline std::string ToString() 
	{
		return Format("x: %5.5f y: %5.5f", x, y);
	}

	// Assignment
	Vector2& operator= (const Vector2& vec);

	// Array Access
	float &operator[](int i);

	// Comparison
	inline bool operator== (const Vector2& other) const
	{
		return x == other.x && y == other.y;
	}

	inline bool operator!= (const Vector2& other) const
	{
		return x != other.x  || y != other.y;
	}
	
	// Arithmetic operations.
	friend inline Vector2 operator+ (const Vector2& left, const Vector2& right)
	{
		return Vector2( left.x + right.x , 
						left.y + right.y );
	}

	friend inline Vector2 operator- (const Vector2& left, const Vector2& right)
	{
		return Vector2( left.x - right.x , 
						left.y - right.y );
	}

	Vector2 operator-() const
	{
		return Vector2( -x, -y );
	}

	// Implemented operator* functions as friends so that operators can appear
	// in either order vec2 * float , float * vec2
	inline Vector2 operator* (const Vector2& op1)//, const float scalar)
	{
		return Vector2( x * op1.x , 
						y * op1.y );
	}

	inline Vector2 operator* (const float scalar)//, const Vector2& op1)
	{
		return Vector2( x * scalar , 
						y * scalar );
	}
	
	inline Vector2 friend operator* (const float scalar, const Vector2& op1)
	{
		return Vector2( op1.x * scalar , 
						op1.y * scalar );
	}

	inline Vector2 operator/ (const float scalar) const
	{
		return Vector2( x / scalar , 
						y / scalar );
	}

	// Arithmetic updates.
    inline Vector2& operator+= (const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	inline Vector2& operator-= (const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	inline Vector2& operator*= (float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	inline Vector2& operator/= (float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	inline float Length() const
	{
		return std::sqrtf( x * x + y * y );
	}

	inline float LengthSquared() const
	{
		return x * x + y * y;
	}

	inline float Dot(const Vector2& other ) const
	{
		return x * other.x + y * other.y;
	}

	inline float Normalise()
	{
		float length = Length();

		if ( length > 0 )
		{
			float invLength = 1 / length;
			x *= invLength;
			y *= invLength;
		}
		else
		{
			length = 0.f;
			x = 0.f;
			y = 0.f;
		}
		return length;
	}

	inline Vector2 Normalised() const
	{
		float length = Length();

		float nx = x;
		float ny = y;

		if ( length > 0 )
		{
			float invLength = 1 / length;
			nx *= invLength;
			ny *= invLength;
		}
		else
		{
			length = 0.f;
			nx = 0.f;
			ny = 0.f;
		}
		return Vector2(nx, ny);
	}

	float Cross(const Vector2& other) const;

	Vector2 Reflect(const Vector2& normal) const;

	float Angle(const Vector2& other) const;

	Vector2 Project(const Vector2& other) const;

	// Class Functions
	static void ComputeExtremes(int numVectors, const Vector2* vectors,
								Vector2& vmin, Vector2& vmax);
};

class Vector3
{
public:
	typedef std::list<Vector3> List;
	float x;
	float y;
	float z;
	
	// Constructors
	Vector3(float v = 0.0f);
	Vector3(const Vector3& vec);
	Vector3(float ix, float iy, float iz);

	// To String
	inline std::string ToString() 
	{
		return Format("x: %5.5f y: %5.5f z: %5.5f", x, y, z);
	}

	// Assignment
	inline Vector3& operator= (const Vector3& vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

	// Array Access
	float &operator[](int i);

    inline bool operator== (const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
    
    inline bool operator!= (const Vector3& other) const
    {
        return x != other.x  || y != other.y || z != other.z;
    }
    
    friend inline Vector3 operator+ (const Vector3& left, const Vector3& right)
    {
        return Vector3( left.x + right.x ,
                        left.y + right.y ,
                        left.z + right.z);
    }
    
    friend inline Vector3 operator- (const Vector3& left, const Vector3& right)
    {
        return Vector3( left.x - right.x ,
                        left.y - right.y ,
                        left.z - right.z);
    }

	Vector3 operator-(void) const
    {
        return Vector3( -x, -y, -z );
    }
    
    // Implemented operator* functions as friends so that operators can appear
    // in either order vec3 * float , float * vec3
    inline Vector3 operator* (const Vector3& op1)//, const float scalar)
    {
        return Vector3 ( x * op1.x,
                         y * op1.y,
                         z * op1.z );
    }
    
    inline Vector3 operator* (const float scalar)
    {
        return Vector3 ( x * scalar,
                         y * scalar,
                         z * scalar);
    }
/*
	inline Vector3 friend operator* (Vector3 vec, Vector3 const &op1)
    {
        return Vector3( op1.x * vec.x, op1.y * vec.y, op1.z * vec.z);
    }
*/    
    inline Vector3 friend operator* (float scalar, Vector3 const &op1)
    {
        return Vector3( op1.x * scalar, op1.y * scalar, op1.z * scalar);
    }

	inline Vector3 operator/ (const float scalar) const
    {
        return Vector3( x / scalar , 
                        y / scalar ,
                        z / scalar );
    }
    
	// Arithmetic updates.
    inline Vector3& operator+= (const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    inline Vector3& operator-= (const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    inline Vector3& operator*= (float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    inline Vector3& operator/= (float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

	inline float Length() const
	{
		return std::sqrtf( x * x + y * y + z * z);
	}

	inline float LengthSquared() const
	{
		return x * x + y * y + z * z;
	}

	inline float Dot(const Vector3& other ) const
	{
		return x * other.x + y * other.y + z * other.z;
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
		}
		else
		{
			length = 0.f;
			x = 0.f;
			y = 0.f;
			z = 0.f;
		}
		return length;
	}

	inline Vector3 Normalised() const
	{
		float length = Length();

		float nx = x;
		float ny = y;
		float nz = z;

		if ( length > 0 )
		{
			float invLength = 1 / length;
			nx *= invLength;
			ny *= invLength;
			nz *= invLength;
		}
		else
		{
			length = 0.f;
			nx = 0.f;
			ny = 0.f;
			nz = 0.f;
		}
		return Vector3(nx, ny, nz);
	}

	Vector3 Cross(const Vector3& other) const;
	Vector3 UnitCross(const Vector3& other) const;

	Vector3 Reflect(const Vector3& normal) const;

	Vector3 RotateAround(const Vector3& axis, float theta);

	
	inline float Distance(const Vector3& other) const
	{
		return (other - *this).Length();
	}

	inline float Angle(const Vector3& other) const
	{
		return acosf( Dot(other) / Length() * other.Length() );
	}

	Vector3 Project(const Vector3& other) const;


	// Class Functions
	static void ComputeExtremes(const Vector3::List vectors,
								Vector3& vmin, Vector3& vmax);

	// Static Common Vectors
	static const Vector3 UP;
	static const Vector3 RIGHT;
	static const Vector3 FORWARD;
	static const Vector3 DOWN;
	static const Vector3 LEFT;
	static const Vector3 BACKWARD;
    
    static const Vector3 ZERO;
    static const Vector3 ONE;
    static const Vector3 IDENTITY;
    
    static const Vector3 UNIT_X;
    static const Vector3 UNIT_Y;
    static const Vector3 UNIT_Z;
};

class Vector4
{
public:
	float x;
	float y;
	float z;
    float w;
	
	// Constructors
	Vector4(float v = 0.0f);
	Vector4(const Vector4& vec);
	Vector4(float ix, float iy, float iz, float iw);

	// To String
	inline std::string ToString() 
	{
		return Format("x: %5.5f y: %5.5f z: %5.5f w: %5.5f", x, y, z, w);
	}
    
	// Assignment
	Vector4& operator= (const Vector4& vec);
    
	// Array Access
	float &operator[](int i);
    
	// Comparison
	inline bool operator== (const Vector4& other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	inline bool operator!= (const Vector4& other) const
	{
		return x != other.x  || y != other.y || z != other.z || w != other.w;
	}
	
	// Arithmetic operations.
	friend inline Vector4 operator+ (const Vector4& left, const Vector4& right)
	{
		return Vector4(left.x + right.x ,
					   left.y + right.y ,
					   left.z + right.z ,
					   left.w + right.w);
	}

	friend inline Vector4 operator- (const Vector4& left, const Vector4& right)
	{
		return Vector4(left.x - right.x ,
					   left.y - right.y ,
					   left.z - right.z ,
					   left.w - right.w);
	}
    
	// Implemented operator* functions as friends so that operators can appear
	// in either order vec4 * float , float * vec4
	inline Vector4 operator* (const Vector4& op1)
	{
		return Vector4(x * op1.x,
					   y * op1.y ,
					   z * op1.z ,
					   w * op1.w );
	}

	inline Vector4 operator* (const float scalar)
	{
		return Vector4(x * scalar ,
					   y * scalar ,
					   z * scalar ,
					   w * scalar);
	}

	inline friend Vector4 operator* (const float scalar, const Vector4& op1)
	{
		return Vector4(op1.x * scalar ,
					   op1.y * scalar ,
					   op1.z * scalar ,
					   op1.w * scalar);
	}
	
	inline Vector4 operator/ (const float scalar) const
	{
		return Vector4(x / scalar ,
					   y / scalar ,
					   z / scalar ,
					   w / scalar);
	}
	inline Vector4 operator- () const
	{
		return Vector4(-x, -y, -z, -w);
	}
    
	// Arithmetic updates.
    inline Vector4& operator+= (const Vector4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	inline Vector4& operator-= (const Vector4& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	inline Vector4& operator*= (float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	inline Vector4& operator/= (float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}
    
	inline float Length() const
	{
		return std::sqrtf( x * x + y * y + z * z + w * w);
	}

	inline float LengthSquared() const
	{
		return x * x + y * y + z * z + w * w;
	}

	inline float Dot(const Vector4& other ) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
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

	inline Vector4 Normalised() const
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
		return Vector4(nx, ny, nz, nw);
	}
    
	// Class Functions
	static void ComputeExtremes(int numVectors, const Vector4* vectors,
								Vector4& vmin, Vector4& vmax);
};