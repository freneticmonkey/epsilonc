//
//  Matrix.c
//  Epsilon
//
//  Created by Scott Porter on 30/12/12.
//  Copyright (c) 2012 Scott Porter. All rights reserved.
//

#include <stdio.h>

#include "math/Matrix.h"
#include "error/Exception.h"

// Constructors
Matrix3::Matrix3()
{
	//data.resize(9);
    // Set data to identity
    data[0] = data[4] = data[8] = 1.f;
    data[1] = data[2] = data[3] = data[5] = data[6] = data[7] = 0.f;
}

Matrix3::Matrix3( const Matrix3& mat)
{
	for (int i = 0; i < 9; i++)
	{
		data[i] = mat.data[i];
	}
}

Matrix3::Matrix3(const Vector3& v1, 
				 const Vector3& v2, 
				 const Vector3& v3)
{
	data[0] = v1.x;
	data[1] = v1.y;
	data[2] = v1.z;

	data[3] = v2.x;
	data[4] = v2.y;
	data[5] = v2.z;

	data[6] = v3.x;
	data[7] = v3.y;
	data[8] = v3.z;
}

Matrix3::Matrix3( float a, float b, float c,
                 float e, float f, float g,
                 float i, float j, float k )
{
    data[0] = a;
    data[1] = b;
    data[2] = c;
    
    data[3] = e;
    data[4] = f;
    data[5] = g;
    
    data[6] = i;
    data[7] = j;
    data[8] = k;
}

// Array Access
float &Matrix3::operator[](int i)
{
	if ( i < 0 || i > 9 )
	{
		throw BoundaryException("Matrix3", i, 9);
	}
	
	return data[i];
}

//inline Matrix3 operator* (const Matrix3& op1, const Matrix3 other)
Matrix3 Matrix3::operator* (const Matrix3& other) const
{
    Matrix3 result;
    result.data[0] = data[0] * other.data[0] + data[1] * other.data[3] + data[2] * other.data[6];
    result.data[1] = data[0] * other.data[0] + data[1] * other.data[4] + data[2] * other.data[7];
    result.data[2] = data[0] * other.data[0] + data[1] * other.data[5] + data[2] * other.data[8];
    
    result.data[3] = data[3] * other.data[0] + data[4] * other.data[3] + data[5] * other.data[6];
    result.data[4] = data[3] * other.data[1] + data[4] * other.data[4] + data[5] * other.data[7];
    result.data[5] = data[3] * other.data[2] + data[4] * other.data[5] + data[5] * other.data[8];
    
    result.data[6] = data[6] * other.data[0] + data[7] * other.data[3] + data[8] * other.data[6];
    result.data[7] = data[6] * other.data[1] + data[7] * other.data[4] + data[8] * other.data[7];
    result.data[8] = data[6] * other.data[2] + data[7] * other.data[5] + data[8] * other.data[8];
    
    return result;
}

/*
inline Matrix3 operator* (const Matrix3 other, const Matrix3& op1)
{
    Matrix3 result;
    result.data[0] = op1.data[0] * other.data[0] + op1.data[1] * other.data[3] + op1.data[2] * other.data[6];
    result.data[1] = op1.data[0] * other.data[0] + op1.data[1] * other.data[4] + op1.data[2] * other.data[7];
    result.data[2] = op1.data[0] * other.data[0] + op1.data[1] * other.data[5] + op1.data[2] * other.data[8];
    
    result.data[3] = op1.data[3] * other.data[0] + op1.data[4] * other.data[3] + op1.data[5] * other.data[6];
    result.data[4] = op1.data[3] * other.data[1] + op1.data[4] * other.data[4] + op1.data[5] * other.data[7];
    result.data[5] = op1.data[3] * other.data[2] + op1.data[4] * other.data[5] + op1.data[5] * other.data[8];
    
    result.data[6] = op1.data[6] * other.data[0] + op1.data[7] * other.data[3] + op1.data[8] * other.data[6];
    result.data[7] = op1.data[6] * other.data[1] + op1.data[7] * other.data[4] + op1.data[8] * other.data[7];
    result.data[8] = op1.data[6] * other.data[2] + op1.data[7] * other.data[5] + op1.data[8] * other.data[8];
    
    return result;
}
 */

Vector3 Matrix3::operator* (const Vector3& vec) const
{
    Vector3 res;
    res[0] = data[0] * vec.x + data[1] * vec.y + data[2] * vec.z;
    res[1] = data[3] * vec.x + data[4] * vec.y + data[5] * vec.z;
    res[2] = data[6] * vec.x + data[7] * vec.y + data[8] * vec.z;
    
    return res;
}

Matrix3 Matrix3::Scale(float x, float y)
{
    return *this *= CreateScale(x, y);
}

Matrix3 Matrix3::Translate(float x, float y)
{
    return *this *= CreateTranslation(x, y);
}

Matrix3 Matrix3::Rotate(float angle)
{
    return *this *= CreateRotation(angle);
}


Matrix3 Matrix3::CreateScale(float x, float y)
{
    Matrix3 scale;
    scale.data[0] = x;
    scale.data[4] = y;
    return scale;
}

Matrix3 Matrix3::CreateTranslation(float x, float y)
{
    Matrix3 trans;
    trans.data[2] = x;
    trans.data[5] = y;
	return trans;
}

Matrix3 Matrix3::CreateRotation(float angle)
{
    Matrix3 rot;
    float s = std::sin(angle);
    float c = std::cos(angle);
    rot.data[0] = rot.data[4] = c;
    rot.data[1] = -s;
    rot.data[3] = s;
    return rot;
}


float Matrix3::Determinant()
{
    return ( data[0] * data[4] * data[8] \
            +data[1] * data[5] * data[6] \
            +data[2] * data[3] * data[7] \
            -data[0] * data[5] * data[7] \
            -data[1] * data[3] * data[8] \
            -data[2] * data[4] * data[6] );
}

Matrix3 Matrix3::Inverse()
{
    Matrix3 tmp;
    float d = Determinant();
    
    if ( std::abs(d) < 0.001f )
    {
        return tmp;
    }
    else
    {
        d = 1.f / d;
        
        tmp.data[0] = d * ( data[4] * data[8] - data[5] * data[7] );
        tmp.data[1] = d * ( data[2] * data[7] - data[1] * data[8] );
        tmp.data[2] = d * ( data[1] * data[5] - data[2] * data[4] );
        
        tmp.data[3] = d * ( data[5] * data[6] - data[3] * data[8] );
        tmp.data[4] = d * ( data[0] * data[8] - data[2] * data[6] );
        tmp.data[5] = d * ( data[2] * data[3] - data[0] * data[5] );
        
        tmp.data[6] = d * ( data[3] * data[7] - data[4] * data[6] );
        tmp.data[7] = d * ( data[1] * data[6] - data[0] * data[7] );
        tmp.data[8] = d * ( data[0] * data[4] - data[1] * data[3] );
        
        return tmp;
    }
}


/// MATRIX4
// [0]a [1]b [2]c [3]d
// [4]e [5]f [6]g [7]h
// [8]i [9]j [10]k[11]l
// [12]m[13]n[14]o[15]p
// Constructors
Matrix4::Matrix4()
{
    //data.resize(16);
    // Set data to identity
    
    data[0] = data[5] = data[10] = data[15] = 1.f;
    data[1] = data[2] = data[3] = data[4] = data[6] = data[7] = 0.f;
    data[8] = data[9] = data[11] = data[12] = data[13] = data[14] = 0.f;
}

Matrix4::Matrix4( const Matrix4& mat)
{
    data = mat.data;
	/*
	for (int i = 0; i < 16; i++)
	{
		data[i] = mat.data[i];
	}
	*/
}

Matrix4::Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4)
{
	data[0] = v1.x;		data[1] = v1.y;		data[2] = v1.z;		data[3] = v1.w;
	data[4] = v2.x;		data[5] = v2.y;		data[6] = v2.z;		data[7] = v2.w;
	data[8] = v3.x;		data[9] = v3.y;		data[10] = v3.z;	data[11] = v3.w;
	data[12] = v4.x;	data[13] = v4.y;	data[14] = v4.z;	data[15] = v4.w;
}

Matrix4::Matrix4(float a, float b, float c, float d,
	float e, float f, float g, float h,
	float i, float j, float k, float l,
	float m, float n, float o, float p)
{
	data[0] = a; data[1] = b; data[2] = c; data[3] = d;
	data[4] = e; data[5] = f; data[6] = g; data[7] = h;
	data[8] = i; data[9] = j; data[10] = k; data[11] = l;
	data[12]= m; data[13]= n; data[14]= o; data[15]= p;
}

// Array Access
float &Matrix4::operator[](int i)
{
    if ( i < 0 || i > 16 )
	{
		throw BoundaryException("Matrix4", i, 16);
	}
	
	return data[i];
}

Matrix4 Matrix4::operator* (const Matrix4 &other) const
{
    Matrix4 result;
	std::array<float,16> t = data;
	std::array<float, 16> o = other.data;

	result.data[0] = (t[0] * o[0]) + (t[1] * o[4]) + (t[2] * o[8])  + ( t[3] * o[12]);
	result.data[1] = (t[0] * o[1]) + (t[1] * o[5]) + (t[2] * o[9])  + ( t[3] * o[13]);
	result.data[2] = (t[0] * o[2]) + (t[1] * o[6]) + (t[2] * o[10]) + ( t[3] * o[14]);
	result.data[3] = (t[0] * o[3]) + (t[1] * o[7]) + (t[2] * o[11]) + ( t[3] * o[15]);

	result.data[4] = (t[4] * o[0]) + (t[5] * o[4]) + (t[6] * o[8])  + ( t[7] * o[12]);
	result.data[5] = (t[4] * o[1]) + (t[5] * o[5]) + (t[6] * o[9])  + ( t[7] * o[13]);
	result.data[6] = (t[4] * o[2]) + (t[5] * o[6]) + (t[6] * o[10]) + ( t[7] * o[14]);
	result.data[7] = (t[4] * o[3]) + (t[5] * o[7]) + (t[6] * o[11]) + ( t[7] * o[15]);

	result.data[8]  = (t[8] * o[0]) + (t[9] * o[4]) + (t[10] * o[8])  + ( t[11] * o[12]);
	result.data[9]  = (t[8] * o[1]) + (t[9] * o[5]) + (t[10] * o[9])  + ( t[11] * o[13]);
	result.data[10] = (t[8] * o[2]) + (t[9] * o[6]) + (t[10] * o[10]) + ( t[11] * o[14]);
	result.data[11] = (t[8] * o[3]) + (t[9] * o[7]) + (t[10] * o[11]) + ( t[11] * o[15]);

	result.data[12] = (t[12] * o[0]) + (t[13] * o[4]) + (t[14] * o[8])  + ( t[15] * o[12]);
	result.data[13] = (t[12] * o[1]) + (t[13] * o[5]) + (t[14] * o[9])  + ( t[15] * o[13]);
	result.data[14] = (t[12] * o[2]) + (t[13] * o[6]) + (t[14] * o[10]) + ( t[15] * o[14]);
	result.data[15] = (t[12] * o[3]) + (t[13] * o[7]) + (t[14] * o[11]) + ( t[15] * o[15]);

    return result;
    
}

Vector3 Matrix4::operator* (const Vector3 vec) const
{
    Vector3 result;
    result.x = data[0] * vec.x + data[1] * vec.y + data[2] * vec.z;
    result.y = data[4] * vec.x + data[5] * vec.y + data[6] * vec.z;
    result.z = data[8] * vec.x + data[9] * vec.y + data[10] * vec.z;
    
    return result;
}

Vector3 Matrix4::Transform(const Vector3& vec)
{
    Vector3 trans;
    float w;
    
    trans.x = data[0] * vec.x + data[1] * vec.y + data[2] * vec.z + data[3];
    trans.y = data[4] * vec.x + data[5] * vec.y + data[6] * vec.z + data[7];
    trans.z = data[8] * vec.x + data[9] * vec.y + data[10] * vec.z + data[11];
    w =   data[12] * vec.x + data[13] * vec.y + data[14] * vec.z + data[15];
    if ( w != 0)
    {
        trans.x /= w;
        trans.y /= w;
        trans.z /= w;
    }
        
    return trans;
}

Matrix4 Matrix4::Scale(float x, float y, float z)
{
    return *this *= CreateScale(x, y, z);
}

Matrix4 Matrix4::Translate(float x, float y, float z)
{
    return *this *= CreateTranslation(x, y, z);
}

Matrix4 Matrix4::RotateX(float angle)
{
    return *this *= CreateRotateX(angle);
}

Matrix4 Matrix4::RotateY(float angle)
{
    return *this *= CreateRotateY(angle);
}

Matrix4 Matrix4::RotateZ(float angle)
{
    return *this *= CreateRotateZ(angle);
}

Matrix4 Matrix4::RotateAxis(Vector3 axis, float angle)
{
    return *this *= CreateRotateAxis(axis, angle);
}

Matrix4 Matrix4::RotateEuler(float heading, float attitude, float bank)
{
    return *this *= CreateRotateEuler(heading, attitude, bank);
}

Matrix4 Matrix4::RotateTripleAxis(Vector3 x, Vector3 y, Vector3 z)
{
    return *this *= CreateRotateTripleAxis(x, y, z);
}

void Matrix4::Transpose()
{
    float tmpdata[16];// = data;
	for (int i = 0; i < 16; i++)
	{
		tmpdata[i] = data[i];
	}
    
    data[0] = tmpdata[0];
    data[4] = tmpdata[1];
    data[8] = tmpdata[2];
    data[12] = tmpdata[3];
    data[1] = tmpdata[4];
    data[5] = tmpdata[5];
    data[9] = tmpdata[6];
    data[13] = tmpdata[7];
    data[2] = tmpdata[8];
    data[6] = tmpdata[9];
    data[10] = tmpdata[10];
    data[14] = tmpdata[11];
    data[3] = tmpdata[12];
    data[7] = tmpdata[13];
    data[11] = tmpdata[14];
    data[15] = tmpdata[15];
}

Matrix4 Matrix4::Transposed()
{
    Matrix4 copy(*this);
    copy.Transpose();
    return copy;
}


Matrix4 Matrix4::CreateScale(float x, float y, float z)
{
    Matrix4 newmat;
    newmat.data[0] = x;
    newmat.data[5] = y;
    newmat.data[10] = z;
    return newmat;
}

Matrix4 Matrix4::CreateTranslation(float x, float y, float z)
{
    Matrix4 newmat;
    newmat.data[3] = x;
    newmat.data[7] = y;
    newmat.data[11] = z;
    return newmat;
}

Matrix4 Matrix4::CreateRotateX(float angle)
{
    Matrix4 newmat;
    float s = std::sin(angle);
    float c = std::cos(angle);
    newmat.data[5] = newmat.data[10] = c;
    newmat.data[6] = s;
    newmat.data[9] = -s;
    return newmat;
}

Matrix4 Matrix4::CreateRotateY(float angle)
{
    Matrix4 newmat;
    float s = std::sin(angle);
    float c = std::cos(angle);
    newmat.data[0] = newmat.data[10] = c;
    newmat.data[2] = s;
    newmat.data[8] = -s;
    return newmat;
}

Matrix4 Matrix4::CreateRotateZ(float angle)
{
    Matrix4 newmat;
    float s = std::sin(angle);
    float c = std::cos(angle);
    newmat.data[0] = newmat.data[5] = c;
    newmat.data[1] = -s;
    newmat.data[4] = -s;
    return newmat;
}

Matrix4 Matrix4::CreateRotateAxis(Vector3 axis, float angle)
{
    Matrix4 newmat;
    
    Vector3 vec = axis;
    vec.Normalise();
    
    float s = std::sin(angle);
    float c = std::cos(angle);
    
    float c1 = 1.f - c;
    newmat.data[0] = vec.x * vec.x * c1 + c;
    newmat.data[1] = vec.x * vec.y * c1 - vec.z * s;
    newmat.data[2] = vec.x * vec.z * c1 + vec.y * s;
    newmat.data[4] = vec.y * vec.x * c1 + vec.z * s;
    newmat.data[5] = vec.y * vec.y * c1 + c;
    newmat.data[6] = vec.y * vec.z * c1 - vec.x * s;
    newmat.data[8] = vec.x * vec.z * c1 - vec.y * s;
    newmat.data[9] = vec.y * vec.z * c1 + vec.x * s;
    newmat.data[10] = vec.z * vec.z * c1 + c;
    return newmat;
}

Matrix4 Matrix4::CreateRotateEuler(float heading, float attitude, float bank)
{
    float ch = std::cos(heading);
    float sh = std::sin(heading);
    float ca = std::cos(attitude);
    float sa = std::sin(attitude);
    float cb = std::cos(bank);
    float sb = std::sin(bank);
    
    Matrix4 newmat;
    newmat.data[0] = ch * ca;
    newmat.data[1] = sh * sb - ch * sa * cb;
    newmat.data[2] = ch * sa * sb + sh * cb;
    
    newmat.data[4] = sa;
    newmat.data[5] = ca * cb;
    newmat.data[6] = -ca * sb;
    
    newmat.data[8] = -sh * ca;
    newmat.data[9] = sh * sa * cb + ch * sb;
    newmat.data[10] = -sh * sa * sb + ch * cb;
    
    return newmat;
}

Matrix4 Matrix4::CreateRotateTripleAxis(Vector3 x, Vector3 y, Vector3 z)
{
    Matrix4 newmat;
    newmat.data[0] = x.x;
    newmat.data[1] = y.x;
    newmat.data[2] = z.x;
    
    newmat.data[4] = x.y;
    newmat.data[5] = y.y;
    newmat.data[6] = z.y;
    
    newmat.data[8] = x.z;
    newmat.data[9] = y.z;
    newmat.data[10] = z.z;
    
    return newmat;
}

Matrix4 Matrix4::CreateRotateQuaternion(Quaternion quat)
{
    Matrix4 newmat;
    
    float xx = quat.x * quat.x;
    float xy = quat.x * quat.y;
    float xz = quat.x * quat.z;
    float xw = quat.x * quat.w;
    float yy = quat.y * quat.y;
    float yz = quat.y * quat.z;
    float yw = quat.y * quat.w;
    float zz = quat.z * quat.z;
    float zw = quat.z * quat.w;
    
	newmat.data[0] = 1 - 2 * (yy + zz);
    newmat.data[1] = 2 * (xy - zw);
    newmat.data[2] = 2 * (xz + yw);
    newmat.data[4] = 2 * (xy + zw);
    newmat.data[5] = 1 - 2 * (xx + zz);
    newmat.data[6] = 2 * (yz - xw);
    newmat.data[8] = 2 * (xz - yw);
    newmat.data[9] = 2 * (yz + xw);
    newmat.data[10] = 1 - 2 * (xx + yy);
    
    return newmat;
}

Matrix4 Matrix4::CreateLookAt(Vector3 eye, Vector3 at, Vector3 up)
{
	Vector3 z = (eye - at).Normalised();
    Vector3 x = up.Cross(z).Normalised();
	Vector3 y = z.Cross(x).Normalised();
	
    Matrix4 newmat = CreateRotateTripleAxis(x, y, z);
    newmat.data[3] = -eye.x;
    newmat.data[7] = -eye.y;
    newmat.data[11] = eye.z;

	/*
	Vector3 z = (at - eye).Normalised();
	Vector3 x = up.Normalised();//.Cross(z).Normalised();
	Vector3 y = z.Cross(x).Normalised();
	x = y.Cross(z);
    
    Matrix4 newmat = CreateRotateTripleAxis(y, x, -z);
	//newmat.data[12] = -eye.Dot(y);//y.Dot(eye);
	newmat.data[12] = -y.Dot(eye);

    //newmat.data[13] = -eye.Dot(x);//x.Dot(eye);
	newmat.data[13] = -x.Dot(eye);

    //newmat.data[14] = eye.Dot(z);//z.Dot(eye);
	newmat.data[14] = z.Dot(eye);
	*/
    
    return newmat;
}

Matrix4 Matrix4::CreatePerspective(float fovY, float aspect, float nearDist, float farDist)
{
    float f = 1.f / std::tan(fovY / 2.f );
    Matrix4 newmat;
    
    if ( nearDist != 0.f && farDist != 0.f )
    {
        newmat.data[0] = f / aspect;
        newmat.data[5] = f;
        newmat.data[10] = ( farDist + nearDist ) / ( nearDist - farDist );
        newmat.data[11] = 2.f * farDist * nearDist / ( nearDist - farDist);
        newmat.data[14] = -1.f;
        newmat.data[15] = 0.f;
    }
    return newmat;
}

Matrix4 Matrix4::CreateOrthographic(float left, float right, float top, float bottom, float nearDist, float farDist)
{
	Matrix4 newMat;

	newMat.data[0] = 2 / (right - left);
	newMat.data[1] = 0;
	newMat.data[2] = 0;
	newMat.data[3] = 0;

	newMat.data[4] = 0;
	newMat.data[5] = 2 / (top - bottom);
	newMat.data[6] = 0;
	newMat.data[7] = 0;

	newMat.data[8] = 0;
	newMat.data[9] = 0;
	newMat.data[10] = -1 / (farDist - nearDist);
	newMat.data[11] = 0;

	newMat.data[12] = -(right + left) / (right - left);
	newMat.data[13] = -(top + bottom) / (top - bottom);
	newMat.data[14] = -nearDist / (farDist - nearDist);
	newMat.data[15] = 1;

	return newMat;
}


float Matrix4::Determinant()
{
    return (  (data[0] *  data[5]  - data[4]  * data[1])
            * (data[10] * data[15] - data[14] * data[11])
            - (data[0] *  data[9]  - data[8]  * data[1])
            * (data[6] *  data[15] - data[14] * data[7])
            + (data[0] *  data[13] - data[12] * data[1])
            * (data[6] *  data[11] - data[10] * data[7])
            + (data[4] *  data[9]  - data[8]  * data[5])
            * (data[2] *  data[15] - data[14] * data[3])
            - (data[4] *  data[13] - data[12] * data[5])
            * (data[2] *  data[11] - data[10] * data[3])
            + (data[8] *  data[13] - data[12] * data[9])
            * (data[2] *  data[7]  - data[6]  * data[3]));
}

Matrix4 Matrix4::Inverse()
{
    Matrix4 tmp;
    float d = Determinant();
    
    if ( std::abs(d) < 0.001f )
    {
        return tmp;
    }
    else
    {
        d = 1.f / d;
        tmp.data[0] = d * (  data[5] * (data[10] * data[15] - data[14] * data[11])
                           + data[9] * (data[14] * data[7] - data[6] * data[15])
                           + data[13] * (data[6] * data[11] - data[10] * data[7]));
        
        tmp.data[4] = d * (  data[6] * (data[8] * data[15] - data[12] * data[11])
                           + data[10] * (data[12] * data[7] - data[4] * data[15])
                           + data[14] * (data[4] * data[11] - data[8] * data[7]));
        
        tmp.data[8] = d * (  data[7] * (data[8] * data[13] - data[12] * data[9])
                           + data[11] * (data[12] * data[5] - data[4] * data[13])
                           + data[15] * (data[4] * data[9] - data[8] * data[5]));
        
        tmp.data[12] = d * (  data[4] * (data[13] * data[10] - data[9] * data[14])
                            + data[8] * (data[5] * data[14] - data[13] * data[6])
                            + data[12] * (data[9] * data[6] - data[5] * data[10]));
        
        tmp.data[1] = d * (  data[9] * (data[2] * data[15] - data[14] * data[3])
                           + data[13] * (data[10] * data[3] - data[2] * data[11])
                           + data[1] * (data[14] * data[11] - data[10] * data[15]));
        
        tmp.data[5] = d * (  data[10] * (data[0] * data[15] - data[12] * data[3])
                           + data[14] * (data[8] * data[3] - data[0] * data[11])
                           + data[2] * (data[12] * data[11] - data[8] * data[15]));
        
        tmp.data[9] = d * (  data[11] * (data[0] * data[13] - data[12] * data[1])
                           + data[15] * (data[8] * data[1] - data[0] * data[9])
                           + data[3] * (data[12] * data[9] - data[8] * data[13]));
        
        tmp.data[13] = d * (  data[8] * (data[13] * data[2] - data[1] * data[14])
                            + data[12] * (data[1] * data[10] - data[9] * data[2])
                            + data[0] * (data[9] * data[14] - data[13] * data[10]));
        
        tmp.data[2] = d * (  data[13] * (data[2] * data[7] - data[6] * data[3])
                           + data[1] * (data[6] * data[15] - data[14] * data[7])
                           + data[5] * (data[14] * data[3] - data[2] * data[15]));
        
        tmp.data[6] = d * (  data[14] * (data[0] * data[7] - data[4] * data[3])
                           + data[2] * (data[4] * data[15] - data[12] * data[7])
                           + data[6] * (data[12] * data[3] - data[0] * data[15]));
        
        tmp.data[10] = d * (  data[15] * (data[0] * data[5] - data[4] * data[1])
                            + data[3] * (data[4] * data[13] - data[12] * data[5])
                            + data[7] * (data[12] * data[1] - data[0] * data[13]));
        
        tmp.data[14] = d * (  data[12] * (data[5] * data[2] - data[1] * data[6])
                            + data[0] * (data[13] * data[6] - data[5] * data[14])
                            + data[4] * (data[1] * data[14] - data[13] * data[2]));
        
        tmp.data[3] = d * (  data[1] * (data[10] * data[7] - data[6] * data[11])
                           + data[5] * (data[2] * data[11] - data[10] * data[3])
                           + data[9] * (data[6] * data[3] - data[2] * data[7]));
        
        tmp.data[7] = d * (  data[2] * (data[8] * data[7] - data[4] * data[11])
                           + data[6] * (data[0] * data[11] - data[8] * data[3])
                           + data[10] * (data[4] * data[3] - data[0] * data[7]));
        
        tmp.data[11] = d * (  data[3] * (data[8] * data[5] - data[4] * data[9])
                            + data[7] * (data[0] * data[9] - data[8] * data[1])
                            + data[11] * (data[4] * data[1] - data[0] * data[5]));
        
        tmp.data[15] = d * (  data[0] * (data[5] * data[10] - data[9] * data[6])
                            + data[4] * (data[9] * data[2] - data[1] * data[10])
                            + data[8] * (data[1] * data[6] - data[5] * data[2]));

        return tmp;
    }
    
}


Vector3 Matrix4::GetTranslation()
{
    return Vector3(data[3], data[7], data[11]);
}

Vector3 Matrix4::GetScale()
{
    return Vector3(data[0], data[5], data[10]);
}

Quaternion Matrix4::GetRotation()
{
    float s, t;
    
    if ( data[0*4 + 0] + data[1*4 + 1] + data[2*4 + 2] > 0.00000001f)
    {
        t = data[0*4 + 0] + data[1*4 + 1] + data[2*4 + 2] + 1.0f;
        s = 0.5f/std::sqrt(t);
        
        return Quaternion(
                           s*t,
                           (data[1*4 + 2] - data[2*4 + 1])*s,
                           (data[2*4 + 0] - data[0*4 + 2])*s,
                           (data[0*4 + 1] - data[1*4 + 0])*s
                          );
    }
    else if ( data[0*4 + 0] > data[1*4 + 1] && data[0*4 + 0] > data[2*4 + 2])
    {
        t = data[0*4 + 0] - data[1*4 + 1] - data[2*4 + 2] + 1.0f;
        s = 0.5f/std::sqrt(t);
        
        return Quaternion(
                           (data[1*4 + 2] - data[2*4 + 1])*s,
                           s*t,
                           (data[0*4 + 1] + data[1*4 + 0])*s,
                           (data[2*4 + 0] + data[0*4 + 2])*s
                          );
    }
    else if ( data[1*4 + 1] > data[2*4 + 2] )
    {
        t = -data[0*4 + 0] + data[1*4 + 1] - data[2*4 + 2] + 1.0f;
        s = 0.5f/std::sqrt(t);
        
        return Quaternion(
                           (data[2*4 + 0] - data[0*4 + 2])*s,
                           (data[0*4 + 1] + data[1*4 + 0])*s,
                           s*t,
                           (data[1*4 + 2] + data[2*4 + 1])*s
                          );
    }
    else
    {
        t = -data[0*4 + 0] - data[1*4 + 1] + data[2*4 + 2] + 1.0f;
        s = 0.5f/std::sqrt(t);
        
        return Quaternion(
                       (data[0*4 + 1] - data[1*4 + 0])*s,
                       (data[2*4 + 0] + data[0*4 + 2])*s,
                       (data[1*4 + 2] + data[2*4 + 1])*s,
                       s*t
                   );
    }
}