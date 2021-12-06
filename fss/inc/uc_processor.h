/**
 * processor.h -- SystemC Processor Module
 *
 * Wraps Unicorn in a SystemC module for use in simulations
 */

#ifndef UC_PROCESSOR_H
#define UC_PROCESSOR_H

#include <string>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <fstream>


#include <systemc.h>
#include <unicorn/unicorn.h>

using namespace std;
using namespace sc_core;

struct uc_processor : public sc_module {
    sc_in_clk   CLK;

// processor variables
    // registers
    int32_t gpr[13];    // general purpose registers
    uint32_t sp;        // stack pointer
    uint32_t lr;        // link register
    uint32_t pc;        // program counter
    uint32_t apsr;      // application program status register
    uint32_t ipsr;      // interrupt program status register
    uint32_t cpsr;      // current program status register (arm/thumb mode)
    uint32_t control;   // control register

// unicorn variables
    uc_engine *uc;      // unicorn instance pointer
    uc_err ucerr;       // unicorn error
    uint64_t mem_start; // memory start address
    size_t mem_size;    // memory size
    string firmware_fn; // firmware filename
    uc_hook trace_gpio; // gpio hook
    uint32_t start_pc;  // custom start execution (-1 if not)

    void initialize(void);

    void main_action(void);

    SC_HAS_PROCESS(uc_processor);

    uc_processor(sc_module_name name, uint64_t addr, 
            size_t msize, string fn, uint32_t pc) {
        // initialize unicorn variables
        firmware_fn = fn;
        mem_start = addr;
        mem_size = msize;
        start_pc = pc;

        initialize();

        SC_CTHREAD(main_action, CLK.pos());
    }
};


#endif // PROCESSOR_H