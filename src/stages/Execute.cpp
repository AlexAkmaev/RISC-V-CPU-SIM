#include "Execute.h"
#include "simulator.h"

PipelineState Execute::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::STALL;
    }

    we_gen_ = WE_GEN{CONTROL_EX_.MEM_WE, CONTROL_EX_.WB_WE, CONTROL_EX_.EBREAK, v_ex_};

    wb_a_ = instr_.getRd();
    imm_ = IMM{instr_, CONTROL_EX_.JALR};
    PC_DISP_ = PC{imm_.getImm()};

    cpu.hu_.setA1_A2_EX(instr_.getRs1(), instr_.getRs2());
    auto RS1V = ChooseRS(cpu.hu_.HU_RS1(cpu), cpu);
    auto RS2V = ChooseRS(cpu.hu_.HU_RS2(cpu), cpu);
    cpu.fetch_.setD1(RS1V);
    rs2v_ = RS2V;

    std::bitset<32> alu_src1 = ChooseALU_SRC1(RS1V);
    std::bitset<32> alu_src2 = ChooseALU_SRC2(RS2V);

    alu_out_ = ALU::calc(alu_src1, alu_src2, CONTROL_EX_.ALU_OP);
    PC_R_ = CMP::calc(RS1V, RS2V, CONTROL_EX_.CMP_OP) &&
            CONTROL_EX_.BRANCH_COND || CONTROL_EX_.JMP && v_ex_ || CONTROL_EX_.JALR && v_ex_;
    cpu.hu_.setHU_PC_REDIECT(PC_R_);

    cpu.decode_.setPC_R(PC_R_);
    cpu.fetch_.setPC_R(PC_R_);
    cpu.fetch_.setPC_EX(PC_EX_);
    cpu.fetch_.setPC_DISP(PC_DISP_);
    cpu.fetch_.setJALR(CONTROL_EX_.JALR);

    cpu.hu_.CheckForStall(CONTROL_EX_.WS, wb_a_, cpu);

    cpu.EMtransmitData();

    is_set = false;
    return PipelineState::OK;
}

std::bitset<32> Execute::ChooseRS(const HazardUnit::HU_RS &hu_rs, Simulator &cpu) const {
    switch (hu_rs) {
        case HazardUnit::HU_RS::D1:
            return {d1_};
        case HazardUnit::HU_RS::D2:
            return {d2_};
        case HazardUnit::HU_RS::BP_MEM:
            return cpu.hu_.BP_MEM();
        case HazardUnit::HU_RS::BP_WB:
            return cpu.hu_.BP_WB();
        case HazardUnit::HU_RS::BP_RD:
            return cpu.hu_.BP_RD();
    }
}

std::bitset<32> Execute::ChooseALU_SRC1(std::bitset<32> RS1V) {
    switch (CONTROL_EX_.ALU_SRC1) {
        case 0:
            return RS1V;
        case 1:
            return std::bitset<32>{PC_EX_.realVal()};  // PC for jal, jalr and auipc
        case 2:
            return {};  // 0 for lui
        default:
            std::cerr << "Unknown operand for ALU\n";
            return {};
    }
}

std::bitset<32> Execute::ChooseALU_SRC2(std::bitset<32> RS2V) {
    switch (CONTROL_EX_.ALU_SRC2) {
        case 0:
            return RS2V;
        case 1:
            return imm_.getImm();
        case 2:
            return std::bitset<32>{4};  // PC + 4 for jal
        default:
            std::cerr << "Unknown operand for ALU\n";
            return {};
    }
}

WE_GEN Execute::getWE_GEN() const noexcept {
    return we_gen_;
}

std::bitset<32> Execute::ALU_OUT() const noexcept {
    return alu_out_;
}

std::bitset<32> Execute::D1() const noexcept {
    return d1_;
}

std::bitset<32> Execute::RS2V() const noexcept {
    return rs2v_;
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

bool Execute::JALR() const noexcept {
    return CONTROL_EX_.JALR;
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
