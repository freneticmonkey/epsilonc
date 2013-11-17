#pragma once
#include <string>

#include "logging/Logging.h"

namespace epsilon
{

	class PythonLogListener
	{
	public:
		PythonLogListener() : prefix("python:") {}
		PythonLogListener(std::string type) : prefix("python:")
		{
			if (type.size() > 0 )
			{
				prefix = std::string("python:") + type + ": ";
			}
		}
		~PythonLogListener() {}

		void OutputLog(const std::string &out)
		{
			Log(prefix, out);
		}
	private:
		std::string prefix;
	};
}