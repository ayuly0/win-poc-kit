#include "base_utils.h"
#include "logger.h"
#include <iostream>

using namespace Base;

int main( int argc, char *argv[] )
{
    const char *comp     = "LOADER";
    const char *dll_name = ( argc > 1 ) ? argv[ 1 ] : "poc.dll";

    LOG_INFO( comp, "Loader Initialized." );
    LOG_INFO( comp, "Attempting to load: {}", dll_name );

    HMODULE hPayload = LoadLibraryA( dll_name );
    if ( !hPayload )
    {
        LOG_FATAL( comp, "Failed to load {}. Error: 0x{:08x}", dll_name, GetLastError() );
        return 1;
    }

    LOG_SUCCESS( comp, "Successfully loaded {} at {:p}", dll_name, ( void * )hPayload );

    // TODO: Add logic to call specific exports if needed

    std::cout << "\nPress Enter to unload and exit..." << std::endl;
    std::cin.get();

    FreeLibrary( hPayload );
    LOG_INFO( comp, "Payload unloaded." );

    return 0;
}
