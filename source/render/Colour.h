#pragma once

#pragma warning(disable: 4244) // For FromHex Colour Conversion (int->float)

#include <string>
#include <sstream>
#include <algorithm>
#include <list>

#include "math/Vector.h"

// Lexical cast for converting hex to int :)
// http://stackoverflow.com/a/1070499
template<typename T2, typename T1>
inline T2 lexical_cast(const T1 &in) 
{
    T2 out;
    std::stringstream ss;
    ss << in;
    ss >> out;
    return out;
}

class Colour
{
public:
	float r;
	float g;
	float b;
	float a;

	//Constructors

	// Defaults to white
	Colour();
	// Greyscale contructor
	Colour(float grey);
	// Standard Component Constructor
	Colour(float ir, float ig, float ib, float ia);
	
	// From Vectors
	Colour(const Vector3& other);
	Colour(const Vector4& other);

	// Assignment / Conversion
	Colour& operator=(const Colour& other);
	Colour& operator=(const Vector4& other);
	
	// Helper Function for Python because construtor overloading is problematic.
	static Colour FromVector4(const Vector4& other);
	
	Vector4 ToVector4();

	// Array Access
	float &operator[](int i);

	// Comparison
	inline bool operator==(const Colour& other) const
	{
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}

	inline bool operator!=(const Colour& other) const
	{
		return r != other.r || g != other.g || b != other.b || a == other.a;
	}

	// With Vector4
	inline bool operator==(const Vector4& other) const
	{
		return r == other.x && g == other.y && b == other.z && a == other.w;
	}

	inline bool operator!=(const Vector4& other) const
	{
		return r != other.x || g != other.y || b != other.z || a == other.w;
	}

	std::string ToString();

	// Transparency will default to 1.0
	static Colour FromHex(std::string hexStr);

	// Convert Colour to 0 - 255 Range
	void To8bit();

	std::list<float> ToList();

	void	Invert();
	Colour	Inverted();
	void	Tint(const Colour &other, float prop = 0.5);
	Colour  Tinted(const Colour &other, float prop = 0.5);

private:
	static float NormaliseColourValue(float value);
public:
	static const Colour RED;
	static const Colour GREEN;
	static const Colour BLUE;
	static const Colour YELLOW;
	static const Colour ORANGE;
	static const Colour CYAN;
	static const Colour PURPLE;
	static const Colour BLACK;
	static const Colour GREY;
};