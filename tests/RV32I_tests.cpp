#include "simulator.h"

bool test1() {
    /*
        lw a0, 32(zero)
        lw a1, 36(zero)
        sub a2, a1, a0
        sw a2, 40(zero)
    */
    std::vector<std::bitset<32>> imem = { 0x02002503U, 0x02402583U, 0x40a58633U, 0x02c02423U };
    Simulator cpu = Simulator{std::move(imem)};
    if (!cpu.Run()) {
        return false;
    }
    return true;
}

int main () {
    return test1();
}
