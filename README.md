# Cyberrio
This is a chip completed under the leadership of AI, with the majority of the Verilog code implementation done using GPT-4. The relevant prompts can be found in the [cyberrio_prompt](cyberrio_prompt.pdf) file. Besides, the user_wrapper prompt part is in [user_wrapper_prompt](user_wrapper_prompt.pdf)

This is a small RISC-V core written in synthesizable Verilog that supports the RV32I unprivileged ISA and parts of the privileged ISA, namely M-mode.

## Code and verification

* The Verilog source of the core can be found inside the `src` directory. The top module is the `core` module inside `verilog/src/core.v`.
* In the `sim` directory you find a small simulator that when compiled using Verilator is used for testing.
* Inside the `tests` directory are the main tests testing the functionality of the core. Most of them are modified versions of the tests in [riscv-tests](https://github.com/riscv/riscv-tests).
  * The tests inside `verilog/tests/rv32ui` test the unprivileged ISA.
  * The tests inside `verilog/tests/rv32mi` test the privileged ISA.
* The makefile contains the `test` and `sim` targets. If you want to run all the test, run `make` or `make test`. If you only want to build the simulator run `make sim`.
* At present, only the isa test verification of the core part has been completed, and the correct verification result should print out every run and every isa te

## Architecture and GPT-4 part

The core uses the *classic* five-stage RISC pipeline (FETCH, DECODE, EXECUTE, MEMORY, WRITEBACK). It also implements bypassing from the WRITEBACK and, when possible, from the MEMORY stages. All pipeline stages have their own file inside `verilog/src/pipeline` and are connected together inside the `verilog/src/pipeline/pipeline.v` module.
Among them, fetch.v execute.v alu.v cmp.v busio.v regfile.v etc is completed through GPT-4, related prompt and output can refer to [cyberrio_prompt](cyberrio_prompt.pdf)

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

## GPT-4 generates cpu code using experience
During the process of generation, a multitude of options are available for selection. Ultimately, we opted to use GPT-4 to complete the principal sections in this repository. Given that all tasks executable by AutoGPT can also be performed by GPT-4, and considering the enhanced flexibility offered by GPT-4 over AutoGPT, we adopted GPT-4. Furthermore, the utility of Langchain was also contemplated, such as linking RISC-V documentation in advance and subsequently instructing GPT-4 to consult the documentation during generation. However, in the practical application of GPT-4, we observed that its understanding of some basic concepts was limited, leading us to forgo the use of Langchain. Yet, this approach may be reconsidered and further tested in the future, especially for areas requiring intricate details, such as Decode and CSR, where GPT lacks precise knowledge.

### File list generation

Throughout the actual steps of generation, virtually each step required multiple iterations, between 5 to 10, to achieve satisfactory results. Initially, we tried employing GPT to propose a five-stage pipeline CPU core file structure. However, due to the plethora of possible implementation strategies, even when it generated a file structure and was later informed about the entire structure, it failed to correctly generate specific features in many files. Thus, we ended up using only a portion of GPT's file structure definition, while we needed to predetermine the features to be implemented in each file.


### Module generation

Two primary issues emerged when generating each specific module: One was the Verilog syntax errors generated by GPT-4, primarily attributed to insufficient learning and training on hardware language, often processing parallel relationships in non-parallel software-writing ways, making errors more prone in sequential logic, and even producing unsupported Verilog syntax. The other was GPT-4's lack of understanding of firm handshaking relationships between hardware modules. Even when told that Module A needed a strict handshake with Module B, it couldn't provide the correct signal pattern.

Indeed, tackling hardware problems is a significant challenge for GPT-4. Thus, for most successfully generated modules, the prompts also included detailed interface descriptions and definitions, as well as the required sequential logic for different functions. Although describing these proved costly, it greatly differed from our initial speculation about GPT-4 usage.

Sections like decode and CSR, packed with information but relatively simple in logic, could hardly be completed with GPT-4. Even when GPT-4 was informed of certain details, it tended to suggest that we should complete this part of the code ourselves, only providing some irrelevant comments.

Generating files like pipeline.v proved unexpectedly challenging. The pipeline module only accomplishes one task: linking different modules and connecting the signals. Initially, we thought GPT-4 could handle this with ease, as it was a straightforward task with simple logic. However, despite the limited number of modules requiring connection, GPT-4 struggled to add necessary wires and establish appropriate connections to existing interfaces.

### Summary 

In summary, GPT-4's understanding of hardware language remains substantially incomplete, including comprehension of the language itself and concepts such as handshaking and parallel processing. We suspect this might be a deficiency in GPT-4's training process. Nevertheless, for issues related to information deficiency, alternative approaches such as Langchain can be employed as a potential solution.
# Caravel User Project

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0) [![UPRJ_CI](https://github.com/efabless/caravel_project_example/actions/workflows/user_project_ci.yml/badge.svg)](https://github.com/efabless/caravel_project_example/actions/workflows/user_project_ci.yml) [![Caravel Build](https://github.com/efabless/caravel_project_example/actions/workflows/caravel_build.yml/badge.svg)](https://github.com/efabless/caravel_project_example/actions/workflows/caravel_build.yml)

# Contributor
Xinze Wang, Guohua Yin, Yifei Zhu