
# == Directories
SRC_DIR   := src
BUILD_DIR := build
TEST_DIR  := tests
SIM_DIR   := sim
# ==

# == Test files
VERILOG_SRC := $(shell find $(SRC_DIR) -type f -name '*.v')
TEST_SRC    := $(shell find $(TEST_DIR) -type f -name '*.[Sc]')
TEST_BINS   := $(patsubst $(TEST_DIR)/%.c, $(TEST_DIR)/build/%, $(patsubst $(TEST_DIR)/%.S, $(TEST_DIR)/build/%, $(TEST_SRC)))
# ==

# == Simulator files
SIM_SRC := $(shell find $(SIM_DIR) -type f -name '*.cpp')
# ==

# == Runing goals
RUNTESTS  := $(addprefix RUNTEST.,$(TEST_BINS))
# ==

# == Verilator config
VERILATOR := verilator
VERIFLAGS := $(addprefix -I,$(shell find $(SRC_DIR) -type d)) -Wall -Mdir $(BUILD_DIR)
# ==

.SILENT:
.SECONDARY:
.SECONDEXPANSION:
.PHONY: test sim build-tests RUNTEST.$(TEST_DIR)/build/% $(TEST_DIR)/build

test: $(RUNTESTS)

sim: $(BUILD_DIR)/Vcore

$(BUILD_DIR)/V%: $(SRC_DIR)/units/%.v $(UNITS_DIR)/%.cpp | $$(dir $$@)
	@echo Building $@
	$(VERILATOR) $(VERIFLAGS) --cc --exe --build $^

$(BUILD_DIR)/Vcore: $(VERILOG_SRC) $(SIM_SRC) | $$(dir $$@)
	@echo Building $@
	$(VERILATOR) $(VERIFLAGS) --cc --exe --build -LDFLAGS -lelf $(SRC_DIR)/core.v $(SIM_SRC)

$(TEST_DIR)/build: $(TEST_SRC)
	$(MAKE) -C $(TEST_DIR)

$(TEST_DIR)/build/%: $(TEST_DIR)/build $(TEST_DIR)/%.S
	@true

%/:
	mkdir -p $@

RUNTEST.$(TEST_DIR)/build/%: $(TEST_DIR)/build/% $(BUILD_DIR)/Vcore
	@echo "Running test $(notdir $<)"
	$(BUILD_DIR)/Vcore -c 1000000 -e -l 5 $<

