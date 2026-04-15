#include "base_utils.h"

namespace Base::Utils
{
    DWORD GetProcessIdByName( std::wstring_view name )
    {
        DWORD pid = 0;
        WinHandle snapshot( CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ) );
        if ( snapshot.get() != INVALID_HANDLE_VALUE )
        {
            PROCESSENTRY32W entry;
            entry.dwSize = sizeof( entry );
            if ( Process32FirstW( snapshot.get(), &entry ) )
            {
                do
                {
                    if ( name == entry.szExeFile )
                    {
                        pid = entry.th32ProcessID;
                        break;
                    }
                } while ( Process32NextW( snapshot.get(), &entry ) );
            }
        }
        return pid;
    }

    PVOID GetModuleBase( DWORD pid, std::wstring_view name )
    {
        PVOID base = nullptr;
        WinHandle snapshot( CreateToolhelp32Snapshot( TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid ) );
        if ( snapshot.get() != INVALID_HANDLE_VALUE )
        {
            MODULEENTRY32W entry;
            entry.dwSize = sizeof( entry );
            if ( Module32FirstW( snapshot.get(), &entry ) )
            {
                do
                {
                    if ( name == entry.szModule )
                    {
                        base = entry.modBaseAddr;
                        break;
                    }
                } while ( Module32NextW( snapshot.get(), &entry ) );
            }
        }
        return base;
    }
} // namespace Base::Utils
