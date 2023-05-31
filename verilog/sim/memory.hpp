#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include <functional>
#include <vector>

#include "Vcore.h"

struct MagicMappedHandler {
    uint32_t start;
    uint32_t length;
    std::function<uint32_t(uint32_t)> handle_read;
    std::function<void(uint32_t, uint32_t, uint8_t)> handle_write;
};

struct MagicMemory {
    std::vector<MagicMappedHandler> mapping;

    void handleRequest(Vcore &core);
    void delayRequest(Vcore &core);
    void addHandler(MagicMappedHandler &handler);
    uint32_t* addRamHandler(uint32_t start, uint32_t length);
};

bool loadFromElfFile(const char* filename, uint32_t* data, uint32_t start, uint32_t length);

#endif
