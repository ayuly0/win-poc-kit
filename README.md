# Windows PoC Research Starter

**FOR AUTHORIZED SECURITY RESEARCH ONLY.**

A minimalist C++/ASM framework for rapid multi-architecture Windows PoC development and system state visualization.

## Logging / Stack / Memory
- `LOG_MEM(comp, addr, sz, name)`: Memory region hex dump with boxed header.
- `LOG_STACK(comp, lines)`: Automated capture and dump of current stack frame.
- `LOG_CALLS(comp, depth)`: Captures return address backtrace (CaptureStackBackTrace).
- `LOG_REGS(comp, ctx)`: Serialized CPU register state dump.
- `LOG_DUMP(comp, ptr, sz)`: Multi-line hex dump with ASCII representation.

## Utilities / Types
- `u8`, `u16`, `u32`, `u64`: Shorthand technical types.
- `addr_t`, `ptr_t`, `byte_t`: Technical aliases for pointers and bytes.
- `WinHandle`: RAII management for Windows HANDLE objects.
- `MEM_READ` / `MEM_WRITE`: Macros for direct memory access.
- `M_ALLOC` / `M_FREE`: Virtual memory allocation shortcuts.
- `nt.h`: Bare-metal definitions for PEB, TEB, and LDR structures.

## Build System

| Task | Command | Optional Flags |
| :--- | :--- | :--- |
| **Build** | `make build` | `NAME=poc ARCH=x64\|x86 CONFIG=Debug\|Release` |
| **Run** | `make run` | `ARCH=x64 CONFIG=Debug` |
| **Clean** | `make clean` | — |

*Note: Ninja generator is used to provide `compile_commands.json` in the root for LSP synchronization.*
