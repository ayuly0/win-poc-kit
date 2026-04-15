#ifndef BASE_NT_H
#define BASE_NT_H

#include <windows.h>
#include <winternl.h>

// Undocumented Structures for PoC Research

typedef struct _UNICODE_STRING_BASE
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING_BASE, *PUNICODE_STRING_BASE;

typedef struct _PEB_LDR_DATA_BASE
{
    BYTE Reserved1[ 8 ];
    PVOID Reserved2[ 3 ];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA_BASE, *PPEB_LDR_DATA_BASE;

typedef struct _LDR_DATA_TABLE_ENTRY_BASE
{
    PVOID Reserved1[ 2 ];
    LIST_ENTRY InMemoryOrderLinks;
    PVOID Reserved2[ 2 ];
    PVOID DllBase;
    PVOID EntryPoint;
    PVOID Reserved3;
    UNICODE_STRING FullDllName;
    BYTE Reserved4[ 8 ];
    PVOID Reserved5[ 3 ];
    union {
        ULONG CheckSum;
        PVOID Reserved6;
    } DUMMYUNIONNAME;
    ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY_BASE, *PLDR_DATA_TABLE_ENTRY_BASE;

typedef struct _RTL_USER_PROCESS_PARAMETERS_BASE
{
    BYTE Reserved1[ 16 ];
    PVOID Reserved2[ 10 ];
    UNICODE_STRING_BASE ImagePathName;
    UNICODE_STRING_BASE CommandLine;
} RTL_USER_PROCESS_PARAMETERS_BASE, *PRTL_USER_PROCESS_PARAMETERS_BASE;

typedef struct _PEB_BASE
{
    BYTE Reserved1[ 2 ];
    BYTE BeingDebugged;
    BYTE Reserved2[ 1 ];
    PVOID Reserved3[ 2 ];
    PPEB_LDR_DATA_BASE Ldr;
    PRTL_USER_PROCESS_PARAMETERS_BASE ProcessParameters;
    BYTE Reserved4[ 104 ];
    PVOID Reserved5[ 52 ];
    PVOID PostProcessInitRoutine;
    BYTE Reserved6[ 128 ];
    PVOID Reserved7[ 1 ];
    ULONG SessionId;
} PEB_BASE, *PPEB_BASE;

// Minimal TEB for stack research
typedef struct _TEB_BASE
{
    PVOID Reserved1[ 12 ];
    PPEB_BASE ProcessEnvironmentBlock;
    PVOID Reserved2[ 399 ];
    BYTE Reserved3[ 1952 ];
    PVOID TlsSlots[ 64 ];
    BYTE Reserved4[ 8 ];
    PVOID Reserved5[ 26 ];
    PVOID ReservedForOle;
    PVOID Reserved6[ 4 ];
    PVOID TlsExpansionSlots;
} TEB_BASE, *PTEB_BASE;

#endif
