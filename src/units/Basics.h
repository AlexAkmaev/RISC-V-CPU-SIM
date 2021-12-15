#ifndef SIMULATOR_STAGE_H
#define SIMULATOR_STAGE_H

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <bitset>
#include <variant>
#include <numeric>
#include <algorithm>
#include "riscv/instruction.h"

class Simulator;

class Stage {
public:
    virtual bool Run(Simulator &cpu) = 0;
    virtual bool Stall() = 0;
    virtual ~Stage() {}
};

// Takes subset of a given bitset in range e.g. [x,x,xL,x,x,x,xR,x,x] -> N = 9, L = 6, R = 2
// returns bitset [xL,x,x,x,xR] -> N = L - R + 1 = 5
template<std::size_t L, std::size_t R, std::size_t N>
std::bitset<L - R + 1> sub_range(std::bitset<N> b) {
    static_assert(R >= 0 && R <= L && L <= N - 1, "invalid bitrange");
    std::bitset<L - R + 1> sub_set{b.to_string().substr(N - L - 1, L - R + 1)};
    return sub_set;
}

// Collects one word from several bitsets
template<size_t N, size_t... N_Args>
std::bitset<N> concat(std::bitset<N_Args>... args) {
    std::vector<std::string> bit_params{args.to_string()...};
    std::string bit_representation_ans = std::accumulate(bit_params.begin(), bit_params.end(), std::string{});
    assert(bit_representation_ans.size() == N);
    return std::bitset<N>{bit_representation_ans};
}

template<std::size_t N>
std::bitset<1> SignBit(std::bitset<N> imm) {
    return imm[0] ? std::bitset<1>{1} : std::bitset<1>{0};
}

template<std::size_t N>
std::bitset<N> SignExt(std::bitset<1> SE) {
    std::bitset<N> output;
    return SE.all() ? output.set() : output;
}

template<size_t N>
std::bitset<N> operator+(std::bitset<N> lhs, std::bitset<N> rhs) {
    return std::bitset<N>{lhs.to_ulong() + rhs.to_ulong()};
}

/*======== Fetch units ===========*/

struct PC final {
    PC() : pc_(0) {}

    explicit PC(uint32_t pc) : pc_(pc) {}
    explicit PC(std::bitset<32> pc) : pc_(pc.to_ulong()) {}

    PC operator+(uint32_t offset) const noexcept {
        assert(offset % 4 == 0);
        uint32_t res_pc = pc_ + offset / 4;
        assert(res_pc >= 0);
        return PC{res_pc};
    }

    PC operator+(PC rhs) const noexcept {
        uint32_t res_pc = pc_ + rhs.val() / 4;
        assert(res_pc >= 0);
        return PC{res_pc};
    }

    PC &operator+=(uint32_t offset) noexcept {
        assert(offset % 4 == 0);
        pc_ += offset / 4;
        assert(pc_ >= 0);
        return *this;
    }

    PC &operator+=(std::bitset<32> offset) noexcept {
        pc_ += offset.to_ulong() / 4;
        assert(pc_ >= 0);
        return *this;
    }

    PC &operator=(uint32_t pc) noexcept {
        assert(pc % 4 == 0);
        pc_ = pc;
        return *this;
    }

    [[nodiscard]] uint32_t val() const noexcept {
        return pc_;
    }

private:
    uint32_t pc_{0};
};

class IMEM final {
public:
    explicit IMEM(uint32_t instr_count) : imem_(std::vector<std::bitset<32>>(instr_count)) {}
    explicit IMEM(std::vector<std::bitset<32>> &&imem) : imem_(std::move(imem)) {}

    void addToImem(const PC &pc, std::bitset<32> instr) {
        imem_[pc.val()] = instr;
    }

    std::bitset<32> getInstr(const PC &pc) {
        return imem_.at(pc.val());
    }

private:
    // Instructions memory
    std::vector<std::bitset<32>> imem_;
};

/*======== Decode units ===========*/

class ControlUnit;

class RegisterFile final {
public:
    void Write(std::bitset<5> A3, std::bitset<32> D3) {
        uint8_t idx = A3.to_ulong();
        assert(idx < 32);
        regs_[idx] = D3;
    }

    std::bitset<32> Read(std::bitset<5> A) {  // A is A1 or A2 - idx of source register
        uint8_t idx = A.to_ulong();
        assert(idx < 32);
        return regs_.at(idx);
    }

private:
    std::vector<std::bitset<32>> regs_ = std::vector<std::bitset<32>>(32);
};

/*======== Execute units ===========*/

class IMM final {
public:
    enum class Type {
        Imm_None,
        Imm_I,
        Imm_S,
        Imm_B,
        Imm_U,
        Imm_J
    };

    IMM() : type_(Type::Imm_None), imm_({}) {}
    explicit IMM(const RISCVInstr &instr) {
        std::bitset<32> ins_bits = instr.getInstr();
        switch (instr.getFormat()) {
            case RISCVInstr::Format::R:
                type_ = Type::Imm_None;
                break;
            case RISCVInstr::Format::I: {
                type_ = Type::Imm_I;
                imm_ = concat<32>(SignExt<21>(SignBit(ins_bits)), sub_range<30, 20>(ins_bits));
                break;
            }
            case RISCVInstr::Format::S: {
                type_ = Type::Imm_S;
                imm_ = concat<32>(SignExt<21>(SignBit(ins_bits)),
                                  sub_range<30, 25>(ins_bits),
                                  sub_range<11, 7>(ins_bits));
                break;
            }
            case RISCVInstr::Format::B: {
                type_ = Type::Imm_B;
                imm_ = concat<32>(SignExt<20>(SignBit(ins_bits)), sub_range<7, 7>(ins_bits),
                                  sub_range<30, 25>(ins_bits), sub_range<11, 8>(ins_bits), std::bitset<1>{0});
                break;
            }
            case RISCVInstr::Format::U: {
                type_ = Type::Imm_U;
                imm_ = concat<32>(sub_range<31, 12>(ins_bits), std::bitset<12>{0});
                break;
            }
            case RISCVInstr::Format::J: {
                type_ = Type::Imm_J;
                imm_ = concat<32>(SignExt<12>(SignBit(ins_bits)), sub_range<19, 12>(ins_bits),
                                  sub_range<20, 20>(ins_bits), sub_range<30, 21>(ins_bits), std::bitset<1>{0});
                break;
            }
        }
    }

    [[nodiscard]] std::bitset<32> getImm() const noexcept {
        assert(type_ != Type::Imm_None);
        return imm_;
    }

private:
    Type type_;
    std::bitset<32> imm_;
};

class ALU final {
public:
    enum class Op {
        ADD,
        SUB,
        XOR,
        OR,
        AND,
        SLL,
        SRL,
        SRA,
        SLT,
        SLTU
    };

    static std::bitset<32> calc(std::bitset<32> lhs, std::bitset<32> rhs, Op alu_op) {
        switch (alu_op) {
            case Op::ADD:
                return std::bitset<32>{lhs.to_ulong() + rhs.to_ulong()};
            case Op::SUB:
                return std::bitset<32>{lhs.to_ulong() - rhs.to_ulong()};
            case Op::XOR:
                return lhs ^ rhs;
            case Op::OR:
                return lhs | rhs;
            case Op::AND:
                return lhs & rhs;
            case Op::SLL:
                return lhs << rhs.to_ulong();
            case Op::SRL:
                return lhs >> rhs.to_ulong();
            case Op::SRA: {
                std::cerr << "TODO\n";
                return {};
//                std::bitset<32> res = lhs >> rhs.to_ulong();
//                /**??**/
//                res[0] = res[1];
//                return res;
            }
            case Op::SLT:
                return (static_cast<int32_t>(lhs.to_ulong()) < static_cast<int32_t>(rhs.to_ulong())) ? 1 : 0;
            case Op::SLTU:
                return (lhs.to_ulong() < rhs.to_ulong()) ? 1 : 0;
            default:
                std::cerr << "Incorrect ALU op\n";
                break;
        }
    }
};

class CMP final {
public:
    enum class Op {
        EQ,
        NE,
        LT,
        GE,
        LTU,
        GEU
    };

    static bool calc(std::bitset<32> lhs, std::bitset<32> rhs, Op cmp_op) {
        switch (cmp_op) {
            case Op::EQ:
                return lhs == rhs;
            case Op::NE:
                return lhs != rhs;
            case Op::LT:
                return static_cast<int32_t>(lhs.to_ulong()) < static_cast<int32_t>(rhs.to_ulong());
            case Op::GE:
                return static_cast<int32_t>(lhs.to_ulong()) >= static_cast<int32_t>(rhs.to_ulong());
            case Op::LTU:
                return lhs.to_ulong() < rhs.to_ulong();
            case Op::GEU:
                return lhs.to_ulong() >= rhs.to_ulong();
            default:
                std::cerr << "Incorrect CMP op\n";
                break;
        }
    }
};

class WE_GEN final {
public:
    WE_GEN() = default;
    explicit WE_GEN(bool mem_we, bool wb_we, bool v_ex) : mem_we_(mem_we && v_ex), wb_we_(wb_we && v_ex) {}

    [[nodiscard]] bool MEM_WE() const noexcept {
        return mem_we_;
    }

    [[nodiscard]] bool WB_WE() const noexcept {
        return wb_we_;
    }

private:
    bool mem_we_{true};
    bool wb_we_{true};
};

/*======== Memory units ===========*/

class DMEM final {
public:
    // 2^12 words
    DMEM() : dmem_(std::vector<std::bitset<32>>(4096)) {}

    void Store(std::bitset<32> WD, std::bitset<32> A) {
        dmem_.at(A.to_ulong()) = WD;
    }

    std::bitset<32> Load(std::bitset<32> A) {
        return dmem_.at(A.to_ulong());
    }

private:
    std::vector<std::bitset<32>> dmem_;
};

#endif //SIMULATOR_STAGE_H
