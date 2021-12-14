#include "Fetch.h"
#include "simulator.h"

bool Fetch::Run(Simulator &cpu) {
    if (!is_set) {
        return false;
    }

    instr_ = RISCVInstr{imem_.getInstr(pc_)};
    if (!cpu.execute_.PC_R()) {
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

void Fetch::setIMEM(IMEM &&imem) noexcept {
    imem_ = std::move(imem);
    is_set = true;
}
