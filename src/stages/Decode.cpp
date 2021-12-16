#include "Decode.h"
#include "simulator.h"

PipelineState Decode::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::ERR;
    }

    cu_.setState(instr_);
    cpu.hu_.setA1_A2_D(instr_.getRs1(), instr_.getRs2());

    D1 = reg_file_.Read(instr_.getRs1());
    D2 = reg_file_.Read(instr_.getRs2());
    if (cpu.write_back_.WB_WE()) {
        reg_file_.Write(cpu.write_back_.WB_A(), cpu.write_back_.WB_D());
    }

    v_de_ = !(pc_f_ || cpu.execute_.PC_R());
    cpu.DEtransmitData();

    is_set = false;
    return PipelineState::OK;
}

ControlUnit::Flags Decode::getCUState() const noexcept {
    return cu_.flags;
}

std::bitset<32> Decode::getRD1() const noexcept {
    return D1;
}

std::bitset<32> Decode::getRD2() const noexcept {
    return D2;
}

std::bitset<5> Decode::getA1() const noexcept {
    return instr_.getRs1();
}

std::bitset<5> Decode::getA2() const noexcept {
    return instr_.getRs2();
}

PC Decode::getPC() const noexcept {
    return pc_;
}

RISCVInstr Decode::getInstr() const noexcept {
    return instr_;
}

bool Decode::V_DE() const noexcept {
    return v_de_;
}

void Decode::setInstr(const RISCVInstr &instr) {
    instr_ = instr;
}

void Decode::setPC(const PC &pc) {
    pc_ = pc;
}

void Decode::setPC_R(bool pc_f) {
    pc_f_ = pc_f;
}
