#include "Decode.h"

bool Decode::Run() {
    return true;
}

ControlUnit::Flags Decode::getCUState() const noexcept {
    return cu_.flags;
}

std::bitset<32> Decode::getRD1() const noexcept {
    return D1;
}

std::bitset<32> Decode::getRD2() const noexcept {
    return D2;
}
