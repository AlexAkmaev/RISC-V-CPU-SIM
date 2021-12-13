#include "Fetch.h"

bool Fetch::Run() {
    assert(is_set);
    instr_ = RISCVInstr{imem_.getInstr(pc_)};
    if (!static_cast<bool>(Flags::PC_R)) {
        pc_next_ += 4;
    } else {
        pc_next_ += sign_ext;
    }
    return true;
}

RISCVInstr Fetch::getInstr() const noexcept {
    return instr_;
}

PC Fetch::getNextPC() const noexcept {
    return pc_next_;
}

PC Fetch::getDispPC() const noexcept {
    return pc_;
}

void Fetch::setIMEM(IMEM &&imem) noexcept {
    imem_ = std::move(imem);
    is_set = true;
}
