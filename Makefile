# Compiler and linker
CC = g++
LD = g++

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Output binary
TARGET = $(BIN_DIR)/SteamEngine.bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Compiler flags
CFLAGS = -Wall -O2 -std=c++11

# Linker flags
LDFLAGS = 

# Build target
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(LD) $(LDFLAGS) -o $@ $^

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Default target
all: clean $(TARGET)
