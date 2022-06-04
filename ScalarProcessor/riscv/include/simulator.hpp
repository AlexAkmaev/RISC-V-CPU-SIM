#ifndef SIMULATOR_SIMULATOR_H
#define SIMULATOR_SIMULATOR_H

#include <iostream>
#include <memory>

#include "Fetch.hpp"
#include "Decode.hpp"
#include "Execute.hpp"
#include "Memory.hpp"
#include "WriteBack.hpp"

#include "ContolUnit.hpp"
#include "HazardUnit.hpp"

#include "instruction.hpp"
#include "opcodes.hpp"

struct Simulator final {
    explicit Simulator(uint32_t instr_count);
    explicit Simulator(std::vector<std::bitset<32>> &&imem);

    PipelineState Run();

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
