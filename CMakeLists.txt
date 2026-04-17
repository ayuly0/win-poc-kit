cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ─── Project Configuration ───
if(NOT POC_NAME)
    set(POC_NAME "poc")
endif()

# PROJECT_TYPE can be: EXE, DLL, LOADER
if(NOT PROJECT_TYPE)
    set(PROJECT_TYPE "EXE")
endif()
string(TOUPPER ${PROJECT_TYPE} PROJECT_TYPE)

project(${POC_NAME} LANGUAGES C CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ─── Source Discovery ───
# Shared utilities
file(GLOB COMMON_SOURCES src/common/*.cpp)

# Target-specific entry point
if(PROJECT_TYPE STREQUAL "EXE")
    set(TARGET_SOURCES src/exe/main.cpp)
    set(TARGET_KIND "EXECUTABLE")
    set(ACTUAL_NAME ${POC_NAME})
elseif(PROJECT_TYPE STREQUAL "DLL")
    set(TARGET_SOURCES src/dll/dllmain.cpp)
    set(TARGET_KIND "LIBRARY_SHARED")
    set(ACTUAL_NAME ${POC_NAME})
elseif(PROJECT_TYPE STREQUAL "LOADER")
    set(TARGET_SOURCES src/loader/main.cpp)
    set(TARGET_KIND "EXECUTABLE")
    set(ACTUAL_NAME "loader")
else()
    message(FATAL_ERROR "Invalid PROJECT_TYPE: ${PROJECT_TYPE}. Use EXE, DLL, or LOADER.")
endif()

# ASM support
file(GLOB ASM_FILES asm/*.asm)

# ─── Target Definition ───
if(TARGET_KIND STREQUAL "EXECUTABLE")
    add_executable(${ACTUAL_NAME} ${TARGET_SOURCES} ${COMMON_SOURCES} ${ASM_FILES})
else()
    add_library(${ACTUAL_NAME} SHARED ${TARGET_SOURCES} ${COMMON_SOURCES} ${ASM_FILES})
endif()

# ─── Compiler & Linker Settings ───
if(MSVC)
    target_compile_options(${ACTUAL_NAME} PRIVATE 
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:/W4 /WX /permissive- /Zc:preprocessor /wd5105>
    )
    
    target_link_options(${ACTUAL_NAME} PRIVATE /INCREMENTAL:NO)
    
    # Disable SAFESEH for x86 builds
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_link_options(${ACTUAL_NAME} PRIVATE /SAFESEH:NO)
    endif()
endif()

target_link_libraries(${ACTUAL_NAME} PRIVATE dbghelp.lib)
target_include_directories(${ACTUAL_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ─── Output Infrastructure ───
set_target_properties(${ACTUAL_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Release"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Debug"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Release"
)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set_target_properties(${ACTUAL_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/Win32/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/Win32/Release"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/Win32/Debug"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/Win32/Release"
    )
endif()
