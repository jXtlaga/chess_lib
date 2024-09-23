# Variables
CC = gcc
CFLAGS = -Wall -fPIC
SRC_DIR = src  # Root source directory
OBJ_DIR = obj  # Object files directory
LIB_DIR = lib  # Directory for the library
TARGET = $(LIB_DIR)/libmy_library.so  # Shared library target

# Find all .c files in src and its subdirectories
SOURCES = $(shell find $(SRC_DIR) -name "*.c")

# Convert the .c files into corresponding .o files in obj/
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Default target
all: $(TARGET)

# Create the shared library
$(TARGET): $(OBJECTS)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared $(OBJECTS) -o $(TARGET)

# Rule to compile .c files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)  # Create the object file's directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
