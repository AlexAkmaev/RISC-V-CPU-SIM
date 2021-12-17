#ifndef SIMULATOR_CONTOLUNIT_H
#define SIMULATOR_CONTOLUNIT_H

#include "units/Basics.h"

class ControlUnit final {
public:
    /* True = 1, false = 0 for multiplexer flags */
    struct Flags final {
        bool WB_WE{true};  // reg file write enable
        bool ALU_SRC1{true};
        bool ALU_SRC2{true};
        ALU::Op ALU_OP;
        CMP::Op CMP_OP;
        bool MEM_WE{false};
        DMEM::Width MEM_WIDTH;
        bool WS{false};  // write source
        bool BRANCH_COND{false};
        bool JMP{false};
        bool JALR{false};
    } flags;

    void setState(const RISCVInstr &instr);

private:
    void SelectALUOp(const RISCVInstr &instr);
    void SelectCMPOp(const RISCVInstr &instr);
    void SelectLoadFlags(const RISCVInstr &instr);
};

#endif //SIMULATOR_CONTOLUNIT_H
