#ifndef SIMULATOR_INSTRUCTION_H
#define SIMULATOR_INSTRUCTION_H

#include <iostream>
#include <bitset>
#include "opcodes.h"

class RISCVInstr {
public:
    // RISCV base instruction formats
    enum class Format : uint8_t { R, I, S, B, U, J };

    RISCVInstr() = default;
    explicit RISCVInstr(std::bitset<32> i);

    [[nodiscard]] Opcode getOpcode() const;
    [[nodiscard]] Format getFormat() const;
    std::bitset<5> getRs1();
    std::bitset<5> getRs2();
    std::bitset<5> getRd();
    std::bitset<3> getFunct3();
    std::bitset<7> getFunct7();
private:
    void SelectBranch();
    void SelectL();
    void SelectS();
    void SelectII();
    void SelectR();

    std::bitset<32> instr_{0};
    Format type_;
    Opcode op_;

    std::bitset<3> funct3_{0};
    std::bitset<7> funct7_{0};
};

#endif //SIMULATOR_INSTRUCTION_H
