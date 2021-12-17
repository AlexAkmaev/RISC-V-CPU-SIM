#include "WriteBack.h"
#include "simulator.h"

PipelineState WriteBack::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::ERR;
    }

    ++cycle;
    cpu.hu_.setHU_MEM_RD_WB(wb_a_, wb_we_);

    is_set = false;
    return PipelineState::OK;
}

bool WriteBack::WB_WE() const noexcept {
    return wb_we_;
}

std::bitset<5> WriteBack::WB_A() const noexcept {
    return wb_a_;
}

std::bitset<32> WriteBack::WB_D() const noexcept {
    return wb_d_;
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
