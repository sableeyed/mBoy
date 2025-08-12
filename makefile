# Set default linker type to 'dynamic' (you can change this to 'static' by default if desired)
LINK_TYPE ?= dynamic

CC ?= gcc
CFLAGS = -Wall -g -I $(INC)

SRC = src
INC = include
BUILD = build
BIN = bin

SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(SRCS))

TARGET := $(BIN)/masochistboy

# Auto-detect target OS or manually set TARGET_OS
ifeq ($(OS),Windows_NT)
    TARGET_OS := windows
else
    TARGET_OS := linux
endif

# Adjust settings based on OS
ifeq ($(TARGET_OS),windows)
    EXE_EXT := .exe
    MKDIR_CMD = if not exist $(BUILD) mkdir $(BUILD) & if not exist $(BIN) mkdir $(BIN)
    RM_CMD = if exist $(BUILD) del /F /Q $(BUILD)\* & if exist $(BIN) del /F /Q $(BIN)\*.exe
    CFLAGS += -DWINDOWS_BUILD
    CFLAGS += -IC:\sdl3\include #replace with the path to your SDL3 installation
    #CFLAGS += -mwindows

    ifeq ($(LINK_TYPE), static)
        LDFLAGS = C:\sdl3\lib\libSDL3.a -static -lmingw32 -limm32 -lole32 -lwinmm -ldxguid \
            -lsetupapi -lversion -loleaut32 -luuid -lcfgmgr32 -lcomdlg32 # Static linking for Windows
    else
        LDFLAGS = -L C:\sdl3\lib -lSDL3 -lcomdlg32 # Dynamic linking for Windows
    endif
else
    EXE_EXT :=
    MKDIR_CMD = mkdir -p $(BUILD) $(BIN)
    RM_CMD = rm -rf $(BUILD)/* $(BIN)/*

    # WIP
    ifeq ($(LINK_TYPE), static)
        LDFLAGS = -L /path/to/sdl3/lib -lSDL3 -static # Static linking for Linux
    else
        LDFLAGS = -L /path/to/sdl3/lib -lSDL3 # Dynamic linking for Linux
    endif
endif

TARGET := $(BIN)/masochistboy$(EXE_EXT)

# Default target
all: $(TARGET)

# Build target executable
$(TARGET): $(OBJS)
	$(MKDIR_CMD)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files to object files
$(BUILD)/%.o: $(SRC)/%.c
	$(MKDIR_CMD)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	$(RM_CMD)
