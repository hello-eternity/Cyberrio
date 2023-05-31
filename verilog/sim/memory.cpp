
#include <algorithm>
#include <iostream>
#include <gelf.h>
#include <fcntl.h>
#include <libelf.h>

#include "memory.hpp"

void MagicMemory::handleRequest(Vcore &core) {
    if (core.ext_valid) {
        uint32_t address = core.ext_address;
        for (auto& handler : mapping) {
            if (address >= handler.start && address < handler.start + handler.length) {
                if (core.ext_write_strobe == 0) {
                    core.ext_read_data = handler.handle_read(address - handler.start);
                } else {
                    handler.handle_write(address - handler.start, core.ext_write_data, core.ext_write_strobe);
                }
                break;
            }
        }
        core.ext_ready = true;
    } else {
        core.ext_ready = false;
    }
}

void MagicMemory::delayRequest(Vcore& core) {
    core.ext_ready = false;
}

void MagicMemory::addHandler(MagicMappedHandler& handler) {
    mapping.push_back(handler);
}

uint32_t* MagicMemory::addRamHandler(uint32_t start, uint32_t length) {
    uint32_t* data = (uint32_t*)(new uint8_t[length]);
    MagicMappedHandler handler = {
        .start = start,
        .length = length,
        .handle_read = [=](uint32_t address) {
            return data[address / 4];
        },
        .handle_write = [=](uint32_t address, uint32_t write_data, uint8_t strobe) {
            uint32_t new_data = data[address / 4];
            for (int i = 0; i < 4; i++) {
                if (strobe & (1 << i)) {
                    new_data &= ~(0xff << (8 * i));
                    new_data |= (0xff << (8 * i)) & write_data;
                }
            }
            data[address / 4] = new_data;
        }
    };
    addHandler(handler);
    return data;
}

bool loadFromElfFile(const char* filename, uint32_t* data, uint32_t start, uint32_t length) {
    int fd = open(filename, O_RDONLY, 0);
    if (fd == -1) {
        std::cerr << "Failed to open the file '" << filename << "'" << std::endl;
        return false;
    }
    if (elf_version(EV_CURRENT) == EV_NONE) {
        std::cerr << "ELF library initialization failed" << std::endl;
        return false;
    }
    Elf* elf = elf_begin(fd, ELF_C_READ, NULL);
    if (elf == NULL) {
        std::cerr << "Failed to open ELF file '" << filename << "'" << std::endl;
        return false;
    } else if (elf_kind(elf) != ELF_K_ELF) {
        std::cerr << "The file must be an ELF object" << std::endl;
        return false;
    }
    GElf_Ehdr header;
    if (gelf_getehdr(elf, &header) != &header) {
        std::cerr << "Failed to read ELF header" << std::endl;
        return false;
    } else if (gelf_getclass(elf) != ELFCLASS32 || header.e_machine != EM_RISCV) {
        std::cerr << "ELF object architecture must be 32-bit riscv" << std::endl;
        return false;
    } else if (header.e_type != ET_EXEC) {
        std::cerr << "ELF object should be an executable" << std::endl;
        return false;
    }
    size_t program_count;
    if (elf_getphdrnum(elf, &program_count) != 0) {
        std::cerr << "Failed to get ELF program header count" << std::endl;
        return false;
    }
    for (size_t i = 0; i < program_count; i++) {
        GElf_Phdr program_header;
        if (gelf_getphdr(elf, i, &program_header) != &program_header) {
            std::cerr << "Failed to get ELF program header " << i << std::endl;
            return false;
        }
        if (program_header.p_type == PT_LOAD) {
            Elf_Data* elf_data = elf_getdata_rawchunk(elf, program_header.p_offset, program_header.p_filesz, ELF_T_WORD);
            if (program_header.p_paddr < (start + length) && (program_header.p_paddr + program_header.p_filesz) > start) {
                size_t copy_start = std::max<size_t>(program_header.p_paddr, start);
                size_t copy_end = std::min<size_t>(program_header.p_paddr + program_header.p_filesz, start + length);
                uint32_t* raw_data = (uint32_t*)elf_data->d_buf;
                for (size_t i = copy_start; i < copy_end; i += 4) {
                    data[(i - start) / 4] |= raw_data[(i - program_header.p_paddr) / 4];
                }
            }
        }
    }
    elf_end(elf);
    close(fd);
    return true;
}

