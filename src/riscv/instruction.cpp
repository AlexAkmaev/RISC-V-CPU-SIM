#include "instruction.h"

// Takes subset of a given bitset in range e.g. [x,x,xL,x,x,x,xR,x,x] -> N = 9, L = 6, R = 2
// returns bitset [xL,x,x,x,xR] -> N = L - R + 1 = 5
template<std::size_t L, std::size_t R, std::size_t N>
std::bitset<L - R + 1> sub_range(std::bitset<N> b) {
    static_assert(R >= 0 && R <= L && L <= N - 1, "invalid bitrange");
    std::bitset<L - R + 1> sub_set{b.to_string().substr(N - L - 1, L - R + 1)};
    return sub_set;
}

RISCVInstr::RISCVInstr(const std::bitset<32> i)  : instr_(i) {
    std::bitset<7> opcode = sub_range<6, 0>(instr_);
    switch (opcode.to_ulong()) {
        case 0b0110111:
            type_ = Format::U;
            op_ = Opcode::LUI;
            break;
        case 0b0010111:
            type_ = Format::U;
            op_ = Opcode::AUIPC;
            break;
        case 0b1101111:
            type_ = Format::J;
            op_ = Opcode::JAL;
            break;
        case 0b1100111:
            type_ = Format::I;
            op_ = Opcode::JALR;
            break;
        case 0b1100011:
            type_ = Format::B;
            SelectBranch();
            break;
    }
}

Opcode RISCVInstr::getOpcode() const {
    return op_;
}

RISCVInstr::Format RISCVInstr::getFormat() const {
    return type_;
}

void RISCVInstr::SelectBranch() {
    funct3_ = sub_range<14, 12>(instr_);
    switch (funct3_.to_ulong()) {
        case 0b000:
            op_ = Opcode::BEQ;
            break;
        case 0b001:
            op_ = Opcode::BNE;
            break;
        case 0b100:
            op_ = Opcode::BLT;
            break;
        case 0b101:
            op_ = Opcode::BGE;
            break;
        case 0b110:
            op_ = Opcode::BLTU;
            break;
        case 0b111:
            op_ = Opcode::BGEU;
            break;
        default:
            std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
            return;
    }
}

