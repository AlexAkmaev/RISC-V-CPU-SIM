#include "WriteBack.h"
#include "simulator.h"

bool WriteBack::Run(Simulator &cpu) {
    return true;
}

bool WriteBack::Stall() {
    return true;
}

std::bitset<32> WriteBack::BP_WB() const noexcept {
    return bp_wb_;
}

std::bitset<5> WriteBack::WB_A() const noexcept {
    return wb_a_;
}

std::bitset<32> WriteBack::WB_D() const noexcept {
    return wb_d_;
}

bool WriteBack::WB_WE() const noexcept {
    return wb_we_;
}

void WriteBack::setWB_A(std::bitset<5> wb_a) {
    wb_a_ = wb_a;
}

void WriteBack::setWB_D(std::bitset<32> wb_d) {
    wb_d_ = wb_d;
}

void WriteBack::setWB_WE(bool wb_we) {
    wb_we_ = wb_we;
}
