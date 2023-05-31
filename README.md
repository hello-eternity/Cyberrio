# Cyberrio
This is a chip completed under the leadership of AI, with the majority of the Verilog code implementation done using GPT-4. The relevant prompts can be found in the "cyberrio_prompt.pdf" file.

This is a small RISC-V core written in synthesizable Verilog that supports the RV32I unprivileged ISA and parts of the privileged ISA, namely M-mode.
This is only the RISC-V core and does not include any other peripherals.

## Files in this Repository

* The Verilog source of the core can be found inside the `src` directory. The top module is the `core` module inside `verilog/src/core.v`.
* In the `sim` directory you find a small simulator that when compiled using Verilator is used for testing.
* Inside the `tests` directory are the main tests testing the functionality of the core. Most of them are modified versions of the tests in [riscv-tests](https://github.com/riscv/riscv-tests).
  * The tests inside `verilog/tests/rv32ui` test the unprivileged ISA.
  * The tests inside `verilog/tests/rv32mi` test the privileged ISA.
* The makefile contains the `test` and `sim` targets. If you want to run all the test, run `make` or `make test`. If you only want to build the simulator run `make sim`.

## Architecture

This core is currently not really optimized in any way. It is designed mainly with simplicity in mind. There is currently no instruction and no data cache, which means that the speed of the core will depend significantly on the memory latency and speed.
The core uses the *classic* five-stage RISC pipeline (FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK). It also implements bypassing from the WRITEBACK and, when possible, from the MEMORY stages. All pipeline stages have their own file inside `verilog/src/pipeline` and are connected together inside the `verilog/src/pipeline/pipeline.v` module.

### Memory interface

The native memory interface of the core is a simple 32 bit valid-ready interface that can run one memory transfer at a time.
```verilog
output        ext_valid,
output        ext_instruction,
input         ext_ready,

output [31:0] ext_address,
output [31:0] ext_write_data,
output [ 3:0] ext_write_strobe,
input  [31:0] ext_read_data
```
#### Read

For a memory read operation (this includes instruction fetch) `valid` will be `1` and `write_strobe` will be `0`. `address` points to the address that should be read and `instruction` is set to `1` if this operation is an instruction fetch.
The operation's result should be returned by writing the value to `read_data` and asserting `ready`. After asserting `ready` for a single rising edge of the clock a new memory operation starts.

#### Write

For a memory write operation `valid` will be `1` and `write_strobe` will be different from `0`. `address` points to the address that should be written to and `write_data` is set to the value that should be written. `write_strobe` indicates which bytes of the 32 bit word at `address` should be written to. `write_strobe[0]` indicates whether the least significant byte of data should be written, and `write_strobe[3]` indicates whether the most significant byte should be written.
The operation's completion should be signalled by asserting `ready`. After asserting `ready` for a single rising edge of the clock a new memory operation starts.

## Simulator

The simulator is only very simple. It allows you to set the amount of available memory, the memory latency and the maximum number of cycles to execute (this is used in the test to prevent infinite loops) and initialize the memory using ELF executable files. The simulator will also write every byte written to the address `0x10000000` to stderr (this is a placeholder for a real UART device).
# Caravel User Project

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) [![UPRJ_CI](https://github.com/efabless/caravel_project_example/actions/workflows/user_project_ci.yml/badge.svg)](https://github.com/efabless/caravel_project_example/actions/workflows/user_project_ci.yml) [![Caravel Build](https://github.com/efabless/caravel_project_example/actions/workflows/caravel_build.yml/badge.svg)](https://github.com/efabless/caravel_project_example/actions/workflows/caravel_build.yml)

| :exclamation: Important Note            |
|-----------------------------------------|

## Please fill in your project documentation in this README.md file 

Refer to [README](docs/source/index.rst#section-quickstart) for a quickstart of how to use caravel_user_project

Refer to [README](docs/source/index.rst) for this sample project documentation. 
