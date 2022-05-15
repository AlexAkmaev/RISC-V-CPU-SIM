#include "Fetch.h"
#include "simulator.h"
#include "instruction.h"

PipelineState Fetch::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::OK;
    }

    if (imem_.isEndOfIMEM(pc_next_)) {
        instr_ = RISCVInstr(std::bitset<32>{/* ebreak */ 0x100073});
        cpu.hu_.sendEndOfIMEM();
    } else {
        instr_ = RISCVInstr{imem_.getInstr(pc_)};
    }

    if (cpu.hu_.PC_EN()) {
        bool prediction = cpu.hu_.getPredicton(pc_);
        if (cpu.execute_.isRestore()) {
            pc_next_ = pc_ex_ + 4;
        } else if (prediction) {
            pc_next_ = cpu.hu_.getTarget(prediction, pc_);
        } else if (!pc_r_) {
            pc_next_ += 4;
        } else {
            pc_next_ = (jalr_ ? PC{static_cast<uint32_t>(d1_.to_ulong()) / 4 - 1} : pc_ex_) + pc_disp_;
        }
    }

    // Prohibit data transmission through the register while stall
    if (!cpu.hu_.FD_EN()) {
        cpu.decode_.is_set = true;
        ++cycle;
        return PipelineState::STALL;
    }

    cpu.FDtransmitData();

    return PipelineState::OK;
}

RISCVInstr Fetch::getInstr() const noexcept {
    return instr_;
}

PC Fetch::getPC() const noexcept {
    return pc_;
}

PC Fetch::getNextPC() const noexcept {
    return pc_next_;
}

bool Fetch::PC_R() const noexcept {
    return pc_r_;
}

void Fetch::setPC_R(bool pc_r) noexcept {
    pc_r_ = pc_r;
}

void Fetch::setJALR(bool jalr) noexcept {
    jalr_ = jalr;
}

void Fetch::setPC_EX(PC pc_ex) noexcept {
    pc_ex_ = pc_ex;
}

void Fetch::setPC_DISP(PC pc_disp) noexcept {
    pc_disp_ = pc_disp;
}

void Fetch::setD1(std::bitset<32> d1) noexcept {
    d1_ = d1;
}

void Fetch::setIMEM(IMEM &&imem) noexcept {
    imem_ = std::move(imem);
    is_set = true;
}

void Fetch::applyPC() noexcept {
    pc_ = pc_next_;
}
