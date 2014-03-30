#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <stdarg.h>
#include <time.h>
#include <fstream>
#include <errno.h>
#include <stdarg.h>

// For string split
#include <vector>
#include <sstream>

#ifdef _MSC_VER
#include <windows.h>
#endif

namespace epsilon
{
	// TODO: Not sure if this should be an inline, but for now...
	inline std::string Format(const std::string fmt, ...) {

		int size=1024;
		std::string str;

		va_list ap;
        

		while (1) {
			str.resize(size);
			va_start(ap, fmt);
			//int n = vsnprintf((char *)str.c_str(), size, fmt.c_str(), ap);
            
#ifdef __APPLE__
			int n = 0;
#else
			int n = _vsnprintf_s((char *)str.c_str(), size, size, fmt.c_str(), ap);
#endif
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
		std::ifstream in(filename, std::ios::in | std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize((unsigned int)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		else
		{
			printf("File not found: %s", filename.c_str());
			return "";
		}
		throw(errno);
	}


	/*
	 * String split functions because the damned std::string class doesn't have it? hmm
	 */

	// Grabbed from http://stackoverflow.com/a/236803
	// Skips empty tokens.
	inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			if (!item.empty())
				elems.push_back(item);
		}
		return elems;
	}

	inline std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}


	/*
	 * Helper class to easily generate cyclical values
	 */
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