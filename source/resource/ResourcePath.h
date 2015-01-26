#pragma once
//
//  ResourcePath.h
//  Epsilon
//
//  Created by Scott Porter on 24/01/2015.
//

//#include "EpsilonCore.h"

#include <string>
#include <exception>
#include <boost/filesystem.hpp>

#include "logging/Logging.h"

using namespace boost;

namespace epsilon
{
	class ResourcePathException : public exception
	{
	public:
		ResourcePathException(std::string path = "") : invalidPath(path) {}
		virtual const char* what() const throw()
		{
			return (std::string("Invalid path to Resource: ") + invalidPath).c_str();
		}
	private:
		std::string invalidPath;
	};

	class ResourcePath
	{

	private:
		ResourcePath(void);

	public:
		static ResourcePath & GetInstance()
		{
			static ResourcePath instance;
			return instance;
		}
		~ResourcePath(void) {}

		void        SetBasePath(std::string basepath);

		std::string GetBasePath() { return resourcePath; }

		std::string GetResourceFullPath(std::string resourceRelativePath);

	private:
		std::string resourcePath;
		std::string resourceFolder;
	};
}
