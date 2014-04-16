#include <exception>
#include <string>
#include <ostream>
#include "utilities/Utilities.h"

#include <boost/format.hpp>

using namespace boost;

namespace epsilon
{

	class BoundaryException : public std::exception
	{
	public:
		std::string msg;
	
		BoundaryException(std::string className, int access, int max)
		{
            msg = boost::str(format("%s: Boundary error. Max: %d Accessed: %d") % className % max % access );
		}
        
        virtual ~BoundaryException() throw() {}

		const char* what() const throw()
		{
			return msg.c_str();
		}
	};
}