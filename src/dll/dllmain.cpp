#include "base_utils.h"
#include "logger.h"

using namespace Base;

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
    UNREFERENCED_PARAMETER( hModule );
    UNREFERENCED_PARAMETER( lpReserved );

    const char *comp = "DLL";

    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
        LOG_SUCCESS( comp, "Payload attached to process." );
        LOG_INFO( comp, "Base Address: {:p}", ( void * )hModule );
        
        // TODO: Implement DLL payload logic here
        
        break;

    case DLL_PROCESS_DETACH:
        LOG_INFO( comp, "Payload detached." );
        break;
    }
    return TRUE;
}
