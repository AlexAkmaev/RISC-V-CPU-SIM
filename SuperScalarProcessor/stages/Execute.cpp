#include "Execute.h"
#include "simulator.h"

PipelineState Execute::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::STALL;
    }

    wb_a_up_ = instrUp_.getRd();
    // Can't write to x0 reg
    if ((CONTROL_EX_Up_.JMP || CONTROL_EX_Up_.JALR) && wb_a_up_ == 0) {
        CONTROL_EX_Up_.WB_WE = false;
    }

    wb_a_down_ = instrDown_.getRd();
    // Can't write to x0 reg
    if ((CONTROL_EX_Down_.JMP || CONTROL_EX_Down_.JALR) && wb_a_down_ == 0) {
        CONTROL_EX_Down_.WB_WE = false;
    }

    we_gen_up_ = WE_GEN{CONTROL_EX_Up_.MEM_WE, CONTROL_EX_Up_.WB_WE, CONTROL_EX_Up_.EBREAK, v_ex_up_};
    we_gen_down_ = WE_GEN{CONTROL_EX_Down_.MEM_WE, CONTROL_EX_Down_.WB_WE, CONTROL_EX_Down_.EBREAK, v_ex_down_};

    immUp_ = IMM{instrUp_, CONTROL_EX_Up_.JALR};
    immDown_ = IMM{instrDown_, CONTROL_EX_Down_.JALR};
    PC_DISP_Up_ = PC{immUp_.getImm()};
    PC_DISP_Down_ = PC{immDown_.getImm()};

    cpu.hu_.setA1_A2_EX(instrUp_.getRs1(), instrUp_.getRs2());
    auto RS1V = ChooseRS(cpu.hu_.HU_RS1(), cpu);
    auto RS2V = ChooseRS(cpu.hu_.HU_RS2(), cpu);
    cpu.hu_.setA4_A5_EX(instrDown_.getRs1(), instrDown_.getRs2());
    auto RS4V = ChooseRS(cpu.hu_.HU_RS4(), cpu);
    auto RS5V = ChooseRS(cpu.hu_.HU_RS5(), cpu);
    cpu.fetch_.setD1(RS1V);
    cpu.fetch_.setD4(RS4V);
    rs2v_ = RS2V;
    rs5v_ = RS5V;

    std::bitset<32> alu_src1 = ChooseALU_SRC1(RS1V);
    std::bitset<32> alu_src2 = ChooseALU_SRC2(RS2V);
    alu_out_up_ = ALU::calc(alu_src1, alu_src2, CONTROL_EX_Up_.ALU_OP);

    std::bitset<32> alu_src4 = ChooseALU_SRC4(RS4V);
    std::bitset<32> alu_src5 = ChooseALU_SRC5(RS5V);
    alu_out_down_ = ALU::calc(alu_src4, alu_src5, CONTROL_EX_Down_.ALU_OP);

    bool compUp = CMP::calc(RS1V, RS2V, CONTROL_EX_Up_.CMP_OP);
    bool compDown = CMP::calc(RS4V, RS5V, CONTROL_EX_Down_.CMP_OP);

    PC_R_ = ((compUp && CONTROL_EX_Up_.BRANCH_COND) || CONTROL_EX_Up_.JMP || CONTROL_EX_Up_.JALR) && v_ex_up_;

    ProcessPrediction(cpu, Way::UP, compUp, compDown);

    if (PC_R_) {
        we_gen_down_.Invalidate();
        v_ex_down_ = false;
        cpu.fetch_.setPC_EX(PC_EX_Up_);
        cpu.fetch_.setPC_DISP(PC_DISP_Up_);
        cpu.fetch_.setJALR(CONTROL_EX_Up_.JALR, Way::UP);
    } else if (!restoreUp_) {
        PC_R_ = ((compDown && CONTROL_EX_Down_.BRANCH_COND) || CONTROL_EX_Down_.JMP || CONTROL_EX_Down_.JALR) && v_ex_down_;
        cpu.fetch_.setPC_EX(PC_EX_Up_ + 4);
        cpu.fetch_.setPC_DISP(PC_DISP_Down_);
        cpu.fetch_.setJALR(CONTROL_EX_Up_.JALR, Way::DOWN);
    }

    cpu.hu_.setHU_PC_REDIECT(PC_R_);

    ProcessPrediction(cpu, Way::DOWN, compUp, compDown);

    cpu.decode_.setPC_R(PC_R_);
    cpu.fetch_.setPC_R(PC_R_);

    cpu.hu_.CheckForStall(cpu);

    cpu.EMtransmitData();

    is_set = false;
    return PipelineState::OK;
}

void Execute::ProcessPrediction(Simulator &cpu, Way way, bool compUp, bool compDown) noexcept {
    bool is_control_instr = way == Way::UP ? v_ex_up_ && (CONTROL_EX_Up_.BRANCH_COND || CONTROL_EX_Up_.JMP) :
                                             v_ex_down_ && (CONTROL_EX_Down_.BRANCH_COND || CONTROL_EX_Down_.JMP);

    if (is_control_instr) {
        bool is_taken;
        if (way == Way::UP) {
            is_taken = cpu.hu_.getPredicton(PC_EX_Up_);
            restoreUp_ = is_taken && !PC_R_;
        } else {
            is_taken = cpu.hu_.getPredicton(PC_EX_Up_ + 4);
            restoreDown_ = is_taken && !PC_R_;
        }

        if (is_taken && PC_R_) {
            PC_R_ = false;
        }
    }

    if (CONTROL_EX_Up_.BRANCH_COND && v_ex_up_ && way == Way::UP) {
        cpu.hu_.setBranchPrediction(PC_EX_Up_, PC_DISP_Up_, compUp);
    } else if (CONTROL_EX_Up_.JMP && v_ex_up_ && way == Way::UP) {
        cpu.hu_.setBranchPrediction(PC_EX_Up_, PC_DISP_Up_, true);
    } else if (CONTROL_EX_Down_.BRANCH_COND && v_ex_down_ && way == Way::DOWN) {
        cpu.hu_.setBranchPrediction(PC_EX_Up_ + 4, PC_DISP_Down_, compDown);
    } else if (CONTROL_EX_Down_.JMP && v_ex_down_ && way == Way::DOWN) {
        cpu.hu_.setBranchPrediction(PC_EX_Up_ + 4, PC_DISP_Down_, true);
    }
}

std::bitset<32> Execute::ChooseRS(const HazardUnit::HU_RS &hu_rs, Simulator &cpu) const {
    switch (hu_rs) {
        case HazardUnit::HU_RS::D1:
            return {d1_};
        case HazardUnit::HU_RS::D2:
            return {d2_};
        case HazardUnit::HU_RS::D4:
            return {d4_};
        case HazardUnit::HU_RS::D5:
            return {d5_};
        case HazardUnit::HU_RS::BP_MEM_Up:
            return cpu.hu_.BP_MEM(Way::UP);
        case HazardUnit::HU_RS::BP_WB_Up:
            return cpu.hu_.BP_WB(Way::UP);
        case HazardUnit::HU_RS::BP_MEM_Down:
            return cpu.hu_.BP_MEM(Way::DOWN);
        case HazardUnit::HU_RS::BP_WB_Down:
            return cpu.hu_.BP_WB(Way::DOWN);
    }

    return {};
}

std::bitset<32> Execute::ChooseALU_SRC1(std::bitset<32> RS1V) {
    switch (CONTROL_EX_Up_.ALU_SRC1) {
        case 0:
            return RS1V;
        case 1:
            return std::bitset<32>{PC_EX_Up_.realVal()};  // PC for jal, jalr and auipc
        case 2:
            return {};  // 0 for lui
        default:
            std::cerr << "Unknown operand for ALU\n";
            return {};
    }
}

std::bitset<32> Execute::ChooseALU_SRC4(std::bitset<32> RS4V) {
    switch (CONTROL_EX_Down_.ALU_SRC1) {
        case 0:
            return RS4V;
        case 1: {
            auto pc = PC_EX_Up_ + 4;
            return std::bitset<32>{pc.realVal()};  // PC for jal, jalr and auipc
        }
        case 2:
            return {};  // 0 for lui
        default:
            std::cerr << "Unknown operand for ALU\n";
            return {};
    }
}

std::bitset<32> Execute::ChooseALU_SRC2(std::bitset<32> RS2V) {
    switch (CONTROL_EX_Up_.ALU_SRC2) {
        case 0:
            return RS2V;
        case 1:
            return immUp_.getImm();
        case 2:
            return std::bitset<32>{4};  // PC + 4 for jal
        default:
            std::cerr << "Unknown operand for ALU\n";
            return {};
    }
}

std::bitset<32> Execute::ChooseALU_SRC5(std::bitset<32> RS5V) {
    switch (CONTROL_EX_Down_.ALU_SRC2) {
        case 0:
            return RS5V;
        case 1:
            return immDown_.getImm();
        case 2:
            return std::bitset<32>{4};  // PC + 4 for jal
        default:
            std::cerr << "Unknown operand for ALU\n";
            return {};
    }
}

WE_GEN Execute::getWE_GEN(Way way) const noexcept {
    return way == Way::UP ? we_gen_up_ : we_gen_down_;
}

std::bitset<32> Execute::ALU_OUT(Way way) const noexcept {
    return way == Way::UP ? alu_out_up_ : alu_out_down_;
}

std::bitset<32> Execute::D1() const noexcept {
    return d1_;
}

std::bitset<32> Execute::D4() const noexcept {
    return d4_;
}

std::bitset<32> Execute::RS2V() const noexcept {
    return rs2v_;
}

std::bitset<32> Execute::RS5V() const noexcept {
    return rs5v_;
}

bool Execute::PC_R() const noexcept {
    return PC_R_;
}

PC Execute::PC_EX_Up() const noexcept {
    return PC_EX_Up_;
}

PC Execute::PC_DISP(Way way) const noexcept {
    return way == Way::UP ? PC_DISP_Up_ : PC_DISP_Down_;
}

bool Execute::JALR(Way way) const noexcept {
    return way == Way::UP ? CONTROL_EX_Up_.JALR : CONTROL_EX_Down_.JALR ;
}

bool Execute::WS(Way way) const noexcept {
    return way == Way::UP ? CONTROL_EX_Up_.WS && v_ex_up_ : CONTROL_EX_Down_.WS && v_ex_down_;
}

DMEM::Width Execute::MEM_WIDTH(Way way) const noexcept {
    return way == Way::UP ? CONTROL_EX_Up_.MEM_WIDTH : CONTROL_EX_Down_.MEM_WIDTH;
}

std::bitset<5> Execute::WB_A(Way way) const noexcept {
    return way == Way::UP ? wb_a_up_ : wb_a_down_;
}

void Execute::setD1_D2(std::bitset<32> d1, std::bitset<32> d2) {
    d1_ = d1;
    d2_ = d2;
}

void Execute::setD4_D5(std::bitset<32> d4, std::bitset<32> d5) {
    d4_ = d4;
    d5_ = d5;
}

void Execute::setInstr(const RISCVInstr &instr, Way way) {
    if (way == Way::UP) {
        instrUp_ = instr;
    } else {
        instrDown_ = instr;
    }
}

void Execute::setV_EX(bool v_ex_up, bool v_ex_down) {
    v_ex_up_ = v_ex_up;
    v_ex_down_ = v_ex_down;
}

void Execute::setPC_EX(const PC &pc) {
    PC_EX_Up_ = pc;
}

void Execute::setControl_EX(const ControlUnit::Flags &flags, Way way) {
    if (way == Way::UP) {
        CONTROL_EX_Up_ = flags;
    } else {
        CONTROL_EX_Down_ = flags;
    }
}

bool Execute::isRestore(Way way) const noexcept {
    return way == Way::UP ? restoreUp_ : restoreDown_;
}

RISCVInstr Execute::getInstr(Way way) const noexcept {
    return way == Way::UP ? instrUp_ : instrDown_;
}
