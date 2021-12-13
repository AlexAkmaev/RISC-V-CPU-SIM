#ifndef SIMULATOR_SIMULATOR_H
#define SIMULATOR_SIMULATOR_H

#include <iostream>
#include "stages/Fetch.h"
#include "stages/Decode.h"
#include "units/ContolUnit.h"
#include "units/HazardUnit.h"
#include "riscv/instruction.h"
#include "riscv/opcodes.h"

class Simulator {
public:
    explicit Simulator();
    bool Run();

private:
    Fetch fetch_;
    Decode decode_;
};

#endif //SIMULATOR_SIMULATOR_H
