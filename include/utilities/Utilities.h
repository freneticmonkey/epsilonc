#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <stdarg.h>
#include <time.h>
#include <fstream>

namespace epsilon
{
	// TODO: Not sure if this should be an inline, but for now...
	inline std::string string_format(const std::string fmt, ...) {
		int size=100;
		std::string str;
		va_list ap;
		while (1) {
			str.resize(size);
			va_start(ap, fmt);
			//int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
			int n = _vscprintf(fmt.c_str(), ap) + 1;
			va_end(ap);
			if (n > -1 && n < size) {
				str.resize(n);
				return str;
			}
			if (n > -1)
				size=n+1;
			else
				size*=2;
		}
	}

	inline double diffclock(clock_t clock1,clock_t clock2) 
	{ 
		double diffticks=clock1-clock2; 
		double diffms=(diffticks*1000)/CLOCKS_PER_SEC; 
		return diffms; 
	}

	inline std::string readfile(std::string filename)
	{
		/*
		ifstream fileStream(filename);
		std::string fileContents;

		if ( fileStream.is_open() )
		{
			string line;
			while( getline(fileStream, line) )
			{
				fileContents += line;
			}
			fileStream.close();
		}
		return fileContents;
		*/

		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}

	class CycleValue
	{
	public:
		CycleValue()
		{
			timeValue = 0.0f;
			frequency = 100;
			degrees = 0;
		}
		~CycleValue() {}

		CycleValue& SetFrequency(int freq)
		{
			frequency = freq;
			return *this;
		}

		double GetValue(float el)
		{
			timeValue += el;
			if ( timeValue > (1.0f / frequency) )
			{
				timeValue = 0.0f;
				degrees++;
			}
			if ( degrees > 360 )
			{
				degrees = 0;
			}
			return cos( (degrees * 3.141529) / 180.0f);
		}

	private:
		float	timeValue;
		int		frequency;
		int		degrees;
	};
}