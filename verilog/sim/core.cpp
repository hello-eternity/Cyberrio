
#include "core.hpp"

void Core::reset() {
    core_logic.reset = 1;
    core_logic.clk = 0;
    core_logic.eval();
    core_logic.clk = 1;
    core_logic.eval();
    core_logic.reset = 0;
    core_logic.clk = 0;
    memory_wait = 0;
}

void Core::cycle() {
    if (memory_wait == 0) {
        memory.handleRequest(core_logic);
        memory_wait = memory_latency;
    } else {
        memory.delayRequest(core_logic);
        memory_wait--;
    }
    core_logic.eval();
    core_logic.clk = 1;
    core_logic.eval();
    core_logic.clk = 0;
}

