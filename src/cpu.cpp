#include "simulator.h"

int main() {
    std::bitset<7> b{"1100111"};
    std::cout << 0b1100111 << "\n" << b.to_ulong() << "\n" << (b.to_ulong() == 0b1100111) << std::endl;
    Simulator cpu = Simulator();
    if (!cpu.Run()) {
        return 1;
    }
    return 0;
}
