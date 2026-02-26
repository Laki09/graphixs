# ===============================
# Config
# ===============================
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

C_SRCS := $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)

C_OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))
CXX_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(CXX_SRCS))

TARGET := graphixs

CFLAGS := -I$(INCLUDE_DIR)
CFLAGS  += -Ilib/include -DGL_SILENCE_DEPRECATION
LDFLAGS := -Llib lib/libglfw3.a -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

CXXFLAGS := -I$(INCLUDE_DIR)
CXXFLAGS += -DGL_SILENCE_DEPRECATION

# ===============================
# Compiler detection
# ===============================
CC := $(shell command -v gcc || command -v clang)
ifeq ($(strip $(CC)),)
    $(error Neither gcc nor clang is available.)
endif

CXX := $(shell command -v g++ || command -v clang++)
ifeq ($(strip $(CXX)),)
    $(error Neither g++ nor clang++ is available.)
endif

# ===============================
# Default target
# ===============================
all: release

# ===============================
# Release build
# ===============================
release: CFLAGS += -O2
release: CXXFLAGS += -O2
release: $(TARGET)

$(TARGET): $(C_OBJS) $(CXX_OBJS)
	$(CC) $(C_OBJS) $(CXX_OBJS) $(LDFLAGS) -o $(TARGET)

# ===============================
# Build object files
# ===============================
# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# C++ files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ===============================
# Clean
# ===============================
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all release clean
