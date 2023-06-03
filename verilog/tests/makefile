
# == Directories
INC_DIR   := ./include
SRC_DIR   := .
BUILD_DIR := ./build
# ==

# == Files
TESTS      := $(shell find $(SRC_DIR) -type f -name '*.[Sc]')
TEST_BINS  := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%, $(patsubst $(SRC_DIR)/%.S, $(BUILD_DIR)/%, $(TESTS)))
# ==

# == Tools
CC := riscv32-unknown-elf-gcc -march=rv32id
LD := lld
# ==

# == Options
CFLAGS += -ffreestanding -nostdlib -mno-relax -I$(INC_DIR)
CFLAGS += -static  -Wl,-T,link.ld,--strip-debug
# ==

.SILENT:
.SECONDARY:
.SECONDEXPANSION:
.PHONY: all

all: $(TEST_BINS)

$(BUILD_DIR)/%: $(SRC_DIR)/%.S ./link.ld $(INC_DIR)/riscv_test.h $(MAKEFILE_LIST) | $$(dir $$@)
	@echo "Building $@"
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%: $(SRC_DIR)/%.c ./link.ld $(INC_DIR)/c_test.h $(INC_DIR)/c_support.h $(MAKEFILE_LIST) | $$(dir $$@)
	@echo "Building $@"
	$(CC) $(CFLAGS) $< -o $@

%/:
	mkdir -p $@

