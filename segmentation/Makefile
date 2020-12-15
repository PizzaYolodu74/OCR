# Directories
S_DIR=../segmentation
B_DIR=../segmentation/exec

# Files
S_FILES=$(S_DIR)/*.c

# Output
EXEC=$(B_DIR)/main

# Build settings
CC=gcc
# SDL options
CC_SDL=-lSDL2 -lm


all:Build

Build:
    $(CC) $(S_FILES) -o $(EXEC) $(CC_SDL)


clean:
    rm -rf $(B_DIR)/*