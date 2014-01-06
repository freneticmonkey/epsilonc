#include "EpsilonCore.h"

#include "core/EpsilonManager.h"

//#include <memory>

using namespace epsilon;

//#ifdef _WIN32

int main() 
{
	EpsilonManager em;
	em.Setup();
	em.Run();

	return 0;
}
/*
#endif
#ifdef TARGET_OS_X
#endif
*/