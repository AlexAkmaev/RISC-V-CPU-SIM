#ifndef SIMULATOR_CONTOLUNIT_H
#define SIMULATOR_CONTOLUNIT_H

#include "units/Basics.h"

class ControlUnit final {
public:
    struct Flags final {
        bool WB_WE;  // reg file write enable
        bool ALU_SRC2{true};
        ALU::Op ALU_OP;
        CMP::Op CMP_OP;
        bool MEM_WE;
        bool WS;
        bool BRANCH_COND;
        bool JMP;
    } flags;

    void setState(const RISCVInstr &instr);
};

#endif //SIMULATOR_CONTOLUNIT_H
