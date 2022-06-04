#include <fstream>
#include "simulator.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "No file passed to cpu" << std::endl;
        return 1;
    }

    IMEM imem;
    std::ifstream file((std::string(argv[1])));
    std::string ins_bits;
    while (std::getline(file, ins_bits)) {
        if (!ins_bits.empty()) {
            imem.pushBackInstr(std::bitset<32>{std::stoul(ins_bits, nullptr, 16)});
        }
    }

    Simulator cpu = Simulator{imem.getRawImem()};
    if (cpu.Run() == PipelineState::ERR) {
        return 2;
    }

    std::cout << "Total cycles: " << cpu.write_back_.cycle << std::endl;

    return 0;
}
