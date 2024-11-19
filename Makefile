CC		= gcc
CFLAGS	= -I include/ -I src/ -Wall -Wextra -std=gnu17
BINARY	= $(BIN_DIR)/mcf

.PHONY: release debug buildfs clean

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

release: CFLAGS += -O3 -Wno-unknown-pragmas -Wno-unused-parameter
release: all

debug: CFLAGS += -O0 -ggdb -D MCF_DEBUG -Wno-unknown-pragmas -Wno-unused-parameter
debug: all

all: buildfs | $(BINARY)

$(BINARY): $(OBJ_FILES) $(INC_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for rebuilding when a header file changes
$(INC_FILES): $(wildcard include/MCF/*.h) $(wildcard $(SRC_DIR)/*.h)

buildfs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)