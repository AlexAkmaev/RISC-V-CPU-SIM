# HOW TO
### Setup environment:
```bash
export SIM_ROOT=/path/to/Scalar or SuperScalarProcessor/folder
export SIM_BUILD=/path/to/build/folder
```
### Build:
```
./build.sh
# or
clang++-8 -I"${SIM_ROOT}/common/" -I"${SIM_ROOT}/riscv/include" -I"${SIM_ROOT}/units/include" -I"${SIM_ROOT}/stages/include" ${SIM_ROOT}/riscv/*.cpp ${SIM_ROOT}/stages/*.cpp ${SIM_ROOT}/units/*.cpp ${SIM_ROOT}/cpu.cpp -o cpu_
```
### Run:
```
./run.sh
# or
./cpu_ ${SIM_ROOT}/tests/data/loop10k.dat
```  

For more detailed info dive into folders `ScalarProcessor` or `SuperScalarProcessor`