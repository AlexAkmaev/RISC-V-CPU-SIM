#include "instruction.h"
#include "units/Basics.h"
#include <cassert>

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
        case 0b0000011:
            type_ = Format::I;
            SelectL();
            break;
        case 0b0100011:
            type_ = Format::S;
            SelectS();
            break;
        case 0b0010011:
            type_ = Format::S;
            SelectII();
            break;
        case 0b0110011:
            type_ = Format::R;
            SelectR();
            break;
        case 0b1110011: {
            type_ = Format::I;
            op_ = instr_[11] ? op_ = Opcode::EBREAK : Opcode::ECALL;
            break;
        }
        default:
            std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
            return;
    }
}

Opcode RISCVInstr::getOpcode() const noexcept {
    return op_;
}

RISCVInstr::Format RISCVInstr::getFormat() const noexcept {
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

void RISCVInstr::SelectL() {
    funct3_ = sub_range<14, 12>(instr_);
    switch (funct3_.to_ulong()) {
        case 0b000:
            op_ = Opcode::LB;
            break;
        case 0b001:
            op_ = Opcode::LH;
            break;
        case 0b010:
            op_ = Opcode::LW;
            break;
        case 0b100:
            op_ = Opcode::LBU;
            break;
        case 0b101:
            op_ = Opcode::LHU;
            break;
        default:
            std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
            return;
    }
}

void RISCVInstr::SelectS() {
    funct3_ = sub_range<14, 12>(instr_);
    switch (funct3_.to_ulong()) {
        case 0b000:
            op_ = Opcode::SB;
            break;
        case 0b001:
            op_ = Opcode::SH;
            break;
        case 0b010:
            op_ = Opcode::SW;
            break;
        default:
            std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
            return;
    }
}

void RISCVInstr::SelectII() {
    funct3_ = sub_range<14, 12>(instr_);
    switch (funct3_.to_ulong()) {
        case 0b000:
            op_ = Opcode::ADDI;
            break;
        case 0b010:
            op_ = Opcode::SLTI;
            break;
        case 0b011:
            op_ = Opcode::SLTIU;
            break;
        case 0b100:
            op_ = Opcode::XORI;
            break;
        case 0b110:
            op_ = Opcode::ORI;
            break;
        case 0b111:
            op_ = Opcode::ANDI;
            break;
        case 0b001:
            type_ = Format::R;
            op_ = Opcode::SLLI;
            break;
        case 0b101: {
            type_ = Format::R;
            funct7_ = sub_range<31, 25>(instr_);
            switch (funct7_.to_ulong()) {
                case 0b0000000:
                    op_ = Opcode::SRLI;
                    break;
                case 0b0100000:
                    op_ = Opcode::SRAI;
                    break;
                default:
                    std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
                    return;
            }
            break;
        }
        default:
            std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
            return;
    }
}

void RISCVInstr::SelectR() {
    funct3_ = sub_range<14, 12>(instr_);
    funct7_ = sub_range<31, 25>(instr_);
    switch (funct3_.to_ulong()) {
        case 0b000: {
            switch (funct7_.to_ulong()) {
                case 0b0000000:
                    op_ = Opcode::ADD;
                    break;
                case 0b0100000:
                    op_ = Opcode::SUB;
                    break;
                default:
                    std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
                    return;
            }
            break;
        }
        case 0b001:
            op_ = Opcode::SLL;
            break;
        case 0b010:
            op_ = Opcode::SLT;
            break;
        case 0b011:
            op_ = Opcode::SLTU;
            break;
        case 0b100:
            op_ = Opcode::XOR;
            break;
        case 0b101: {
            switch (funct7_.to_ulong()) {
                case 0b0000000:
                    op_ = Opcode::SRL;
                    break;
                case 0b0100000:
                    op_ = Opcode::SRA;
                    break;
                default:
                    std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
                    return;
            }
            break;
        }
        case 0b110:
            op_ = Opcode::OR;
            break;
        case 0b111:
            op_ = Opcode::AND;
            break;
        default:
            std::cerr << "Invalid instruction: " << instr_.to_string() << "\n";
            return;
    }
}

std::bitset<5> RISCVInstr::getRs1() const noexcept {
    assert(type_ == Format::R || type_ == Format::I || type_ == Format::S || type_ == Format::B);
    std::bitset<5> rs1 = sub_range<19, 15>(instr_);
    return rs1;
}

std::bitset<5> RISCVInstr::getRs2() const noexcept {
    assert(type_ == Format::R || type_ == Format::S || type_ == Format::B);
    std::bitset<5> rs2 = sub_range<24, 20>(instr_);
    return rs2;
}

std::bitset<5> RISCVInstr::getRd() const noexcept {
    assert(type_ != Format::S || type_ != Format::B);
    std::bitset<5> rd = sub_range<11, 7>(instr_);
    return rd;
}

std::bitset<3> RISCVInstr::getFunct3() const noexcept {
    assert(type_ != Format::U && type_ != Format::J);
    return funct3_;
}

std::bitset<7> RISCVInstr::getFunct7() const noexcept {
    assert(type_ == Format::R);
    return funct7_;
}

std::bitset<32> RISCVInstr::getInstr() const noexcept {
    return instr_;
}
