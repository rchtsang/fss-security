# SystemC-Unicorn Full System Simulation

A simple ARM Cortex-M full system simulator.

## Notes

### Cortex-M Registers

| Register  | Unicorn Register 		| Description |
| :-------- | :-------------------- | :------------------------------------ |
| R0 - R12  | UC_ARM_REG_R0-12		| General Purpose Registers				|
| SP 		| UC_ARM_REG_SP			| Stack Pointer 						|
| LR 		| UC_ARM_REG_LR			| Link Register 						|
| PC		| UC_ARM_REG_PC			| Program Counter						|
| PSR		| N/A 					| Program Status Register 				|
| APSR		| UC_ARM_REG_APSR		| Application Program Status Register 	|
| IPSR		| UC_ARM_REG_IPSR		| Interrupt Program Status Register 	|
| EPSR		| N/A 					| Execution Program Status Register 	|
| PRIMASK	| N/A 					| Priority Mask Register 				|
| FAULTMASK | N/A 					| Fault Mask Register 					|
| BASEPRI	| N/A 					| Base Priority Mask Register 			|
| CONTROL	| UC_ARM_REG_CONTROL	| Control Register 						|
