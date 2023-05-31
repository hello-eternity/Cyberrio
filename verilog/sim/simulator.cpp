
#include <iostream>
#include <string.h>

#include "core.hpp"

static void printHelp(const char* prog) {
    std::cerr << "Usage: %s [options] program" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -h --help            print this text" << std::endl;
    std::cerr << "  -m --memory=SIZE     set memory size (default: 1M)" << std::endl;
    std::cerr << "  -c --cycles=CYCLES   set a cycle limit" << std::endl;
    std::cerr << "  -l --latency=CYCLES  set the memory latency (default: 0)" << std::endl;
    std::cerr << "  -e --exit            add the test exit device" << std::endl;
}

static void parseArguments(int argc, const char** argv, uint32_t* memory_size, int* cycle_limit, int* latency, bool* add_exit, const char** elf) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == '-') {
                if (strncmp(argv[i], "--memory", 8) == 0) {
                    int j;
                    if (argv[i][8] == '=') {
                        j = 9;
                    } else if (argv[i][8] == 0) {
                        i++;
                        j = 0;
                    } else {
                        std::cerr << "unknown option " << argv[i] << std::endl;
                        continue;
                    }
                    uint32_t number = 0;
                    for (; argv[i][j] != 0; j++) {
                        if (argv[i][j] >= '0' && argv[i][j] <= '9') {
                            number *= 10;
                            number += argv[i][j] - '0';
                        } else {
                            break;
                        }
                    }
                    if (argv[i][j] == 'G') {
                        j++;
                        number *= 1 << 30;
                    } else if (argv[i][j] == 'M') {
                        j++;
                        number *= 1 << 20;
                    } else if (argv[i][j] == 'K' || argv[i][j] == 'k') {
                        j++;
                        number *= 1 << 10;
                    }
                    if (argv[i][j] != 0) {
                        std::cerr << "memory size must be an size (e.g. 64M 128K 1G)" << std::endl;
                    }
                    *memory_size = number;
                } else if (strncmp(argv[i], "--cycles", 8) == 0) {
                    int j;
                    if (argv[i][8] == '=') {
                        j = 9;
                    } else if (argv[i][8] == 0) {
                        i++;
                        j = 0;
                    } else {
                        std::cerr << "unknown option " << argv[i] << std::endl;
                        continue;
                    }
                    int number = 0;
                    for (; argv[i][j] != 0; j++) {
                        if (argv[i][j] >= '0' && argv[i][j] <= '9') {
                            number *= 10;
                            number += argv[i][j] - '0';
                        } else {
                            std::cerr << "cycle count must be an integer" << std::endl;
                            break;
                        }
                    }
                    *cycle_limit = number;
                } else if (strncmp(argv[i], "--latency", 9) == 0) {
                    int j;
                    if (argv[i][9] == '=') {
                        j = 10;
                    } else if (argv[i][9] == 0) {
                        i++;
                        j = 0;
                    } else {
                        std::cerr << "unknown option " << argv[i] << std::endl;
                        continue;
                    }
                    int number = 0;
                    for (; argv[i][j] != 0; j++) {
                        if (argv[i][j] >= '0' && argv[i][j] <= '9') {
                            number *= 10;
                            number += argv[i][j] - '0';
                        } else {
                            std::cerr << "latency must be an integer" << std::endl;
                            break;
                        }
                    }
                    *latency = number;
                } else if (strcmp(argv[i], "--help") == 0) {
                    printHelp(argv[0]);
                } else if (strcmp(argv[i], "--exit") == 0) {
                    *add_exit = true;
                } else {
                    std::cerr << "unknown option " << argv[i] << std::endl;
                }
            } else {
                int p = i;
                for (int j = 1; argv[p][j] != 0; j++) {
                    if (argv[p][j] == 'm') {
                        i++;
                        int number = 0;
                        int j;
                        for (j = 0; argv[i][j] != 0; j++) {
                            if (argv[i][j] >= '0' && argv[i][j] <= '9') {
                                number *= 10;
                                number += argv[i][j] - '0';
                            } else {
                                break;
                            }
                        }
                        if (argv[i][j] == 'G') {
                            j++;
                            number *= 1 << 30;
                        } else if (argv[i][j] == 'M') {
                            j++;
                            number *= 1 << 20;
                        } else if (argv[i][j] == 'K' || argv[i][j] == 'k') {
                            j++;
                            number *= 1 << 10;
                        }
                        if (argv[i][j] != 0) {
                            std::cerr << "memory size must be an size (e.g. 64M 128K 1G)" << std::endl;
                        }
                        *memory_size = number;
                    } else if (argv[p][j] == 'c') {
                        i++;
                        int number = 0;
                        for (int j = 0; argv[i][j] != 0; j++) {
                            if (argv[i][j] >= '0' && argv[i][j] <= '9') {
                                number *= 10;
                                number += argv[i][j] - '0';
                            } else {
                                std::cerr << "cycle count must be an integer" << std::endl;
                                break;
                            }
                        }
                        *cycle_limit = number;
                    } else if (argv[p][j] == 'l') {
                        i++;
                        int number = 0;
                        for (int j = 0; argv[i][j] != 0; j++) {
                            if (argv[i][j] >= '0' && argv[i][j] <= '9') {
                                number *= 10;
                                number += argv[i][j] - '0';
                            } else {
                                std::cerr << "latency must be an integer" << std::endl;
                                break;
                            }
                        }
                        *latency = number;
                    } else if (argv[p][j] == 'h') {
                        printHelp(argv[0]);
                    } else if (argv[p][j] == 'e') {
                        *add_exit = true;
                    } else {
                        std::cerr << "unknown option -" << argv[p][j] << std::endl;
                    }
                }
            }
        } else {
            *elf = argv[i];
        }
    }
}

static void addHandlers(MagicMemory& memory, bool add_exit) {
    MagicMappedHandler console = {
        .start = 0x10000000,
        .length = 4,
        .handle_read = [](uint32_t addres) {
            return 0;
        },
        .handle_write = [](uint32_t address, uint32_t data, uint8_t strobe) {
            if (strobe & 0b0001) {
                std::cerr << (char)(data & 0xff);
            }
        }
    };
    memory.addHandler(console);
    if (add_exit) {
        MagicMappedHandler exiter = {
            .start = 0x11000000,
            .length = 4,
            .handle_read = [](uint32_t addres) {
                return 0;
            },
            .handle_write = [](uint32_t address, uint32_t data, uint8_t strobe) {
                if (data == 1) {
                    exit(EXIT_SUCCESS);
                } else if ((data & 0x100) != 0) {
                    if (data & 0x7000000) {
                        static const char* exception_name[] = {
                            "user software",       "supervisor software",
                            "hypervisor software", "machine software",
                            "user timer",          "supervisor timer",
                            "hypervisor timer",    "machine timer",
                            "user external",       "supervisor external",
                            "hypervisor external", "machine external",
                        };
                        if ((data & 0xff) < 12) {
                            std::cerr << "Failed with unhandled interrupt '" << exception_name[data & 0xff] << "'" << std::endl;
                        } else {
                            std::cerr << "Failed with unhandled interrupt " << (data & 0xff) << std::endl;
                        }
                    } else {
                        static const char* exception_name[] = {
                            "misaligned fetch",    "fetch access",
                            "illegal instruction", "breakpoint",
                            "misaligned load",     "load access",
                            "misaligned store",    "store access",
                            "user_ecall",          "supervisor_ecall",
                            "hypervisor_ecall",    "machine_ecall",
                            "fetch page fault",    "load page fault",
                            "reserved for std",    "store page fault",
                        };
                        if ((data & 0xff) < 16) {
                            std::cerr << "Failed with unhandled exception '" << exception_name[data & 0xff] << "'" << std::endl;
                        } else {
                            std::cerr << "Failed with unhandled exception " << (data & 0xff) << std::endl;
                        }
                    }
                    exit(1);
                } else {
                    std::cerr << "Failed test case #" << data << std::endl;
                    exit(1);
                }
            }
        };
        memory.addHandler(exiter);
    }
}

int main(int argc, const char** argv) {
    uint32_t memory_size = 1 << 20;
    int cycle_limit = 0;
    int latency = 0;
    bool add_exit = false;
    const char* elf = NULL;
    parseArguments(argc, argv, &memory_size, &cycle_limit, &latency, &add_exit, &elf);
    if (elf == NULL) {
        return 1;
    } else {
        Core core;
        core.memory_latency = latency;
        uint32_t* ram = core.memory.addRamHandler(0x80000000, memory_size);
        if (!loadFromElfFile(elf, ram, 0x80000000, memory_size)) {
            return 1;
        }
        addHandlers(core.memory, add_exit);
        core.reset();
        for (int i = 0; i < cycle_limit || cycle_limit == 0; i++) {
            core.cycle();
        }
        std::cerr << "terminated after " << cycle_limit << " cycles" << std::endl;
        delete[] ram;
        return 1;
    }
}
