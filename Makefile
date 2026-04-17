# PoC Kit Builder - Windows Version

ARCH   ?= x64
CONFIG ?= Debug
NAME   ?= poc
TYPE   ?= EXE

# Mapping ARCH to target directory used in CMakeLists.txt (lines 68-82)
# x86 maps to Win32 as per standard MSVC conventions in your CMakeLists
ifeq ($(ARCH),x86)
    BIN_ARCH = Win32
else
    BIN_ARCH = x64
endif

BUILD_DIR = build\$(ARCH)-$(CONFIG)
BIN_DIR   = bin\$(BIN_ARCH)\$(CONFIG)

.PHONY: all build clean run run-exe run-loader run-elf build-all

all: build

build:
	@cmake -E make_directory $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..\..\ -DARCH=$(ARCH) -DCMAKE_BUILD_TYPE=$(CONFIG) -DPOC_NAME=$(NAME) -DPROJECT_TYPE=$(TYPE) -G Ninja
	@cmake --build $(BUILD_DIR)
	-@cmake -E copy $(BUILD_DIR)\compile_commands.json .

build-all:
	@$(MAKE) ARCH=x64 TYPE=EXE build
	@$(MAKE) ARCH=x64 TYPE=DLL build
	@$(MAKE) ARCH=x64 TYPE=LOADER build

clean:
	@cmake -E rm -rf build bin

run-exe:
	@if exist $(BIN_DIR)\$(NAME).exe ( $(BIN_DIR)\$(NAME).exe ) else ( echo [!] Executable not found: $(BIN_DIR)\$(NAME).exe )

run-loader:
	@if exist $(BIN_DIR)\loader.exe ( $(BIN_DIR)\loader.exe $(BIN_DIR)\$(NAME).dll ) else ( echo [!] Loader not found: $(BIN_DIR)\loader.exe )

run:
ifeq ($(TYPE),EXE)
	@$(MAKE) run-exe
else ifeq ($(TYPE),LOADER)
	@$(MAKE) run-loader
else
	@echo [!] Cannot run DLL directly. Use TYPE=LOADER to test your payload.
endif
