#pragma once

#include <array>

#include "math/Vector.h"
#include "math/Rotation.h"
#include "math/Quaternion.h"

class Quaternion;

class Matrix3
{
private:
     float data[9];
public:
    
    // Constructors
    Matrix3();
    Matrix3( const Matrix3& mat);
	Matrix3(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    Matrix3( float a, float b, float c,
             float e, float f, float g,
             float i, float j, float k );

	// To String
	inline std::string ToString() 
	{
		return boost::str(format("[ %5.5f, %5.5f, %5.5f ]\n \
                                 [ %5.5f, %5.5f, %5.5f ]\n \
                                 [ %5.5f, %5.5f, %5.5f ]\n") %
                                  data[0] % data[1] % data[2] %
                                  data[3] % data[4] % data[5] %
                                  data[6] % data[7] % data[8] );
	}
    
    // Comparison
    inline bool operator==(const Matrix3& other) const
	{
		for ( int i = 0; i < 9; i++)
		{
			if ( data[i] != other.data[i] )
			{
				return false;
			}
		}
		return true;
	}

	inline bool operator!=(const Matrix3& other) const
	{
		int matches = 0;
		for ( int i = 0; i < 9; i++)
		{
			if ( data[i] != other.data[i] )
			{
				matches++;
			}
		}
		return ( matches < 9 );
	}
    
    // Array Access
	float &operator[](int i);
    
    Matrix3 operator* (const Matrix3& other) const;//, const Matrix3 other);
    //inline friend Matrix3 operator* (const Matrix3 other, const Matrix3& op1);
    
    Vector3 operator* (const Vector3& vec) const;
    
    inline Matrix3& operator*=( const Matrix3& other )
	{
		float mydata[9];// = data;
	
		for (int i = 0; i < 9; i++)
		{
			mydata[i] = data[i];
		}
    
		mydata[0] = data[0] * other.data[0] + data[1] * other.data[3] + data[2] * other.data[6];
		mydata[1] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[7];
		mydata[2] = data[0] * other.data[0] + data[1] * other.data[5] + data[2] * other.data[8];
    
		mydata[3] = data[3] * other.data[0] + data[4] * other.data[3] + data[5] * other.data[6];
		mydata[4] = data[3] * other.data[1] + data[4] * other.data[4] + data[5] * other.data[7];
		mydata[5] = data[3] * other.data[2] + data[4] * other.data[5] + data[5] * other.data[8];
    
		mydata[6] = data[6] * other.data[0] + data[7] * other.data[3] + data[8] * other.data[6];
		mydata[7] = data[6] * other.data[1] + data[7] * other.data[4] + data[8] * other.data[7];
		mydata[8] = data[6] * other.data[2] + data[7] * other.data[5] + data[8] * other.data[8];
    
		//data = mydata;
		for (int i = 0; i < 9; i++)
		{
			data[i] = mydata[i];
		}
    
		return *this;
	}
	
    Matrix3 Scale(float x, float y);
    Matrix3 Translate(float x, float y);
    Matrix3 Rotate(float angle);
    
    Matrix3 CreateScale(float x, float y);
    Matrix3 CreateTranslation(float x, float y);
    Matrix3 CreateRotation(float angle);
    
    float Determinant();
    Matrix3 Inverse();
    
};

class Matrix4
{
private:
    std::array<float, 16> data;
public:
    
    // Constructors
    Matrix4();
    Matrix4( const Matrix4& mat );
	Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);
	Matrix4(float a, float b, float c, float d,
			float e, float f, float g, float h,
			float i, float j, float k, float l,
			float m, float n, float o, float p);
    
	// To String
	inline std::string ToString() 
	{
		return boost::str(format("[ %5.5f, %5.5f, %5.5f, %5.5f ]\n \
                                 [ %5.5f, %5.5f, %5.5f, %5.5f ]\n \
                                 [ %5.5f, %5.5f, %5.5f, %5.5f ]\n \
                                 [ %5.5f, %5.5f, %5.5f, %5.5f ]\n") %
                                  data[0] % data[1] % data[2] % data[3] %
                                  data[4] % data[5] % data[6] % data[7] %
                                  data[8] % data[9] % data[10] % data[11] % 
                                  data[12] % data[13] % data[14] % data[15] );
	}

    // Comparison
	inline bool operator==(const Matrix4& other) const
	{
		for ( int i = 0; i < 16; i++)
		{
			if ( data[i] != other.data[i] )
			{
				return false;
			}
		}
		return true;
	}

	inline bool operator!=(const Matrix4& other) const
	{
		int matches = 0;
		for ( int i = 0; i < 16; i++)
		{
			if ( data[i] != other.data[i] )
			{
				matches++;
			}
		}
		return ( matches < 16 );
	}

	// Conversion to float
	operator float()
	{
		return data[0];
	}
    
    // Array Access
	float &operator[](int i);
    
    Matrix4 operator* (const Matrix4 &other) const;
    //inline friend Matrix4 operator* (const Matrix4 other, const Matrix4& op1);
    
    Vector3 operator* (const Vector3 vec) const;
    
    inline Matrix4& operator*=( const Matrix4& other )
	{
		float mydata[16];// = data;
		for (int i = 0; i < 16; i++)
		{
			mydata[i] = data[i];
		}
    
		mydata[0] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[8]  + data[3] * other.data[12];
		mydata[1] = data[0] * other.data[1] + data[1] * other.data[5] + data[2] * other.data[9]  + data[3] * other.data[13];
		mydata[2] = data[0] * other.data[2] + data[1] * other.data[6] + data[2] * other.data[10] + data[3] * other.data[14];
		mydata[3] = data[0] * other.data[3] + data[1] * other.data[7] + data[2] * other.data[11] + data[3] * other.data[15];
    
		mydata[4] = data[4] * other.data[0] + data[5] * other.data[4] + data[6] * other.data[8]  + data[7] * other.data[12];
		mydata[5] = data[4] * other.data[1] + data[5] * other.data[5] + data[6] * other.data[9]  + data[7] * other.data[13];
		mydata[6] = data[4] * other.data[2] + data[5] * other.data[6] + data[6] * other.data[10] + data[7] * other.data[14];
		mydata[7] = data[4] * other.data[3] + data[5] * other.data[7] + data[6] * other.data[11] + data[7] * other.data[15];
    
		mydata[8]  = data[8] * other.data[0] + data[9] * other.data[4] + data[10] * other.data[8]  + data[11] * other.data[12];
		mydata[9]  = data[8] * other.data[1] + data[9] * other.data[5] + data[10] * other.data[9]  + data[11] * other.data[13];
		mydata[10] = data[8] * other.data[2] + data[9] * other.data[6] + data[10] * other.data[10] + data[11] * other.data[14];
		mydata[11] = data[8] * other.data[3] + data[9] * other.data[7] + data[10] * other.data[11] + data[11] * other.data[15];
    
		mydata[12] = data[12] * other.data[0] + data[13] * other.data[4] + data[14] * other.data[8]  + data[15] * other.data[12];
		mydata[13] = data[12] * other.data[1] + data[13] * other.data[5] + data[14] * other.data[9]  + data[15] * other.data[13];
		mydata[14] = data[12] * other.data[2] + data[13] * other.data[6] + data[14] * other.data[10] + data[15] * other.data[14];
		mydata[15] = data[12] * other.data[3] + data[13] * other.data[7] + data[14] * other.data[11] + data[15] * other.data[15];
    
		//data = mydata;
		for (int i = 0; i < 16; i++)
		{
			data[i] = mydata[i];
		}
    
		return *this;
	}
    
    Vector3 Transform(const Vector3& other);
    
    Matrix4 Scale(float x, float y, float z);
    Matrix4 Translate(float x, float y, float z);
    Matrix4 RotateX(float angle);
    Matrix4 RotateY(float angle);
    Matrix4 RotateZ(float angle);
    
    Matrix4 RotateAxis(Vector3 axis, float angle);
    Matrix4 RotateEuler(float heading, float attitude, float bank);
    Matrix4 RotateTripleAxis(Vector3 x, Vector3 y, Vector3 z);

    void Transpose();
    Matrix4 Transposed();

    // Build a matrix from transforms
    static Matrix4 CreateScale(float x, float y, float z);
    static Matrix4 CreateTranslation(float x, float y, float z);
    static Matrix4 CreateRotateX(float angle);
    static Matrix4 CreateRotateY(float angle);
    static Matrix4 CreateRotateZ(float angle);
    static Matrix4 CreateRotateAxis(Vector3 axis, float angle);
    static Matrix4 CreateRotateEuler(float heading, float attitude, float bank);
    static Matrix4 CreateRotateTripleAxis(Vector3 x, Vector3 y, Vector3 z);
    static Matrix4 CreateRotateQuaternion(Quaternion quat);
    
    static Matrix4 CreateLookAt(Vector3 eye, Vector3 at, Vector3 up);
    static Matrix4 CreatePerspective(float fovY, float aspect, float near, float far);

    float Determinant();
    Matrix4 Inverse();

    Vector3 GetTranslation();
    Vector3 GetScale();
    Quaternion GetRotation();
    
};