#include "ContolUnit.h"

void ControlUnit::setState(const RISCVInstr &instr) {
    switch (instr.getFormat()) {
        case RISCVInstr::Format::R:
            flags.ALU_SRC2 = true;
            SelectALUOp(instr);
            break;
        case RISCVInstr::Format::I:
            flags.ALU_SRC2 = false;
            SelectWS(instr);
            break;
        case RISCVInstr::Format::S:
            flags.WB_WE = false;
            flags.ALU_SRC2 = true;
            flags.MEM_WE = true;
            break;
        case RISCVInstr::Format::B:
            flags.WB_WE = false;
            flags.ALU_SRC2 = true;
            SelectCMPOp(instr);
            flags.BRANCH_COND = true;
            break;
        case RISCVInstr::Format::U:
            flags.ALU_SRC2 = true;
            break;
        case RISCVInstr::Format::J:
            flags.ALU_SRC2 = true;
            flags.JMP = true;
            break;
    }
}

void ControlUnit::SelectALUOp(const RISCVInstr &instr) {
    assert(instr.getFormat() == RISCVInstr::Format::R || instr.getFormat() == RISCVInstr::Format::I);
    switch (instr.getOpcode()) {
        case Opcode::ADD:
        case Opcode::ADDI:
            flags.ALU_OP = ALU::Op::ADD;
            break;
        case Opcode::SUB:
            flags.ALU_OP = ALU::Op::SUB;
            break;
        case Opcode::XOR:
        case Opcode::XORI:
            flags.ALU_OP = ALU::Op::XOR;
            break;
        case Opcode::OR:
        case Opcode::ORI:
            flags.ALU_OP = ALU::Op::OR;
            break;
        case Opcode::AND:
        case Opcode::ANDI:
            flags.ALU_OP = ALU::Op::AND;
            break;
        case Opcode::SLL:
        case Opcode::SLLI:
            flags.ALU_OP = ALU::Op::SLL;
            break;
        case Opcode::SRL:
        case Opcode::SRLI:
            flags.ALU_OP = ALU::Op::SRL;
            break;
        case Opcode::SRA:
        case Opcode::SRAI:
            flags.ALU_OP = ALU::Op::SRA;
            break;
        case Opcode::SLT:
        case Opcode::SLTI:
            flags.ALU_OP = ALU::Op::SLT;
            break;
        case Opcode::SLTU:
        case Opcode::SLTIU:
            flags.ALU_OP = ALU::Op::SLTU;
            break;
        default:
            std::cerr << "Wrong opcode for ALU" << std::endl;
            return;
    }
}

void ControlUnit::SelectCMPOp(const RISCVInstr &instr) {
    switch (instr.getOpcode()) {
        case Opcode::BEQ:
            flags.CMP_OP = CMP::Op::EQ;
            break;
        case Opcode::BNE:
            flags.CMP_OP = CMP::Op::NE;
            break;
        case Opcode::BLT:
            flags.CMP_OP = CMP::Op::LT;
            break;
        case Opcode::BGE:
            flags.CMP_OP = CMP::Op::GE;
            break;
        case Opcode::BLTU:
            flags.CMP_OP = CMP::Op::LTU;
            break;
        case Opcode::BGEU:
            flags.CMP_OP = CMP::Op::GEU;
            break;
        default:
            std::cerr << "Wrong opcode for CMP" << std::endl;
            return;
    }
}

void ControlUnit::SelectWS(const RISCVInstr &instr) {
    switch (instr.getOpcode()) {
        case Opcode::LB:
        case Opcode::LH:
        case Opcode::LW:
        case Opcode::LBU:
        case Opcode::LHU:
            flags.WS = true;
            break;
        default:
            flags.WS = false;
            return;
    }
}
