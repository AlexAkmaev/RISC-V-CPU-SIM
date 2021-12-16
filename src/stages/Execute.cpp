#include "Execute.h"
#include "simulator.h"

PipelineState Execute::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::ERR;
    }

    we_gen_ = WE_GEN{CONTROL_EX_.MEM_WE, CONTROL_EX_.WB_WE, v_ex_};

    wb_a_ = instr_.getRd();
    imm_ = IMM{instr_};
    PC_DISP_ = PC{imm_.getImm()};

    cpu.hu_.setA1_A2_EX(instr_.getRs1(), instr_.getRs2());
    auto RS1V = ChooseRS(cpu.hu_.HU_RS1(cpu), cpu);
    auto RS2V = ChooseRS(cpu.hu_.HU_RS2(cpu), cpu);

    std::bitset<32> alu_src1;
    std::bitset<32> alu_src2;
    if (CONTROL_EX_.ALU_SRC2) {
        alu_src2 = RS2V;
    } else {
        alu_src2 = imm_.getImm();
    }

    alu_out_ = ALU::calc(alu_src1, alu_src2, CONTROL_EX_.ALU_OP);
    PC_R_ = CMP::calc(RS1V, RS2V, CONTROL_EX_.CMP_OP) && CONTROL_EX_.BRANCH_COND || CONTROL_EX_.JMP;

    cpu.EMtransmitData();

    is_set = false;
    return PipelineState::OK;
}

std::bitset<32> Execute::ChooseRS(const HazardUnit::HU_RS &hu_rs, Simulator &cpu) const {
    switch (hu_rs) {
        case HazardUnit::HU_RS::D1:
            return d1_;
        case HazardUnit::HU_RS::D2:
            return d2_;
        case HazardUnit::HU_RS::BP_MEM:
            return cpu.memory_.ALU_OUT();
        case HazardUnit::HU_RS::BP_WB:
            return cpu.write_back_.WB_D();
    }
}

WE_GEN Execute::getWE_GEN() const noexcept {
    return we_gen_;
}

std::bitset<32> Execute::ALU_OUT() const noexcept {
    return alu_out_;
}

std::bitset<32> Execute::D2() const noexcept {
    return d2_;
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

bool Execute::WS() const noexcept {
    return CONTROL_EX_.WS;
}

DMEM::Width Execute::MEM_WIDTH() const noexcept {
    return CONTROL_EX_.MEM_WIDTH;
}

std::bitset<5> Execute::WB_A() const noexcept {
    return wb_a_;
}

void Execute::setD1_D2(std::bitset<32> d1, std::bitset<32> d2) {
    d1_ = d1;
    d2_ = d2;
}

void Execute::setInstr(const RISCVInstr &instr) {
    instr_ = instr;
}

void Execute::setV_EX(bool v_ex) {
    v_ex_ = v_ex;
}

void Execute::setPC_EX(const PC &pc) {
    PC_EX_ = pc;
}

void Execute::setControl_EX(const ControlUnit::Flags &flags) {
    CONTROL_EX_ = flags;
}
