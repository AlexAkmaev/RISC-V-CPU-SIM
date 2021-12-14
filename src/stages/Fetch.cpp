#include "Fetch.h"
#include "simulator.h"

bool Fetch::Run(Simulator &cpu) {
    if (!is_set) {
        return false;
    }

    instr_ = RISCVInstr{imem_.getInstr(pc_)};
    pc_r_ = cpu.execute_.PC_R();
    if (!pc_r_) {
        pc_next_ += 4;
    } else {
        pc_next_ = cpu.execute_.PC_EX() + cpu.execute_.PC_DISP();
    }

    cpu.FDtransmitData();

    is_set = false;
    return true;
}

bool Fetch::Stall() {
    return false;
}

RISCVInstr Fetch::getInstr() const noexcept {
    return instr_;
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
