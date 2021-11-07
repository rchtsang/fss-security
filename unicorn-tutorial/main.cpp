#include <stdio.h>
#include <unicorn/unicorn.h>

// code to emulate
/**
 *         mov  R0, #0
 *         mov  R1, #10
 * again:  add  R0, R0, R1
 *         subs R1, R1, #1
 *         bne  again
 */
#define ARM_CODE "\x4f\xf0\x00\x00\x4f\xf0\x0a\x01\x08\x44\x49\x1e\xfc\xd1"

// memory address where emulation starts
#define ADDRESS 0x10000

int main(int argc, char **argv, char **envp) {
	uc_engine *uc;
	uc_err err;

	// initialize registers
	int r0 = 0;
	int r1 = 0;
	
	printf("Emulate ARM Assembly\n");

	// initialize emulator in arm mode (dynamically switches to thumb)
	err = uc_open(UC_ARCH_ARM, UC_MODE_THUMB, &uc);
	if (err) {
		printf("Failed on uc_open() with error returned: %u (%s)\n",
			err, uc_strerror(err));
		return -1;
	}
	// map 2MB memory for emulation
	uc_mem_map(uc, ADDRESS, 2 * 1024 * 1024, UC_PROT_ALL);

	// write the machine code to be emulated to memory
	uc_mem_write(uc, ADDRESS, ARM_CODE, sizeof(ARM_CODE) - 1);

	// initialize machine registers
	uc_reg_write(uc, UC_ARM_REG_R0, &r0);
	uc_reg_write(uc, UC_ARM_REG_R1, &r1);

	// emulate machine code in infinite time or until finished
	err = uc_emu_start(uc, ADDRESS | 1, ADDRESS + sizeof(ARM_CODE) - 1, 0, 0);
	if (err) {
		printf("Failed on uc_emu_start() with error returned: %u (%s)\n", 
			err, uc_strerror(err));
	}

	// print register
	printf(">>> Emulation done.\n\nContext:\n");
	uc_reg_read(uc, UC_ARM_REG_R0, &r0);
	uc_reg_read(uc, UC_ARM_REG_R1, &r1);
	printf(">>> R0 = 0x%x\n", r0);
	printf(">>> R1 = 0x%x\n", r1);

	uc_close(uc);

#ifdef DYNLOAD
	uc_dyn_free();
#endif

	return 0;
}