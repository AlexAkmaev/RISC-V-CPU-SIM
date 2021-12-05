#include <iostream>
#include "stages/Fetch.h"
#include "riscv/instruction.h"
#include "riscv/opcodes.h"

#ifndef SIMULATOR_SIMULATOR_H
#define SIMULATOR_SIMULATOR_H

class Simulator {
public:
    explicit Simulator();
    bool Run();

private:
    FetchStage *fetch_;
};

#endif //SIMULATOR_SIMULATOR_H
