#include <iostream>
#include <algorithm>

#include "render/Colour.h"
#include "utilities/Utilities.h"
#include "error/Exception.h"

using namespace epsilon;

Colour::Colour() : r(1.0f), g(1.0f), b(1.0f), a(1.0f)
{
}

Colour::Colour(float grey)
{
	grey = NormaliseColourValue(grey);

	r = grey; 
	g = grey; 
	b = grey; 
	a = grey; 
}

Colour::Colour(const Vector3& other)
{
	r = NormaliseColourValue(other.x);
	g = NormaliseColourValue(other.y);
	b = NormaliseColourValue(other.z);
	a = 1.0f;
}

Colour::Colour(const Vector4& other)
{
	r = NormaliseColourValue(other.x);
	g = NormaliseColourValue(other.y);
	b = NormaliseColourValue(other.z);
	a = NormaliseColourValue(other.w);
}

Colour::Colour(float ir, float ig, float ib, float ia)
{
	r = NormaliseColourValue(ir);
	g = NormaliseColourValue(ig);
	b = NormaliseColourValue(ib);
	a = NormaliseColourValue(ia);
}

// Assignment Operators
Colour& Colour::operator=(const Colour& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
	return *this;
}

Colour& Colour::operator=(const Vector4& other)
{
	r = other.x;
	g = other.y;
	b = other.z;
	a = other.w;
	return *this;
}

Colour Colour::FromVector4(const Vector4& other)
{
	return Colour(other.x, other.y, other.z, other.w);
}

Vector4 Colour::ToVector4()
{
	return Vector4(r, g, b, a);
}

float &Colour::operator[](int i)
{
    if ( i == 0 )
	{
		return r;
	}
	else if ( i == 1 )
	{
		return g;
	}
	else if ( i == 2 )
	{
		return b;
	}
	else if ( i == 3 )
	{
		return a;
	}
    else
    {
        throw BoundaryException("Colour", i, 4);
    }
}

std::string Colour::ToString()
{
	return Format("r: %3.2f g: %3.2f b: %3.2f a: %1.1f", r, g, b, a);
}

Colour Colour::FromHex(std::string hexStr)
{
	// If invalid hex string, just return white.
	if (hexStr.length() < 3)
		return Colour();

	// If short hand hex is used e.g. "FFF"
	if ( hexStr.length() == 3 )
	{
		// Expand it
		size_t i = 0;
		while (i < hexStr.length())
		{
			hexStr.insert(i, 2, hexStr.at(i));
			i+=3;
		}
	}

	// If the hex value isn't prefixed with an '0x'
	if ( hexStr.at(1) != 'x' )
	{
		// Add one
		hexStr = std::string("0x") + hexStr;
	}
	

	int hexValue = lexical_cast<int>(hexStr);
	
	return Colour(static_cast<float>(NormaliseColourValue(hexValue & 255)),
				  static_cast<float>(NormaliseColourValue((hexValue >> 8) & 255)), 
				  static_cast<float>(NormaliseColourValue((hexValue >> 16) & 255)), 
				  1.0f);
}

void Colour::To8bit()
{
	 r *= 255.0f;
	 g *= 255.0f;
	 b *= 255.0f;
	 a *= 255.0f;
}

std::list<float> Colour::ToList()
{
	float l[] = {r,g,b,a};
	return std::list<float>(l, l + sizeof(l) / sizeof(float) );
}

void Colour::Invert()
{
	r = 1.0f - r;
	g = 1.0f - g;
	b = 1.0f - b;
	a = 1.0f;
}

Colour Colour::Inverted()
{
	return Colour(1.0f - r,
				  1.0f - g,
				  1.0f - b,
				  1.0f);
}

void Colour::Tint(const Colour &other, float prop)
{
	r = (r * (1.0f - prop) + (other.r * prop) );
	g = (g * (1.0f - prop) + (other.g * prop) );
	b = (b * (1.0f - prop) + (other.b * prop) );
	a = (a * (1.0f - prop) + (other.a * prop) );
}


Colour Colour::Tinted(const Colour &other, float prop)
{
	return Colour( (r * (1.0f - prop) + (other.r * prop) ),
				   (g * (1.0f - prop) + (other.g * prop) ),
				   (b * (1.0f - prop) + (other.b * prop) ),
				   (a * (1.0f - prop) + (other.a * prop) ) );
}

float Colour::NormaliseColourValue(float value)
{
	value = std::max( std::min(value, 255.0f), 0.0f );

	if ( value > 1.0f )
	{
		value /= 255.0f;
	}
	return value;
}

const Colour Colour::RED(   1.f, 0.f,  0.f, 1.f);
const Colour Colour::GREEN( 0.f, 1.f,  0.f, 1.f);
const Colour Colour::BLUE(  0.f, 0.f,  1.f, 1.f);
const Colour Colour::YELLOW(1.f, 1.f,  0.f, 1.f);
const Colour Colour::ORANGE(1.f, 0.5f, 0.f, 1.f);
const Colour Colour::CYAN(  0.f, 1.f,  1.f, 1.f);
const Colour Colour::PURPLE(1.f, 0.f,  1.f, 1.f);
const Colour Colour::BLACK( 0.f, 0.f,  0.f, 1.f);
const Colour Colour::GREY(  0.5f,0.5f, 0.5f, 1.f);