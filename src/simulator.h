#ifndef SIMULATOR_SIMULATOR_H
#define SIMULATOR_SIMULATOR_H

#include <iostream>
#include <memory>

#include "stages/Fetch.h"
#include "stages/Decode.h"
#include "stages/Execute.h"
#include "stages/Memory.h"
#include "stages/WriteBack.h"

#include "units/ContolUnit.h"
#include "units/HazardUnit.h"

#include "riscv/instruction.h"
#include "riscv/opcodes.h"

class Simulator final {
public:
    explicit Simulator(uint32_t instr_count);
    explicit Simulator(std::vector<std::bitset<32>> &&imem);
    bool Run();

    void FDtransmitData();  // Fetch-Decode data transmition
    void DEtransmitData();  // Decode-Execute data transmition
    void EMtransmitData();  // Execute-Memory data transmition
    void MWBtransmitData();  // Memory-WriteBack data transmition

    Fetch fetch_;
    Decode decode_;
    Execute execute_;
    Memory memory_;
    WriteBack write_back_;

    HazardUnit hu_;
};

#endif //SIMULATOR_SIMULATOR_H
