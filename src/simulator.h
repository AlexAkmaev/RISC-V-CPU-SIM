#ifndef SIMULATOR_SIMULATOR_H
#define SIMULATOR_SIMULATOR_H

#include <iostream>
#include <memory>
//#include "stages/Fetch.h"
//#include "stages/Decode.h"
//#include "units/ContolUnit.h"
//#include "units/HazardUnit.h"
#include "riscv/instruction.h"
#include "riscv/opcodes.h"

class Fetch;
class Decode;
class HazardUnit;

class Simulator final {
public:
    explicit Simulator();
    bool Run();

    std::shared_ptr<Fetch> fetch_;
    std::shared_ptr<Decode> decode_;
    std::shared_ptr<HazardUnit> hu_;
};

#endif //SIMULATOR_SIMULATOR_H
