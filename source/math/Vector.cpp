#include "math/Vector.h"
#include <iostream>
//#include <math.h>

// VECTOR 2

Vector2::Vector2(float v)
{
    x = v;
    y = v;
}

Vector2::Vector2(const Vector2& vec)
{
	x = vec.x;
	y = vec.y;
}

Vector2::Vector2(float ix, float iy)
{
	x = ix;
	y = iy;
}

Vector2& Vector2::operator= (const Vector2& vec)
{
    x = vec.x;
	y = vec.y;
    return *this;
}

float &Vector2::operator[](int i)
{
	if ( i == 0 )
	{
		return x;
	}
	else if ( i == 1 )
	{
		return y;
	}
    else
    {
        std::cout << "Vector2: Boundry error\n";
        exit(1);
    }
}

float Vector2::Cross( const Vector2& other ) const
{
	return x * other.y - y * other.x;
}

Vector2 Vector2::Reflect(const Vector2& normal) const
{
	float d = 2 * x * normal.x + y * normal.y;

	return Vector2( x - d * normal.x,
					y - d * normal.y );
}

float Vector2::Angle(const Vector2& other) const
{
	return acosf( Dot(other) / Length() * other.Length() );
}

Vector2 Vector2::Project(const Vector2& other) const
{
	Vector2 onorm = other.Normalised();

	return Dot(onorm) * onorm;
}

void Vector2::ComputeExtremes( int numVectors, const Vector2* vectors,
									  Vector2& vmin, Vector2& vmax)
{
	if ( numVectors > 0 )
	{
		vmin.x = vectors[0].x;
		vmin.y = vectors[0].y;
		vmax.x = vectors[0].x;
		vmax.y = vectors[0].y;

		for ( int i = 0; i < numVectors; i++ )
		{
			if ( vectors[i].x < vmin.x )
			{
				vmin.x = vectors[i].x;
			}
			if ( vectors[i].y < vmin.y )
			{
				vmin.y = vectors[i].y;
			}
			if ( vectors[i].x > vmax.x )
			{
				vmax.x = vectors[i].x;
			}
			if ( vectors[i].y > vmax.y )
			{
				vmax.y = vectors[i].y;
			}
		}
	}
}


/// VECTOR 3

const Vector3 Vector3::UP(0.f, 1.f, 0.f);
const Vector3 Vector3::RIGHT(1.f, 0.f, 0.f);
const Vector3 Vector3::FORWARD(0.f, 0.f, 1.f);
const Vector3 Vector3::DOWN(0.f, 1.f, 0.f);
const Vector3 Vector3::LEFT(1.f, 0.f, 0.f);
const Vector3 Vector3::BACKWARD(0.f, 0.f, 1.f);

const Vector3 Vector3::ZERO(0.f, 0.f, 0.f);
const Vector3 Vector3::ONE(1.f, 1.f, 1.f);
const Vector3 Vector3::IDENTITY(0.f, 0.f, 1.f);

const Vector3 Vector3::UNIT_X(1.f, 0.f, 0.f);
const Vector3 Vector3::UNIT_Y(0.f, 1.f, 0.f);
const Vector3 Vector3::UNIT_Z(0.f, 0.f, 1.f);

Vector3::Vector3(float v)
{
    x = v;
    y = v;
    z = v;
}

Vector3::Vector3(const Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

Vector3::Vector3(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
}

/*
inline Vector3& operator= (const Vector3& vec)
{
    x = vec.x;
	y = vec.y;
	z = vec.z;
    return *this;
}
 */

float &Vector3::operator[](int i)
{
	if ( i < 0 || i > 2 )
	{
		std::cout << "Vector3: Boundry error\n";
		exit(1);
	}
	else if ( i == 0 )
	{
		return x;
	}
	else if ( i == 1 )
	{
		return y;
	}
	else if ( i == 2 )
	{
		return z;
	}
	return x;// Should never be hit.
}

Vector3 Vector3::Cross( const Vector3& other ) const
{
	return Vector3( y * other.z - z * other.y,
					z * other.x - x * other.z,
					x * other.y - y * other.x);
}

Vector3 Vector3::UnitCross( const Vector3& other ) const
{
	Vector3 cross = Vector3( y * other.z - z * other.y,
							 z * other.x - x * other.z,
							 x * other.y - y * other.x);
	cross.Normalise();
	return cross;
}


Vector3 Vector3::Reflect(const Vector3& normal) const
{
	float d = 2 * x * normal.x + y * normal.y + z * normal.z;

	return Vector3( x - d * normal.x,
					y - d * normal.y,
					z - d * normal.z);
}

Vector3 Vector3::RotateAround(const Vector3& axis, float theta)
{
	// Adapted from equations published by Glenn Murray.
    // http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html
	float u = axis.x;
	float v = axis.y;
	float w = axis.z;

    // Extracted common factors for simplicity and efficiency
    float r2 = pow(u,2) + pow(v,2) + pow(w,2);
    float r = sqrt(r2);
    float ct = cos(theta);
    float st = sin(theta) / r;
    float dt = (u*x + v*y + w*z) * (1 - ct) / r2;

    return Vector3((u * dt + x * ct + (-w * y + v * z) * st),
                    (v * dt + y * ct + ( w * x - u * z) * st),
                    (w * dt + z * ct + (-v * x + u * y) * st));
}

Vector3 Vector3::Project(const Vector3& other) const
{
	Vector3 onorm = other.Normalised();

	return Dot(onorm) * onorm;
}

void Vector3::ComputeExtremes( const Vector3::List vectors,
									  Vector3& vmin, Vector3& vmax)
{
	if ( vectors.size() > 0 )
	{
		const Vector3 vector = (*vectors.begin());
		vmin.x = vector.x;
		vmin.y = vector.y;
		vmin.z = vector.z;

		vmax.x = vector.x;
		vmax.y = vector.y;
		vmax.z = vector.z;

		//for ( int i = 0; i < numVectors; i++ )
		for (Vector3::List::const_iterator vector = vectors.begin(); vector != vectors.end(); vector++)
		{
			const Vector3 v = (*vector);
			if ( v.x < vmin.x )
			{
				vmin.x = v.x;
			}
			if ( v.y < vmin.y )
			{
				vmin.y = v.y;
			}
			if ( v.z < vmin.z )
			{
				vmin.z = v.z;
			}

			if ( v.x > vmax.x )
			{
				vmax.x = v.x;
			}
			if ( v.y > vmax.y )
			{
				vmax.y = v.y;
			}
			if ( v.z > vmax.z )
			{
				vmax.z = v.z;
			}
		}
	}
}

/// VECTOR 4
Vector4::Vector4(float v)
{
    x = v;
    y = v;
    z = v;
    w = v;
}

Vector4::Vector4(const Vector4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
    w = vec.w;
}

Vector4::Vector4(float ix, float iy, float iz, float iw)
{
	x = ix;
	y = iy;
	z = iz;
    w = iw;
}

Vector4& Vector4::operator=(const Vector4& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
	return *this;
}

float &Vector4::operator[](int i)
{
    if ( i == 0 )
	{
		return x;
	}
	else if ( i == 1 )
	{
		return y;
	}
	else if ( i == 2 )
	{
		return z;
	}
	else if ( i == 3 )
	{
		return w;
	}
    else
    {
        std::cout << "Vector4: Boundry error\n";
		exit(1);
    }
}

void Vector4::ComputeExtremes( int numVectors, const Vector4* vectors,
                              Vector4& vmin, Vector4& vmax)
{
	if ( numVectors > 0 )
	{
		vmin.x = vectors[0].x;
		vmin.y = vectors[0].y;
		vmin.z = vectors[0].z;
        vmin.w = vectors[0].w;
        
		vmax.x = vectors[0].x;
		vmax.y = vectors[0].y;
		vmax.z = vectors[0].z;
        vmax.w = vectors[0].w;
        
		for ( int i = 0; i < numVectors; i++ )
		{
			if ( vectors[i].x < vmin.x )
			{
				vmin.x = vectors[i].x;
			}
			if ( vectors[i].y < vmin.y )
			{
				vmin.y = vectors[i].y;
			}
			if ( vectors[i].z < vmin.z )
			{
				vmin.z = vectors[i].z;
			}
            if ( vectors[i].w < vmin.w )
			{
				vmin.w = vectors[i].w;
			}
            
			if ( vectors[i].x > vmax.x )
			{
				vmax.x = vectors[i].x;
			}
			if ( vectors[i].y > vmax.y )
			{
				vmax.y = vectors[i].y;
			}
			if ( vectors[i].z > vmax.z )
			{
				vmax.z = vectors[i].z;
			}
            if ( vectors[i].w > vmax.w )
			{
				vmax.w = vectors[i].w;
			}
		}
	}
}