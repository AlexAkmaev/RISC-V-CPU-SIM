#include <fstream>
#include "simulator.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "No file passed to cpu" << std::endl;
        return 1;
    }

    IMEM imem;
    std::ifstream file((std::string(argv[1])));
    std::string ins_bits;
    while (std::getline(file, ins_bits)) {
        imem.pushBackInstr(std::bitset<32>{ins_bits});
    }

    Simulator cpu = Simulator{imem.getRawImem()};
    if (cpu.Run() == PipelineState::ERR) {
        return 2;
    }

    return 0;
}
