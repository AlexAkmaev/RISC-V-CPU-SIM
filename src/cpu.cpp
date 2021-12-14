#include "simulator.h"

int main() {
//    std::bitset<16> bb{"1111111111110110"};
//    std::bitset<32> bb = -1;
//    std::bitset<32> bb{"11111111111111111111111111111110"};
//    std::bitset<32> bb{"1111000010011111100011111011001"};
//    std::cout << bb.to_ulong() << std::endl;
//    std::cout << bb.to_string() << std::endl;
//    std::cout << static_cast<int32_t>(bb.to_ulong()) << std::endl;
    Simulator cpu = Simulator();
    if (!cpu.Run()) {
        return 1;
    }
    return 0;
}
