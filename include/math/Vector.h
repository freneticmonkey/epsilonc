#pragma once

class Vector2
{
public:
	float x;
	float y;

	// Constructors
	Vector2();
	Vector2(const Vector2& vec);
	Vector2(float ix, float iy);

	// Assignment
	Vector2& operator= (const Vector2& vec);

	// Array Access
	float &operator[](int i);

	// Comparison
	inline bool operator==(const Vector2& other) const;
	inline bool operator!=(const Vector2& other) const;
	
	// Arithmetic operations.
	inline Vector2 operator+ (const Vector2& other) const;
	inline Vector2 operator- (const Vector2& other) const;

	inline friend Vector2 operator* (const Vector2& op1, const float scalar);
	inline friend Vector2 operator* (const float scalar, const Vector2& op1);
	
	inline Vector2 operator/ (const float scalar) const;
	inline Vector2 operator- () const;

	// Arithmetic updates.
    inline Vector2& operator+= (const Vector2& other);
    inline Vector2& operator-= (const Vector2& other);
    inline Vector2& operator*= (float scalar);
    inline Vector2& operator/= (float scalar);

	inline float Length() const;
	inline float LengthSquared() const;
	inline float Dot(const Vector2& other) const;
	inline float Normalise();
	inline Vector2 Normalised() const;

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
	float x;
	float y;
	float z;
	
	// Constructors
	Vector3();
	Vector3(const Vector3& vec);
	Vector3(float ix, float iy, float iz);

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

	// Comparison
    /*
	inline bool operator==(const Vector3& other) const;
	inline bool operator!=(const Vector3& other) const;
	
	// Arithmetic operations.
	inline Vector3 operator+ (const Vector3& other) const;
	inline Vector3 operator- (const Vector3& other) const;

	inline friend Vector3 operator* (const Vector3& op1, const float scalar);
	inline friend Vector3 operator* (const float scalar, const Vector3& op1);
	
	inline Vector3 operator/ (const float scalar) const;
	inline Vector3 operator- () const;
     */
    
    inline bool operator== (const Vector3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
    
    inline bool operator!= (const Vector3& other) const
    {
        return x != other.x  || y != other.y || z != other.z;
    }
    
    inline Vector3 operator+ (const Vector3& other) const
    {
        return Vector3( x + other.x ,
                        y + other.y ,
                        z + other.z);
    }
    
    inline Vector3 operator- (const Vector3& other) const
    {
        return Vector3( x - other.x ,
                        y - other.y ,
                        z - other.z );
    }
    
    // Implemented operator* functions as friends so that operators can appear
    // in either order vec3 * float , float * vec3
    inline Vector3 operator* (const Vector3& op1)//, const float scalar)
    {
        return Vector3 ( x * op1.x,
                         y * op1.y,
                         z * op1.z );
    }
    
    inline Vector3 friend operator* (Vector3 vec, Vector3 const &op1)
    {
        return Vector3( op1.x * vec.x, op1.y * vec.y, op1.z * vec.z);
    }
    
    inline Vector3 operator* (const float scalar)//, const Vector3& op1)
    {
        return Vector3 ( x * scalar,
                         y * scalar,
                         z * scalar);
    }
    
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
    inline Vector3 operator- () const
    {
        return Vector3( -x, 
                        -y,
                        -z );
    }
    
	// Arithmetic updates.
    //inline Vector3& operator+= (const Vector3& other);
    //inline Vector3& operator-= (const Vector3& other);
    //inline Vector3& operator*= (float scalar);
    //inline Vector3& operator/= (float scalar);
    
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

	inline float Length() const;
	inline float LengthSquared() const;
	inline float Dot(const Vector3& other) const;
	inline float Normalise();
	inline Vector3 Normalised() const;

	Vector3 Cross(const Vector3& other) const;
	Vector3 UnitCross(const Vector3& other) const;

	Vector3 Reflect(const Vector3& normal) const;

	Vector3 RotateAround(const Vector3& axis, float theta);

	inline float Distance(const Vector3& other) const;

	inline float Angle(const Vector3& other) const;

	Vector3 Project(const Vector3& other) const;


	// Class Functions
	static void ComputeExtremes(int numVectors, const Vector3* vectors,
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
	Vector4();
	Vector4(const Vector4& vec);
	Vector4(float ix, float iy, float iz, float iw);
    
	// Assignment
	Vector4& operator= (const Vector4& vec);
    
	// Array Access
	float &operator[](int i);
    
	// Comparison
	inline bool operator==(const Vector4& other) const;
	inline bool operator!=(const Vector4& other) const;
	
	// Arithmetic operations.
	inline Vector4 operator+ (const Vector4& other) const;
	inline Vector4 operator- (const Vector4& other) const;
    
	inline friend Vector4 operator* (const Vector4& op1, const float scalar);
	inline friend Vector4 operator* (const float scalar, const Vector4& op1);
	
	inline Vector4 operator/ (const float scalar) const;
	inline Vector4 operator- () const;
    
	// Arithmetic updates.
    inline Vector4& operator+= (const Vector4& other);
    inline Vector4& operator-= (const Vector4& other);
    inline Vector4& operator*= (float scalar);
    inline Vector4& operator/= (float scalar);
    
	inline float Length() const;
	inline float LengthSquared() const;
	inline float Dot(const Vector4& other) const;
	inline float Normalise();
	inline Vector4 Normalised() const;
    
	// Class Functions
	static void ComputeExtremes(int numVectors, const Vector4* vectors,
								Vector4& vmin, Vector4& vmax);
};