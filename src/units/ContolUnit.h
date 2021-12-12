#ifndef SIMULATOR_CONTOLUNIT_H
#define SIMULATOR_CONTOLUNIT_H

#include "riscv/instruction.h"

class ControlUnit {
public:
    enum class Flags : bool {

    };

    void setState(const RISCVInstr &instr);
};

#endif //SIMULATOR_CONTOLUNIT_H
