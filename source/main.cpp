#include "EpsilonCore.h"

#include "core/EpsilonManager.h"
#include <string>

using namespace epsilon;

int main(int argc, char* argv[])
{
    EpsilonManager em;
	
    // Check if a basepath has been provided.
    if ( argc > 1)
    {
        // Second arg
        std::string bp(argv[1]);
        if ( bp.size() > 0 )
        {
            em.SetBasePath(bp);
        }
    }
    
    em.Setup();
	em.Run();

	return 0;
}