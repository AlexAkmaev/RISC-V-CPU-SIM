# SuperScalar RISC-V CPU simulator
This is a scalar pipelined cpu simulator for RISC architecture (only RV32I).
### Structure
```
├── riscv/  ---------- Instruction representation, RV32I opcodes and simulator that combines all stages
├── stages/ ---------- Implementation of 5 pipeline stages: Fetch, Decode, Execute, Memory, WriteBack
├── tests/  ---------- Unit tests for each instruction separately and for blocks of code to check the correctness of branches and elimination conflicts
│   ├── BaseInstructionsTests.cpp
│   ├── BlocksTests.cpp
│   ├── CMakeLists.txt
│   ├── data/ -------- Tests with raw data that can be passed to cpu executable
│   │   └── loop.dat
│   └── HazardUnitTests.cpp
└── units/   --------- Implementation of the basic units that make up the microarchitecture
```
### Building
Run the following commands:
```
$ cmake -S . -B build
$ cmake --build build
```
### Running
Create file with instructions in hex format and pass as argument to cpu executable:
```
$ cd build 
& ./cpu ../tests/data/loop.dat
```
### Testing
To launch unit tests run the following command:
```
$ cd build && ctest
```
### Scheme
The scheme corresponding to this implementation:

