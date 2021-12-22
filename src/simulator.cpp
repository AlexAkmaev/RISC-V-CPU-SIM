#include "simulator.h"

Simulator::Simulator(uint32_t instr_count) {
    fetch_ = Fetch{instr_count};
    decode_ = Decode{};
    execute_ = Execute{};
    memory_ = Memory{};
    write_back_ = WriteBack{};
}

Simulator::Simulator(std::vector<std::bitset<32>> &&imem) {
    fetch_ = Fetch{std::move(imem)};
    decode_ = Decode{};
    execute_ = Execute{};
    memory_ = Memory{};
    write_back_ = WriteBack{};
}

#define ASSERT_STATE(Run)                           \
    state = (Run);                                  \
    if (state == PipelineState::ERR) {              \
        return PipelineState::ERR;                  \
    } else if (state == PipelineState::BREAK) {     \
        return PipelineState::OK;                   \
    }

PipelineState Simulator::Run() {
    PipelineState state;
    while (true) {
        ASSERT_STATE(hu_.exception_state)
        ASSERT_STATE(write_back_.Run(*this))
        ASSERT_STATE(memory_.Run(*this))
        ASSERT_STATE(execute_.Run(*this))
        ASSERT_STATE(decode_.Run(*this))
        ASSERT_STATE(fetch_.Run(*this))
    }
}

#define CYCLE_CONTROL(prev, next)           \
    ++(prev);                               \
    if ((prev) - (next) < 1) {              \
        return;                             \
    }

void Simulator::FDtransmitData() {
    CYCLE_CONTROL(fetch_.cycle, decode_.cycle)
    decode_.setInstr(fetch_.getInstr());
    decode_.setPC(fetch_.getPC());
    decode_.setPC_R_F(fetch_.PC_R());
    fetch_.applyPC();
    decode_.is_set = true;
}

void Simulator::DEtransmitData() {
    CYCLE_CONTROL(decode_.cycle, execute_.cycle)
    execute_.setV_EX(decode_.V_DE());
    execute_.setD1_D2(decode_.getRD1(), decode_.getRD2());
    execute_.setInstr(decode_.getInstr());
    execute_.setPC_EX(decode_.getPC());
    execute_.setControl_EX(decode_.getCUState());
    execute_.is_set = true;
}

void Simulator::EMtransmitData() {
    CYCLE_CONTROL(execute_.cycle, memory_.cycle)
    memory_.setWE_GEN(execute_.getWE_GEN());
    memory_.setWS(execute_.WS());
    memory_.setLWidth(execute_.MEM_WIDTH());
    memory_.setD2(execute_.RS2V());
    memory_.setALU_OUT(execute_.ALU_OUT());
    memory_.setWB_A(execute_.WB_A());
    memory_.is_set = true;
}

void Simulator::MWBtransmitData() {
    CYCLE_CONTROL(memory_.cycle, write_back_.cycle)
    write_back_.setWB_WE(memory_.WB_WE());
    write_back_.setEBREAK(memory_.EBREAK());
    write_back_.setWB_D(memory_.getOutData());
    write_back_.setWB_A(memory_.WB_A());
    write_back_.is_set = true;
}
