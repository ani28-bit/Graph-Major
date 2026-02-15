
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g -O2 -D_GNU_SOURCE
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data
OUTPUT_DIR = output


TARGET = $(BIN_DIR)/dhaka_router


SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)


$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(OUTPUT_DIR)/routes $(OUTPUT_DIR)/logs)
$(shell mkdir -p $(OBJ_DIR)/graph $(OBJ_DIR)/algorithms $(OBJ_DIR)/parsers)
$(shell mkdir -p $(OBJ_DIR)/routing $(OBJ_DIR)/models $(OBJ_DIR)/output $(OBJ_DIR)/utils)


all: $(TARGET)


$(TARGET): $(OBJS)
	@echo "Linking $@..."
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build successful!"


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@


clean:
	@echo "Cleaning build files..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete!"


clean-output:
	@echo "Cleaning output files..."
	@rm -rf $(OUTPUT_DIR)/routes/* $(OUTPUT_DIR)/logs/*
	@echo "Output clean complete!"


clean-all: clean clean-output


run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)


debug: CFLAGS += -DDEBUG -g3
debug: clean all


release: CFLAGS += -O3 -DNDEBUG
release: clean all


valgrind: $(TARGET)
	@echo "Running with valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)


help:
	@echo "Available targets:"
	@echo "  all          - Build the project (default)"
	@echo "  clean        - Remove build files"
	@echo "  clean-output - Remove output files"
	@echo "  clean-all    - Remove all generated files"
	@echo "  run          - Build and run the program"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build optimized release version"
	@echo "  valgrind     - Run with memory leak detection"
	@echo "  help         - Show this help message"

.PHONY: all clean clean-output clean-all run debug release valgrind help