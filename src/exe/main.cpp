#include "base_utils.h"
#include "logger.h"
#include "nt.h"
#include <iostream>

using namespace Base;

/**
 * Entry point for Proof-of-Concept research.
 * Utilize the Base::Logger and Base::Utils namespaces for tactical tools.
 */
int main()
{
    const char *comp = "CORE";
    LOG_INFO( comp, "PoC Initialized." );

    // TODO: Implement PoC logic here

    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}
