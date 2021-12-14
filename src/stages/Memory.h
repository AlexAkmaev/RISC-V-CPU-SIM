#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "units/Basics.h"

class Memory final : public Stage {
public:
    bool Run(Simulator &cpu) override;
    bool Stall() override;

    [[nodiscard]] std::bitset<32> BP_MEM() const noexcept;
private:
    std::bitset<32> bp_mem_;
};

#endif //SIMULATOR_MEMORY_H
