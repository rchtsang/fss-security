#include "uc_processor.h"

#define BUF_SIZE (1 << 20)

#define CLEAR_BUF(buf, len) do {            \
    for (int idx = 0; idx < len; idx++) {   \
        buf[idx] = 0;                       \
    }                                       \
} while(0)

// macro for error checking
#define ERR_CHECK(err, func) do {                           \
    if (err) {                                              \
        printf("Failed on %s with error: %u (%s)\n",        \
            func, err, uc_strerror(err));                   \
        exit(-1);                                           \
    }                                                       \
} while(0)

// convert little endian to word
inline uint32_t le_to_word(uint8_t *src) {
    return ((src[0] & 0xFF)
        + ((src[1] & 0xFF) << 8)
        + ((src[2] & 0xFF) << 16)
        + ((src[3] & 0xFF) << 24));
}

void hook_gpio_mem(uc_engine* uc, uc_mem_type type,
        uint64_t addr, int size, int64_t value, void *user_data) {
    switch(type) {
        case UC_HOOK_MEM_READ_UNMAPPED:
            printf("memory read at 0x%" PRIx64 ", data size = %u\n", addr, size);
            break;
        case UC_HOOK_MEM_WRITE_UNMAPPED:
            printf("memory write at 0x%" PRIx64 ", data size = %u\n", addr, size);
            break;
        default:
            printf("memory access type=%d\n", type);
            break;
    }
    return;
}

void uc_processor::initialize(void) {
    // initialize emulator
    std::cout << "initializing...\n";
    ucerr = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc);
    ERR_CHECK(ucerr, "uc_open()");

    // initialize program memory
    ucerr = uc_mem_map(uc, mem_start, mem_size, UC_PROT_ALL);
    ERR_CHECK(ucerr, "uc_mem_map() (prog)");

    // initialize stack memory
    ucerr = uc_mem_map(uc, 0x20000000, 0x10000, UC_PROT_READ | UC_PROT_WRITE);
    ERR_CHECK(ucerr, "uc_mem_map() (ram)");

    // write firmware into memory
    ifstream file(firmware_fn, ios::binary);
    if (not file) {
        printf("failed to open file: %s\n", firmware_fn.c_str());
        exit(-1);
    }

    printf("writing firmware to memory...\n");

    size_t count;
    char *buf = new char[BUF_SIZE];
    uint64_t offset = 0;
    while(file) {
        // write from file to buffer
        file.read(buf, BUF_SIZE);
        count = file.gcount();
        if (!count) break;

        // write buffer to memory
        ucerr = uc_mem_write(uc, mem_start + offset, (uint8_t *) buf, count);
        ERR_CHECK(ucerr, "uc_mem_write()");
    }
    CLEAR_BUF(buf, BUF_SIZE);

    // register memory hooks
    ucerr = uc_hook_add(uc, &trace_gpio, 
        UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED,
        (void *) hook_gpio_mem, NULL, 1, 0);
    ERR_CHECK(ucerr, "uc_hook_add()");

    delete[] buf;

    return;
}

void uc_processor::main_action(void) {
    // read reset address from 0x0004 in memory for start address
    uint8_t buf[4];
    ucerr = uc_mem_read(uc, mem_start + 0x4, buf, 4);
    ERR_CHECK(ucerr, "uc_mem_read() (pc)");
    pc = (start_pc == -1) ? mem_start + (le_to_word(buf) | 1) : start_pc;
    // initialize stack pointer
    ucerr = uc_mem_read(uc, mem_start, buf, 4);
    ERR_CHECK(ucerr, "uc_mem_read() (sp)");
    sp = le_to_word(buf);
    uc_reg_write(uc, UC_ARM_REG_SP, &sp);

    while(1) {
        // execute one instruction
        printf("PC: %x\n", pc);
        ucerr = uc_mem_read(uc, pc & ~1, buf, 2);
        ERR_CHECK(ucerr, "uc_mem_read() (self loop check)");
        if (*((uint16_t *) buf) == 0xfee7) {
            printf("self loop detected! stopping...\n", pc);
            sc_stop();
        }
        uc_emu_start(uc, pc, 0, 0, 1);

        // get next pc
        uc_reg_read(uc, UC_ARM_REG_PC, &pc);
        uc_reg_read(uc, UC_ARM_REG_CPSR, &cpsr);
        pc |= (cpsr & (1 << 5)) ? 1 : 0;
        // debugging
        // uc_reg_read(uc, UC_ARM_REG_R1, &(gpr[1]));
        // uc_reg_read(uc, UC_ARM_REG_R2, &(gpr[2]));
        // uc_reg_read(uc, UC_ARM_REG_R3, &(gpr[3]));
        // printf("r1=%x r2=%x r3=%x\n", gpr[1], gpr[2], gpr[3]);
        wait();
    }
}