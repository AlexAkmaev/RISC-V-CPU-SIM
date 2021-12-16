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

PipelineState Simulator::Run() {
    PipelineState state;
    while (true) {
        fetch_.Run(*this);
        decode_.Run(*this);
        execute_.Run(*this);
        memory_.Run(*this);
        write_back_.Run(*this);
    }
    return state;
}

void Simulator::FDtransmitData() {
    decode_.setInstr(fetch_.getInstr());
    decode_.setPC(fetch_.getNextPC());
    decode_.setPC_R(fetch_.PC_R());
    decode_.is_set = true;
}

void Simulator::DEtransmitData() {
    execute_.setV_EX(decode_.V_DE());
    execute_.setD1_D2(decode_.getRD1(), decode_.getRD2());
    execute_.setInstr(decode_.getInstr());
    execute_.setPC_EX(decode_.getPC());
    execute_.setControl_EX(decode_.getCUState());
    execute_.is_set = true;
}

void Simulator::EMtransmitData() {
    memory_.setWE_GEN(execute_.getWE_GEN());
    memory_.setWS(execute_.WS());
    memory_.setLWidth(execute_.MEM_WIDTH());
    memory_.setD2(execute_.D2());
    memory_.setALU_OUT(execute_.ALU_OUT());
    memory_.setWB_A(execute_.WB_A());
    memory_.is_set = true;
}

void Simulator::MWBtransmitData() {
    write_back_.setWB_WE(memory_.WB_WE());
    write_back_.setWB_D(memory_.getOutData());
    write_back_.setWB_A(memory_.WB_A());
    write_back_.is_set = true;
}
