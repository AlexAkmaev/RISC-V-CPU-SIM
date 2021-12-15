#ifndef SIMULATOR_CONTOLUNIT_H
#define SIMULATOR_CONTOLUNIT_H

#include "units/Basics.h"

class ControlUnit final {
public:
    struct Flags final {
        bool WB_WE{true};  // reg file write enable
        bool ALU_SRC2{true};
        ALU::Op ALU_OP;
        CMP::Op CMP_OP;
        bool MEM_WE{false};
        bool WS{false};  // ALU{false} or DMEM{true} source
        bool BRANCH_COND{false};
        bool JMP{false};
    } flags;

    void setState(const RISCVInstr &instr);

private:
    void SelectALUOp(const RISCVInstr &instr);
    void SelectCMPOp(const RISCVInstr &instr);
    void SelectWS(const RISCVInstr &instr);
};

#endif //SIMULATOR_CONTOLUNIT_H
