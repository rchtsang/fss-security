# Unicorn Tutorial

Eventually, the goal is to be able to simulate the ISA for Cortex-M architectures, which run primarily on ARMv5, ARMv6 and ARMv7-M (v7E-M). I'm a bit unclear on the terminology when people say "ARM" vs a particular version, but from what I can tell, it has to do with additional dedicated instructions for hardware specific to that architecture. Eventually that will mean defining additional behavior, but for now we'll focus on getting the core up and running.

## Basics of using CMake

CMake is a meta-build system. It builds build systems like Makefiles. We will want to use it for the overall project, so we use it in this tutorial to get our feet wet.

The `CMakeLists.txt` file is used to dictate how cmake will construct the Makefile. 

At the top, the `cmake_minimum_required()` is needed to specify a CMake version, and `project()` defines the name of the project, its version, and languages. 

To build the executable, use `add_executable(<exe name> <files>)` which declares the name of the output executable and the source files needed to build it.

Include directories can be specified with `include_directories()`.

Link directories (which contain linked libraries) can be specified with `link_directories()`. Third party libraries that do not get built by this system are specified as `INTERFACE`s. The actual library file is indicated with `target_link_libraries(<exe name> <libraries>)` which tell cmake which libraries are actually needed.

### Building with CMake

From the project root directory:

```sh
mkdir build && cd build     # creates a build directory
cmake ..                    # generates makefile
make                        # runs makefile and builds project
```

**Resources:**
- https://cmake.org/cmake/help/latest/guide/tutorial/index.html
- https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20a%20Library.html
- https://cmake.org/cmake/help/latest/command/target_link_libraries.html
- https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html

## Getting ARM Assembly Examples

I am using [shell-storm.org](http://shell-storm.org/online/Online-Assembler-and-Disassembler/) right now to get short snippets of ARM(Thumb) for testing purposes. It is an online wrapper around the [Keystone](https://github.com/keystone-engine/keystone) Assembler and [Capstone](https://github.com/aquynh/capstone) Disassembler projects.

For example, a simple set of instructions that add values:

```arm
        mov  R0, #0
        mov  R1, #10
again:  add  R0, R0, R1
        subs R1, R1, #1
        bne  again
```

Gets assembled into the inline little-endian machine code sequence:

```\x4f\xf0\x00\x00\x4f\xf0\x0a\x01\x08\x44\x49\x1e\xfc\xd1```

Note: Eventually, I want to get this working with real binary files compiled from tools like `arm-none-eabi-gcc`, but those binaries are full of standard library stuff that makes it rather annoying to start with, but we'll get there.


