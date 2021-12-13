#include "ContolUnit.h"

void ControlUnit::setState(const RISCVInstr &instr) {
    flags.RF_WE = (instr.getFormat() != RISCVInstr::Format::S &&
                    instr.getFormat() != RISCVInstr::Format::B) ? 1 : 0;
}
