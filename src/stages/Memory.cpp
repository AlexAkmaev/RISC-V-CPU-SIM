#include "Memory.h"
#include "simulator.h"

bool Memory::Run(Simulator &cpu) {
    return true;
}

bool Memory::Stall() {
    return true;
}

std::bitset<32> Memory::BP_MEM() const noexcept {
    return bp_mem_;
}
