#ifndef SIMULATOR_STAGE_H
#define SIMULATOR_STAGE_H
#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
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

struct PC {
    Stage::Type type = Stage::Type::Fetch;

    PC() : pc_(0) {}
    explicit PC(uint32_t pc) : pc_(pc) {}

    void operator+(uint32_t offset) {
        assert(offset % 4 == 0);
        pc_ += offset == 4 ? 1 : offset;
    }

    PC& operator=(uint32_t pc) {
        assert(pc % 4 == 0);
        pc_ = pc;
        return *this;
    }

    [[nodiscard]] uint32_t val() const {
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

class RegisterFile {
public:
    enum class Flags : bool {
        WE = true
    };

    void Write(std::bitset<5> A3, std::bitset<32> WE3) {
        uint8_t idx = A3.to_ulong();
        assert(static_cast<bool>(Flags::WE) && idx < 32);
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

/*======== Execute units ===========*/

/*======== Memory units ===========*/

#endif //SIMULATOR_STAGE_H
