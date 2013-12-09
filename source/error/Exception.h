#include <exception>
#include <string>
#include <ostream>
#include "utilities/Utilities.h"

namespace epsilon
{

	class BoundaryException : public std::exception
	{
	public:
		std::string msg;
	
		BoundaryException(std::string className, int access, int max)
		{
			msg = Format("%s: Boundary error. Max: %d Accessed: %d", className.c_str(), max, access);
		}

		virtual const char* what() const throw()
		{
			return msg.c_str();
		}
	};
}