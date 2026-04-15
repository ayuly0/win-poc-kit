cmake_minimum_required(VERSION 3.15)

# Set the project name dynamically from the Makefile if provided
if(NOT POC_NAME)
    set(POC_NAME "StackSpoofing")
endif()

project(${POC_NAME} LANGUAGES C CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB ASM_FILES asm/*.asm)

# Executable target
add_executable(${POC_NAME} ${SOURCE_FILES} ${ASM_FILES})

# Compiler and Linker Settings
if(MSVC)
    # Target-specific compile options to avoid leaking flags to MASM
    target_compile_options(${POC_NAME} PRIVATE 
        $<$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>:/W4 /WX /permissive- /Zc:preprocessor /wd5105>
    )
    
    target_link_options(${POC_NAME} PRIVATE /INCREMENTAL:NO)
    
    # Disable SAFESEH for x86 builds
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        target_link_options(${POC_NAME} PRIVATE /SAFESEH:NO)
    endif()
endif()

# Global libraries
target_link_libraries(${POC_NAME} PRIVATE dbghelp.lib)

# Include directories
target_include_directories(${POC_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Multi-Architecture Output Directory Structure
set_target_properties(${POC_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Release"
)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set_target_properties(${POC_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/Win32/Debug"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/Win32/Release"
    )
endif()
