#include "EpsilonCore.h"

#include "core/EpsilonManager.h"

//#include <memory>

using namespace epsilon;

int main() 
{
	EpsilonManager em;
	em.Setup();
	em.Run();

	return 0;
}