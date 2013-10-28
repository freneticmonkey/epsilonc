#pragma once

#include <valarray>
#include <array>

#include "math/Vector.h"
#include "math/Rotation.h"
#include "math/Quaternion.h"

class Quaternion;

typedef std::valarray<float> MatrixData;

class Matrix3
{
private:
     float data[9];
public:
    
    // Constructors
    Matrix3();
    Matrix3( const Matrix3& mat);
    Matrix3( float a, float b, float c,
             float e, float f, float g,
             float i, float j, float k );
    
    // Comparison
    inline bool operator==(const Matrix3& other) const;
    inline bool operator!=(const Matrix3& other) const;
    
    // Array Access
	float &operator[](int i);
    
    Matrix3 operator* (const Matrix3& other) const;//, const Matrix3 other);
    //inline friend Matrix3 operator* (const Matrix3 other, const Matrix3& op1);
    
    Vector3 operator* (const Vector3& vec) const;
    
    inline Matrix3& operator*=( const Matrix3& other );
	
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
    
    // Comparison
    inline bool operator==(const Matrix4& other) const;
    inline bool operator!=(const Matrix4& other) const;

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
    
    inline Matrix4& operator*=( const Matrix4& other );
    
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