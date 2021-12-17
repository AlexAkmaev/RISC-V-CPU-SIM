#include "Fetch.h"
#include "simulator.h"

PipelineState Fetch::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::ERR;
    }

    ++cycle;
    if (cpu.decode_.getCUState().JALR && !pc_r_) {
        return PipelineState::BREAK;
    }

    if (cpu.hu_.PC_EN()) {
        instr_ = RISCVInstr{imem_.getInstr(pc_)};
        pc_r_ = cpu.execute_.PC_R();
        if (!pc_r_) {
            pc_next_ += 4;
        } else {
            pc_next_ = (cpu.execute_.JALR() ? PC{cpu.execute_.D1()} : cpu.execute_.PC_EX()) + cpu.execute_.PC_DISP();
        }
    }

    if (cpu.execute_.JALR() && cpu.execute_.WB_A() == std::bitset<5>{0}) {
        cpu.hu_.exception_state = PipelineState::BREAK;
    }

    // Prohibit data transmission through the register while stall
    if (!cpu.hu_.FD_EN()) {
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

void Fetch::setIMEM(IMEM &&imem) noexcept {
    imem_ = std::move(imem);
    is_set = true;
}

void Fetch::applyPC() noexcept {
    pc_ = pc_next_;
}
