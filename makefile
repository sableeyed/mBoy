# Set default linker type to 'dynamic' (you can change this to 'static' by default if desired)
LINK_TYPE ?= dynamic

CC ?= gcc
CFLAGS = -Wall -g -I $(INC)

CXX ?= g++
CXXFLAGS = -Wall -g -I $(INC)

SRC = src
INC = include
BUILD = build
BIN = bin


SRCS := $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*.cpp)
OBJS := $(patsubst $(SRC)/%,$(BUILD)/%,$(SRCS:.c=.o))
OBJS := $(patsubst $(SRC)/%,$(BUILD)/%,$(OBJS:.cpp=.o))


IMGUI_OBJS := $(BUILD)/imgui.o \
              $(BUILD)/imgui_draw.o \
              $(BUILD)/imgui_tables.o \
              $(BUILD)/imgui_widgets.o \
              $(BUILD)/imgui_impl_sdl3.o \
              $(BUILD)/imgui_impl_sdlrenderer3.o

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
    CXXFLAGS += -DWINDOWS_BUILD
    CXXFLAGS += -IC:\sdl3\include -IC:\imgui -IC:\imgui\backends
    #CFLAGS += -mwindows

    ifeq ($(LINK_TYPE), static)
        LDFLAGS = C:\sdl3\lib\libSDL3.a -static -lmingw32 -limm32 -lole32 -lwinmm -ldxguid \
            -lsetupapi -lversion -loleaut32 -luuid -lcfgmgr32 -lcomdlg32 -luser32 -lgdi32
    else
        LDFLAGS = -L C:\sdl3\lib -lSDL3 -lcomdlg32 -luser32 -lgdi32
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

$(TARGET): $(OBJS) $(IMGUI_OBJS)
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) 
    #-Wl,-subsystem,windows

$(BUILD)/%.o: $(SRC)/%.c
	$(MKDIR_CMD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(SRC)/%.cpp
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/imgui.o: C:/imgui/imgui.cpp
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/imgui_draw.o: C:/imgui/imgui_draw.cpp
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/imgui_tables.o: C:/imgui/imgui_tables.cpp
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/imgui_widgets.o: C:/imgui/imgui_widgets.cpp
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/imgui_impl_sdl3.o: C:/imgui/backends/imgui_impl_sdl3.cpp
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/imgui_impl_sdlrenderer3.o: C:/imgui/backends/imgui_impl_sdlrenderer3.cpp
	$(MKDIR_CMD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ORIGINAL TARGETS FOR C ONLY PROJECT

# $(TARGET): $(OBJS)
# 	$(MKDIR_CMD)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# $(BUILD)/%.o: $(SRC)/%.c
# 	$(MKDIR_CMD)
# 	$(CC) $(CFLAGS) -c $< -o $@

# END ORIGINAL TARGETS FOR C ONLY PROJECT

clean:
	$(RM_CMD)
