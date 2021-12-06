
#include <cstdio>
#include <systemc.h>

#include "uc_processor.h"

int sc_main(int argc, char *argv[]) {
    sc_clock clk("Clock", 1, SC_NS, 0.5, 0.0, SC_NS);

    printf("constructing processor...\n");

    uc_processor processor("TEST_PROCESSOR", 0x0, 2 * 1024 * 1024, "nrf52832_xxaa.bin", 0x2FD8);

    processor.CLK(clk);

    printf("starting sim...\n");

    sc_start();

    return 0;
}