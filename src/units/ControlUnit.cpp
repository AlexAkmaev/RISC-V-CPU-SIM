#include "ContolUnit.h"

void ControlUnit::setState(const RISCVInstr &instr) {
    switch (instr.getFormat()) {
        case RISCVInstr::Format::R: {
            flags.WB_WE = true;
            flags.ALU_SRC2 = true;
            break;
        }
        case RISCVInstr::Format::I:
            flags.WB_WE = true;
            flags.ALU_SRC2 = false;
            break;
        case RISCVInstr::Format::S:
            flags.WB_WE = false;
            flags.ALU_SRC2 = true;
            break;
        case RISCVInstr::Format::B:
            flags.WB_WE = false;
            flags.ALU_SRC2 = true;
            break;
        case RISCVInstr::Format::U:
            flags.WB_WE = true;
            flags.ALU_SRC2 = true;
            break;
        case RISCVInstr::Format::J:
            flags.WB_WE = true;
            flags.ALU_SRC2 = true;
            break;
    }
}
