#include "Execute.h"
#include "simulator.h"

bool Execute::Run(Simulator &cpu) {
    if (!is_set) {
        return false;
    }

    imm_ = IMM{instr_};
    auto RS1V = ChooseRS(true, cpu.hu_.HU_RS1(), cpu);
    auto RS2V = ChooseRS(false, cpu.hu_.HU_RS2(), cpu);

    std::bitset<32> alu_src1;
    std::bitset<32> alu_src2;
    if (CONTROL_EX_.ALU_SRC2) {
        alu_src2 = RS2V;
    } else {
        alu_src2 = imm_.getImm();
    }

    alu_out_ = ALU::calc(alu_src1, alu_src2, CONTROL_EX_.ALU_OP);
    PC_R_ = CMP::calc(RS1V, RS2V, CONTROL_EX_.CMP_OP) && CONTROL_EX_.BRANCH_COND;

    is_set = false;
    return true;
}

std::bitset<32> Execute::ChooseRS(bool rs, const HazardUnit::HU_RS &hu_rs, Simulator &cpu) const {
    switch (hu_rs) {
        case HazardUnit::HU_RS::D1:
            return rs ? D1 : D2;
        case HazardUnit::HU_RS::BP_MEM:
            return cpu.memory_.BP_MEM();
        case HazardUnit::HU_RS::BP_WB:
            return cpu.write_back_.BP_WB();
    }
}

bool Execute::Stall() {
    return true;
}

bool Execute::PC_R() const noexcept {
    return PC_R_;
}

PC Execute::PC_EX() const noexcept {
    return PC_EX_;
}

PC Execute::PC_DISP() const noexcept {
    return PC_DISP_;
}

bool Execute::V_EX() const noexcept {
    return V_EX_;
}

void Execute::setD1_D2(std::bitset<32> d1, std::bitset<32> d2) {
    D1 = d1;
    D2 = d2;
}

void Execute::setInstr(const RISCVInstr &instr) {
    instr_ = instr;
}

void Execute::setPC_EX(const PC &pc) {
    PC_EX_ = pc;
}

void Execute::setControl_EX(const ControlUnit::Flags &flags) {
    CONTROL_EX_ = flags;
}
