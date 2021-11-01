# Full System Simulation for Embedded System Development and Security

Heavily inspired by Renode, Logism, and SimSoC

---
## Topics

- Instruction Set Simulation
- Hardware-in-the-Loop
- Full System Simulation
- Hardware/Software Co-Verification
- Symbolic Execution
- 

---
## Major Related Works and Tools

### Simulators

#### Open Source

[**SimSoC**](https://ieeexplore.ieee.org/abstract/document/5416870)
SimSoC is a full system simulation platform based on integrating instruction set simulation (ISS) and hardware simulation using transaction level modeling abstractions to model communications. The platform was developed using SystemC as the basis for hardware simulation. It appeared in 2007 but doesn't appear to be under active development as of 

The project was supposed to be open source, but appears to be unavailable at the moment. It has been used for educational purposes, and seems decently well known in academia, but doesn't appear to be much used in industry. 



#### Proprietary

[**Wind River Simics**](https://ieeexplore.ieee.org/abstract/document/982916)
A full system simulation platform that has been around commercially since 1998, acquired by Intel in 2010 and marketed by Wind River Systems. It too is a collection of instruction set simulators and hardware models that is able to simulate a variety of systems. It's an extremely powerful simulation platform with a large number of features, noteably reverse debugging, and support for various hardware.

[**OVPsim**]()


---
## Meeting Notes

9/20
- todo: 
	- construct the simulator core using unicorn
	- have a presentation for unicorn
- we are aiming to have a presentable by december
	- we want a couple interesting features for this firmware analysis tool 
	- and we might want to focus on some specific classes of vulnerabilities
		- e.g. some common issues and emerging issues
		- memory leaks and attacks
- focus on developing the auditor rather than the attack. emphasis on defense.

- **DEADLINE: presentable by 12/17**

Thoughts:
- need to address how this tool might work with embedded linux
- 

---
## Core Features

- Processor Emulation:
	- built around qemu dynamic translation? (that's what renode does)
	- perhaps translate to VEX as in angr? (requires python)
	- should be able to work with a binary provided as-is
	- should be able to track internal program and processor state
	- assumption of memory mapped i/o
- Node Simulation:
	- Simulation of both digital and analog outputs
	- simulates analog components as well
- Peripheral Simulation:
	- Peripherals can can be user defined with functional behavior
	- More complex peripherals that use wired communication protocols should be supported as well
- Construct circuits from given netlist or schematic file
- Integration with angr or other symbolic execution
	- test for unsafe states
	- test for other vulnerabilities

### Additional Features

- capture node outputs over time
	- outputs can be limit tested and safety tested
- accept verilog component definitions
- integration with gdb
- basic automated output testing

---
## Useful Links

- [renode](https://renode.io/)
	- renode source code ([github](https://github.com/renode/renode))
- [logism](http://www.cburch.com/logisim/index.html)
	- logism source code ([sourceforge](https://sourceforge.net/p/circuit/code/HEAD/tree/tags/2.7.1/src/com/cburch/logisim/))

## Notes

- If we want to simulate the program, we can lift the binary to an intermediate language like VEX as angr does, then process the VEX instructions with simulated memory and registers like angr.

- It full embedded system simulation seems to have been a hot topic last decade. There don't appear to be as many papers on the topic. 

- Some common keywords I'm seeing: Hardware-in-the-Loop, Instruction Set Simulator, Hardware/Software Co-Verification and Co-Design, Full-System Simulation, 