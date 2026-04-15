#ifndef BASE_UTILS_H
#define BASE_UTILS_H

#include "logger.h"
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <tlhelp32.h>
#include <vector>
#include <windows.h>

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using addr_t = uintptr_t;
using ptr_t  = void *;
using byte_t = unsigned char;

namespace Base::Utils
{
    // RAII for Windows Handles
    struct HandleDeleter
    {
        void operator()( HANDLE h ) const
        {
            if ( h && h != INVALID_HANDLE_VALUE )
            {
                CloseHandle( h );
            }
        }
    };
    using WinHandle = std::unique_ptr<void, HandleDeleter>;

// Memory Access Macros
#define MEM_READ( addr, type )       ( *( type * )( addr ) )
#define MEM_WRITE( addr, type, val ) ( *( type * )( addr ) = ( val ) )

#define M_ALLOC( size ) VirtualAlloc( NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE )
#define M_FREE( ptr )   VirtualFree( ptr, 0, MEM_RELEASE )

// Error Checking Macros
#define EXPECT( cond, comp, ... )                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if ( !( cond ) )                                                                                               \
        {                                                                                                              \
            LOG_FATAL( comp, "Expectation failed: " __VA_ARGS__ );                                                     \
            return;                                                                                                    \
        }                                                                                                              \
    } while ( 0 )

#define CHECK_WIN32( call, comp )                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if ( !( call ) )                                                                                               \
        {                                                                                                              \
            DWORD err = GetLastError();                                                                                \
            LOG_ERROR( comp, "{} failed with error 0x{:08x}", #call, err );                                            \
        }                                                                                                              \
    } while ( 0 )

#define CHECK_NT( call, comp )                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        NTSTATUS status = ( call );                                                                                    \
        if ( status != 0 )                                                                                             \
        {                                                                                                              \
            LOG_ERROR( comp, "{} failed with status 0x{:08x}", #call, status );                                        \
        }                                                                                                              \
    } while ( 0 )

    // Helpers
    DWORD GetProcessIdByName( std::wstring_view name );
    PVOID GetModuleBase( DWORD pid, std::wstring_view name );
} // namespace Base::Utils

#endif
