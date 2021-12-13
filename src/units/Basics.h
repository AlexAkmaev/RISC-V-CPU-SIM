#ifndef SIMULATOR_STAGE_H
#define SIMULATOR_STAGE_H

#include <iostream>
#include <vector>
#include <cassert>
#include <bitset>
#include <variant>
#include "ContolUnit.h"

class Stage {
public:
    enum class Type {
        Fetch,
        Decode,
        Execute,
        Memory
    };

    virtual bool Run() = 0;

    virtual bool Stall() = 0;
};

// Takes subset of a given bitset in range e.g. [x,x,xL,x,x,x,xR,x,x] -> N = 9, L = 6, R = 2
// returns bitset [xL,x,x,x,xR] -> N = L - R + 1 = 5
template<std::size_t L, std::size_t R, std::size_t N>
std::bitset<L - R + 1> sub_range(std::bitset<N> b) {
    static_assert(R >= 0 && R <= L && L <= N - 1, "invalid bitrange");
    std::bitset<L - R + 1> sub_set{b.to_string().substr(N - L - 1, L - R + 1)};
    return sub_set;
}

/*======== Fetch units ===========*/

struct PC final {
    Stage::Type type = Stage::Type::Fetch;

    PC() : pc_(0) {}

    explicit PC(uint32_t pc) : pc_(pc) {}

    PC operator+(uint32_t offset) const noexcept {
        assert(offset % 4 == 0);
        uint32_t res_pc = pc_ + offset / 4;
        assert(res_pc >= 0);
        return PC{res_pc};
    }

    PC &operator+=(uint32_t offset) noexcept {
        assert(offset % 4 == 0);
        pc_ += offset / 4;
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
    enum class Flags : bool {
        WE3 = true
    };

    void Write(std::bitset<5> A3, std::bitset<32> WE3) {
        uint8_t idx = A3.to_ulong();
        assert(static_cast<bool>(Flags::WE3) && idx < 32);
        regs_[idx] = WE3;
    }

    std::bitset<32> Read(std::bitset<5> A) {  // A is A1 or A2 - idx of source register
        uint8_t idx = A.to_ulong();
        assert(idx < 32);
        return regs_.at(idx);
    }

private:
    std::vector<std::bitset<32>> regs_ = std::vector<std::bitset<32>>(32);
};

class IMM {
public:
    explicit IMM(const RISCVInstr &instr) {
        std::bitset<32> ins_bits = instr.getInstr();
        switch (instr.getFormat()) {
            case RISCVInstr::Format::R:
                /*== no imm ==*/
                break;
            case RISCVInstr::Format::I:
                imm_ = sub_range<31, 20>(ins_bits);
                break;
            case RISCVInstr::Format::S:
                imm_ = std::bitset<12>{sub_range<31, 25>(ins_bits).to_string() +
                                       sub_range<11, 7>(ins_bits).to_string()};
                break;
            case RISCVInstr::Format::B:
                imm_ = std::bitset<13>{to_string(ins_bits[0]) +
                                       to_string(ins_bits[31 - 7]) +
                                       sub_range<30, 25>(ins_bits).to_string() +
                                       sub_range<11, 8>(ins_bits).to_string()};
                break;
            case RISCVInstr::Format::U:
                imm_ = sub_range<31, 12>(ins_bits);
                break;
            case RISCVInstr::Format::J:
                imm_ = std::bitset<20>{to_string(ins_bits[0]) +
                                       sub_range<19, 12>(ins_bits).to_string() +
                                       to_string(ins_bits[31 - 20]) +
                                       sub_range<30, 21>(ins_bits).to_string()};
                break;
        }
    }

    template<class T>
    T getImm() const noexcept {
        return std::get<T>(imm_);
    }

private:
    std::variant<std::bitset<12>, std::bitset<13>, std::bitset<21>, std::bitset<20>> imm_;
};

/*======== Execute units ===========*/

class ALU final {
public:

private:
    std::bitset<3> op_;
};

/*======== Memory units ===========*/

class DMEM final {
public:
    // dmem_ can consists of 2^30 words, but to simplify lets make only 2^10
    DMEM() : dmem_(std::vector<std::bitset<32>>(1024)) {}

private:
    std::vector<std::bitset<32>> dmem_;
};

#endif //SIMULATOR_STAGE_H
