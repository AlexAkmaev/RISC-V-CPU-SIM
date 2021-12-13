#ifndef SIMULATOR_CONTOLUNIT_H
#define SIMULATOR_CONTOLUNIT_H

#include "riscv/instruction.h"

class ControlUnit final {
public:
    struct Flags final {
        bool WB_WE;  // reg file write enable
        uint8_t ALU_OP;
        uint8_t ALU_SRC2;
        bool MEM_WE;
        bool BRANCH_COND;
    } flags;

    void setState(const RISCVInstr &instr);
};

#endif //SIMULATOR_CONTOLUNIT_H
