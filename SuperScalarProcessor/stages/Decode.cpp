#include "Decode.h"
#include "simulator.h"

PipelineState Decode::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::STALL;
    }

    // For upper instruction
    cu_up_.setState(instrUp_);
    D1 = reg_file_.Read(instrUp_.getRs1());
    D2 = reg_file_.Read(instrUp_.getRs2());

    v_de_up_ = !(pc_f_ || pc_r_ || cpu.hu_.pl_state == PipelineState::STALL);

    cu_down_.setState(instrDown_);
    cpu.hu_.CheckWaysDataDepends(instrUp_.getRd(), cu_up_.flags.WB_WE && v_de_up_,
                                 instrDown_.getRs1(), instrDown_.getRs2(), cu_down_.flags.EBREAK);
    bool is_stall_down = cpu.hu_.pl_state == PipelineState::STALL_DOWN;

    // For down instruction
    D4 = reg_file_.Read(instrDown_.getRs1());
    D5 = reg_file_.Read(instrDown_.getRs2());

    v_de_down_ = !(pc_f_ || pc_r_ || cpu.hu_.pl_state == PipelineState::STALL || is_stall_down || cu_down_.flags.EBREAK);

    cpu.DEtransmitData();

    if (is_stall_down) {
        ShiftData();
    }

    is_set = false;
    return PipelineState::OK;
}

void Decode::ShiftData() {
    cu_up_ = cu_down_;
    instrUp_ = instrDown_;
    D1 = D4;
    D2 = D5;
    v_de_up_ = true;
    pc_up_ += 4;
}

ControlUnit::Flags Decode::getCUState(Way way) const noexcept {
    return way == Way::UP ? cu_up_.flags : cu_down_.flags;
}

std::bitset<32> Decode::getRD1() const noexcept {
    return D1;
}

std::bitset<32> Decode::getRD2() const noexcept {
    return D2;
}

std::bitset<32> Decode::getRD4() const noexcept {
    return D4;
}

std::bitset<32> Decode::getRD5() const noexcept {
    return D5;
}

std::bitset<5> Decode::getA1() const noexcept {
    return instrUp_.getRs1();
}

std::bitset<5> Decode::getA2() const noexcept {
    return instrUp_.getRs2();
}

std::bitset<5> Decode::getA4() const noexcept {
    return instrDown_.getRs1();
}

std::bitset<5> Decode::getA5() const noexcept {
    return instrDown_.getRs2();
}

PC Decode::getPC_Up() const noexcept {
    return pc_up_;
}

RISCVInstr Decode::getInstr(Way way) const noexcept {
    return way == Way::UP ? instrUp_ : instrDown_;
}

bool Decode::V_DE(Way way) const noexcept {
    return way == Way::UP ? v_de_up_ : v_de_down_;
}

void Decode::setInstr(const RISCVInstr &instr, Way way) {
    if (way == Way::UP) {
        instrUp_ = instr;
    } else {
        instrDown_ = instr;
    }
}

void Decode::setPC_Up(const PC &pc) {
    pc_up_ = pc;
}

void Decode::setPC_R_F(bool pc_f) {
    pc_f_ = pc_f;
}

void Decode::setPC_R(bool pc_r) {
    pc_r_ = pc_r;
}

const RegisterFile &Decode::getRegFile() const noexcept {
    return reg_file_;
}

void Decode::writeToRF(std::bitset<5> A, std::bitset<32> D, bool wb_we) {
    if (wb_we) {
        reg_file_.Write(A, D);
    }
}
