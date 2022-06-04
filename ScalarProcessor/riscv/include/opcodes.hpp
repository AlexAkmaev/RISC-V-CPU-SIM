#ifndef SIMULATOR_OPCODES_H
#define SIMULATOR_OPCODES_H

#include <cstdint>

// See official doc for formats at 130 p. in
// https://github.com/riscv/riscv-isa-manual/releases/download/Ratified-IMAFDQC/riscv-spec-20191213.pdf
// or https://github.com/riscv/riscv-opcodes/blob/master/opcodes-rv32i

enum class Opcode : uint8_t {
    LUI,
    AUIPC,
    JAL,
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    LB,
    LH,
    LW,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    ADDI,
    SLTI,
    SLTIU,
    XORI,
    ORI,
    ANDI,
    SLLI,
    SRLI,
    SRAI,
    ADD,
    SUB,
    SLL,
    SLT,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    ECALL,
    EBREAK
};

#endif //SIMULATOR_OPCODES_H
