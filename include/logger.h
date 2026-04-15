#ifndef BASE_LOGGER_H
#define BASE_LOGGER_H

#include <chrono>
#include <format>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <windows.h>

namespace Base
{
    enum class LogLevel : int
    {
        DEBUG    = 0,
        INFO     = 1,
        SUCCESS  = 2,
        WARN     = 3,
        ERR      = 4,
        CRITICAL = 5,
        NONE     = 6
    };

#ifdef NDEBUG
    static constexpr LogLevel CURRENT_LOG_LEVEL = LogLevel::INFO;
#else
    static constexpr LogLevel CURRENT_LOG_LEVEL = LogLevel::DEBUG;
#endif

    namespace Logger
    {
        namespace color
        {
            using namespace std::string_view_literals;

            constexpr auto RESET = "\033[0m"sv;
            constexpr auto BOLD  = "\033[1m"sv;
            constexpr auto DIM   = "\033[2m"sv;
            constexpr auto GREY  = "\033[90m"sv;

            constexpr auto YELLOW     = "\033[38;5;180m"sv;          // Muted Sand
            constexpr auto GREEN      = "\033[38;5;108m"sv;          // Sage
            constexpr auto BLUE       = "\033[38;5;103m"sv;          // Steel Blue
            constexpr auto CYAN       = "\033[38;5;109m"sv;          // Grey-Cyan
            constexpr auto PURPLE     = "\033[38;5;146m"sv;          // Dusty Lavender
            constexpr auto ORANGE     = "\033[38;5;173m"sv;          // Muted Clay
            constexpr auto DARK_GREEN = "\033[38;5;65m"sv;           // Moss
            constexpr auto BRIGHT_RED = "\033[38;5;131m"sv;          // Muted Rose
            constexpr auto BG_RED     = "\033[48;5;236;38;5;210m"sv; // Darker background with soft red text

            // UI Elements
            constexpr auto SEP = "\033[2m\033[90m │ \033[0m"sv; // Dim Grey Pipe
        } // namespace color

        inline std::string timestamp()
        {
            auto now = std::chrono::floor<std::chrono::seconds>( std::chrono::system_clock::now() );
            return std::format( "{:%H:%M:%S}", now );
        }

        inline std::string_view getLevelStr( LogLevel level )
        {
            switch ( level )
            {
            case LogLevel::DEBUG   : return "DEBUG";
            case LogLevel::INFO    : return "INFO ";
            case LogLevel::SUCCESS : return "OK   ";
            case LogLevel::WARN    : return "WARN ";
            case LogLevel::ERR     : return "ERROR";
            case LogLevel::CRITICAL: return "FATAL";
            default                : return "LOG  ";
            }
        }

        inline std::string_view getLevelColor( LogLevel level )
        {
            switch ( level )
            {
            case LogLevel::DEBUG   : return color::PURPLE;
            case LogLevel::INFO    : return color::BLUE;
            case LogLevel::SUCCESS : return color::GREEN;
            case LogLevel::WARN    : return color::YELLOW;
            case LogLevel::ERR     : return color::BRIGHT_RED;
            case LogLevel::CRITICAL: return color::BOLD;
            default                : return color::RESET;
            }
        }

        inline void internal_log( LogLevel level, std::string_view component, std::string_view message )
        {
            if ( level < CURRENT_LOG_LEVEL )
            {
                return;
            }

            // 1. Format Component (Strict 10-char Gutter)
            std::string comp = std::string( component );
            for ( auto &c : comp )
            {
                c = ( char )std::toupper( ( unsigned char )c );
            }
            if ( comp.length() > 10 )
            {
                comp = comp.substr( 0, 10 );
            }
            else
            {
                comp.append( 10 - comp.length(), ' ' );
            }

            // 2. Assemble Tactical HUD
            std::cout << color::GREY << timestamp() << color::SEP;

            if ( level == LogLevel::CRITICAL )
            {
                std::cout << color::BG_RED << color::BOLD << getLevelStr( level ) << color::RESET << color::SEP;
            }
            else
            {
                std::cout << color::BOLD << getLevelColor( level ) << getLevelStr( level ) << color::RESET
                          << color::SEP;
            }

            std::cout << color::BOLD << comp << color::SEP << message << "\n";
        }

        template <typename... Args> void debug( std::string_view comp, std::string_view fmt, Args &&...args )
        {
            internal_log( LogLevel::DEBUG, comp, std::vformat( fmt, std::make_format_args( args... ) ) );
        }

        template <typename... Args> void info( std::string_view comp, std::string_view fmt, Args &&...args )
        {
            internal_log( LogLevel::INFO, comp, std::vformat( fmt, std::make_format_args( args... ) ) );
        }

        template <typename... Args> void success( std::string_view comp, std::string_view fmt, Args &&...args )
        {
            internal_log( LogLevel::SUCCESS, comp, std::vformat( fmt, std::make_format_args( args... ) ) );
        }

        template <typename... Args> void warn( std::string_view comp, std::string_view fmt, Args &&...args )
        {
            internal_log( LogLevel::WARN, comp, std::vformat( fmt, std::make_format_args( args... ) ) );
        }

        template <typename... Args> void error( std::string_view comp, std::string_view fmt, Args &&...args )
        {
            internal_log( LogLevel::ERR, comp, std::vformat( fmt, std::make_format_args( args... ) ) );
        }

        template <typename... Args> void critical( std::string_view comp, std::string_view fmt, Args &&...args )
        {
            internal_log( LogLevel::CRITICAL, comp, std::vformat( fmt, std::make_format_args( args... ) ) );
        }

        // Overloads for no arguments
        inline void debug( std::string_view comp, std::string_view msg )
        {
            internal_log( LogLevel::DEBUG, comp, msg );
        }
        inline void info( std::string_view comp, std::string_view msg )
        {
            internal_log( LogLevel::INFO, comp, msg );
        }
        inline void success( std::string_view comp, std::string_view msg )
        {
            internal_log( LogLevel::SUCCESS, comp, msg );
        }
        inline void warn( std::string_view comp, std::string_view msg )
        {
            internal_log( LogLevel::WARN, comp, msg );
        }
        inline void error( std::string_view comp, std::string_view msg )
        {
            internal_log( LogLevel::ERR, comp, msg );
        }
        inline void critical( std::string_view comp, std::string_view msg )
        {
            internal_log( LogLevel::CRITICAL, comp, msg );
        }

        // ─── Research Utilities ───

        inline void hex_dump( std::string_view component, const void *data, size_t size )
        {
            if ( !data || size == 0 )
            {
                return;
            }
            const unsigned char *byte_data = static_cast<const unsigned char *>( data );

            info( component, "Hex Dump of {:p} ({} bytes):", data, size );

            for ( size_t i = 0; i < size; i += 16 )
            {
                std::stringstream ss;
                ss << color::GREY << std::format( "{:08x}", i ) << color::RESET << "  ";

                for ( size_t j = 0; j < 16; ++j )
                {
                    if ( i + j < size )
                    {
                        ss << std::format( "{:02x} ", byte_data[ i + j ] );
                    }
                    else
                    {
                        ss << "   ";
                    }
                }

                ss << " ";

                for ( size_t j = 0; j < 16; ++j )
                {
                    if ( i + j < size )
                    {
                        char c = ( char )byte_data[ i + j ];
                        ss << ( std::isprint( ( unsigned char )c ) ? c : '.' );
                    }
                }

                std::cout << color::GREY << timestamp() << color::SEP << color::BOLD << color::PURPLE << "DUMP "
                          << color::RESET << color::SEP << ss.str() << "\n";
            }
        }

        inline void reg_dump( std::string_view component, const CONTEXT &ctx )
        {
            info( component, "Register State Dump:" );
            auto print_reg = [ & ]( std::string_view name, DWORD64 val ) {
                std::cout << color::GREY << timestamp() << color::SEP << color::BOLD << color::CYAN << "REGS "
                          << color::RESET << color::SEP << color::BOLD << std::format( "{:4}", name ) << color::RESET
                          << ": " << color::YELLOW << std::format( "0x{:016x}", val ) << color::RESET << "\n";
            };

#ifdef _M_X64
            print_reg( "RAX", ctx.Rax );
            print_reg( "RBX", ctx.Rbx );
            print_reg( "RCX", ctx.Rcx );
            print_reg( "RDX", ctx.Rdx );
            print_reg( "RSI", ctx.Rsi );
            print_reg( "RDI", ctx.Rdi );
            print_reg( "R8", ctx.R8 );
            print_reg( "R9", ctx.R9 );
            print_reg( "R10", ctx.R10 );
            print_reg( "R11", ctx.R11 );
            print_reg( "R12", ctx.R12 );
            print_reg( "R13", ctx.R13 );
            print_reg( "R14", ctx.R14 );
            print_reg( "R15", ctx.R15 );
            print_reg( "RIP", ctx.Rip );
            print_reg( "RSP", ctx.Rsp );
            print_reg( "RBP", ctx.Rbp );
            print_reg( "EFLAGS", ctx.EFlags );
#else
            print_reg( "EAX", ctx.Eax );
            print_reg( "EBX", ctx.Ebx );
            print_reg( "ECX", ctx.Ecx );
            print_reg( "EDX", ctx.Edx );
            print_reg( "ESI", ctx.Esi );
            print_reg( "EDI", ctx.Edi );
            print_reg( "EIP", ctx.Eip );
            print_reg( "ESP", ctx.Esp );
            print_reg( "EBP", ctx.Ebp );
            print_reg( "EFLAGS", ctx.EFlags );
#endif
        }

        // ─── Snapshots ───

        inline void memory_snapshot( std::string_view component, const void *addr, size_t size, std::string_view name )
        {
            if ( !addr || size == 0 )
            {
                return;
            }

            std::cout << color::GREY << timestamp() << color::SEP << color::BOLD << color::YELLOW << "SNAP "
                      << color::RESET << color::SEP << color::BOLD << std::format( "{:10}", component ) << color::RESET
                      << color::SEP << "Snapshot: " << color::BOLD << name << color::RESET << " @ " << addr << " ["
                      << size << " bytes]\n";
            hex_dump( component, addr, size );
        }

        inline void stack_snapshot( std::string_view component, size_t lines )
        {
            CONTEXT ctx;
            RtlCaptureContext( &ctx );

            void *sp = nullptr;
#ifdef _M_X64
            sp = ( void * )ctx.Rsp;
#else
            sp = ( void * )ctx.Esp;
#endif
            memory_snapshot( component, sp, lines * 16, "Stack Frame" );
        }

        inline void call_snapshot( std::string_view component, size_t depth )
        {
            void *stack[ 64 ];
            DWORD frames = CaptureStackBackTrace( 1, ( DWORD )min( depth, ( size_t )64 ), stack, NULL );

            std::cout << color::GREY << timestamp() << color::SEP << color::BOLD << color::ORANGE << "CALLS"
                      << color::RESET << color::SEP << color::BOLD << std::format( "{:10}", component ) << color::RESET
                      << color::SEP << "Backtrace (" << frames << " frames):\n";

            for ( DWORD i = 0; i < frames; i++ )
            {
                std::cout << color::GREY << timestamp() << color::SEP << "      " << color::DIM << "[" << i << "] "
                          << color::RESET << stack[ i ] << "\n";
            }
        }

    } // namespace Logger
} // namespace Base

// ─── Fast Macros ───

#define LOG_DEBUG( comp, ... )    Base::Logger::debug( comp, __VA_ARGS__ )
#define LOG_INFO( comp, ... )     Base::Logger::info( comp, __VA_ARGS__ )
#define LOG_SUCCESS( comp, ... )  Base::Logger::success( comp, __VA_ARGS__ )
#define LOG_WARN( comp, ... )     Base::Logger::warn( comp, __VA_ARGS__ )
#define LOG_ERROR( comp, ... )    Base::Logger::error( comp, __VA_ARGS__ )
#define LOG_FATAL( comp, ... )    Base::Logger::critical( comp, __VA_ARGS__ )
#define LOG_DUMP( comp, ptr, sz ) Base::Logger::hex_dump( comp, ptr, sz )
#define LOG_REGS( comp, ctx )     Base::Logger::reg_dump( comp, ctx )

#define LOG_MEM( comp, addr, sz, name ) Base::Logger::memory_snapshot( comp, addr, sz, name )
#define LOG_STACK( comp, lines )        Base::Logger::stack_snapshot( comp, lines )
#define LOG_CALLS( comp, depth )        Base::Logger::call_snapshot( comp, depth )

#endif
