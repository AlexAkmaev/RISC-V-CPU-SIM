#include "Decode.h"
#include "simulator.h"

bool Decode::Run(Simulator &cpu) {
    if (!is_set) {
        return false;
    }

    cu_.setState(instr_);

    D1 = reg_file_.Read(instr_.getRs1());
    D2 = reg_file_.Read(instr_.getRs2());
    if (cpu.write_back_.WB_WE()) {
        reg_file_.Write(cpu.write_back_.WB_A(), cpu.write_back_.WB_D());
    }

//    V_DE_ = cpu.fetch_.
    cpu.DEtransmitData();

    is_set = false;
    return true;
}

bool Decode::Stall() {
    return true;
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

PC Decode::getPC() const noexcept {
    return pc_;
}

void Decode::setInstr(const RISCVInstr &instr) {
    instr_ = instr;
}

void Decode::setPC(const PC &pc) {
    pc_ = pc;
}

RISCVInstr Decode::getInstr() const noexcept {
    return instr_;
}
