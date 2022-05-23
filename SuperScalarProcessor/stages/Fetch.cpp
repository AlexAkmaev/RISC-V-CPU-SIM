#include "Fetch.h"
#include "simulator.h"
#include "instruction.h"

PipelineState Fetch::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::OK;
    }

    std::bitset<32> inputUp{/* ebreak */ 0x100073}, inputDown{/* ebreak */ 0x100073};
    if (imem_.isEndOfIMEM(pc_up_next_)) {
        cpu.hu_.sendEndOfIMEM();
    } else if (imem_.isEndOfIMEM(pc_up_next_ + 4)) {
        inputUp = imem_.getInstr(pc_up_);
    } else {
        inputUp = imem_.getInstr(pc_up_);
        inputDown = imem_.getInstr(pc_up_ + 4);
    }
    instrUp_ = RISCVInstr(inputUp);
    instrDown_ = RISCVInstr(inputDown);

    uint8_t pc_increment = cpu.hu_.pl_state == PipelineState::STALL_DOWN ? 4 : 8;
    if (cpu.hu_.PC_EN()) {
//        bool prediction = cpu.hu_.getPredicton(pc_up_);
//        if (cpu.execute_.isRestore()) {
//            pc_up_next_ = pc_ex_ + 4;
//        } else if (prediction) {
//            pc_up_next_ = cpu.hu_.getTarget(prediction, pc_up_);
//        } else 
            if (!pc_r_) {
                // !!!!
            pc_up_next_ += pc_increment;
        } else {
            uint32_t reg_pc = jalrUp_ ? d1_.to_ulong() : d4_.to_ulong();
            pc_up_next_ = ((jalrUp_ || jalrDown_) ? PC{reg_pc / 4 - 1} : pc_ex_) + pc_disp_;
        }
    }

    // Prohibit data transmission through the register while stall
    if (!cpu.hu_.FD_EN()) {
        cpu.decode_.is_set = true;
        ++cycle;
        return PipelineState::STALL;
    }

    if (cpu.hu_.pl_state == PipelineState::STALL_DOWN) {
        // Shift Data
        instrDown_ = instrUp_;
    }

    cpu.FDtransmitData();

    return PipelineState::OK;
}

RISCVInstr Fetch::getInstr(Way way) const noexcept {
    return way == Way::UP ? instrUp_ : instrDown_;
}

PC Fetch::getPC_Up() const noexcept {
    return pc_up_;
}

PC Fetch::getNextPC_Up() const noexcept {
    return pc_up_next_;
}

bool Fetch::PC_R() const noexcept {
    return pc_r_;
}

void Fetch::setPC_R(bool pc_r) noexcept {
    pc_r_ = pc_r;
}

void Fetch::setJALR(bool jalr, Way way) noexcept {
    if (way == Way::UP) {
        jalrUp_ = jalr;
    } else {
        jalrDown_ = jalr;
    }
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

void Fetch::setD4(std::bitset<32> d4) noexcept {
    d4_ = d4;
}

void Fetch::setIMEM(IMEM &&imem) noexcept {
    imem_ = std::move(imem);
    is_set = true;
}

void Fetch::applyPC() noexcept {
    pc_up_ = pc_up_next_;
}
