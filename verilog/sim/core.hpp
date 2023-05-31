
#include "Vcore.h"
#include "memory.hpp"

struct Core {
    Vcore core_logic;
    MagicMemory memory;
    int memory_latency;
    int memory_wait;

    void reset();
    void cycle();
};

