#include "WriteBack.h"
#include "simulator.h"

PipelineState WriteBack::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::STALL;
    }

    if (ebreak_) {
        return PipelineState::BREAK;
    }

    cpu.hu_.setHU_MEM_RD_WB(wb_a_up_, wb_we_up_, Way::UP);
    cpu.hu_.setBP_WB(wb_d_up_, Way::UP);
    cpu.decode_.writeToRF(wb_a_up_, wb_d_up_, wb_we_up_);

    // Same for down way
    cpu.hu_.setHU_MEM_RD_WB(wb_a_down_, wb_we_down_, Way::DOWN);
    cpu.hu_.setBP_WB(wb_d_down_, Way::DOWN);
    cpu.decode_.writeToRF(wb_a_down_, wb_d_down_, wb_we_down_);

    ++cycle;
    is_set = false;
    return PipelineState::OK;
}

bool WriteBack::WB_WE(Way way) const noexcept {
    return way == Way::UP ? wb_we_up_ : wb_we_down_;
}

std::bitset<5> WriteBack::WB_A(Way way) const noexcept {
    return way == Way::UP ? wb_a_up_ : wb_a_down_;
}

std::bitset<32> WriteBack::WB_D(Way way) const noexcept {
    return way == Way::UP ? wb_d_up_ : wb_d_down_;
}

void WriteBack::setWB_A(std::bitset<5> wb_a_up, std::bitset<5> wb_a_down) {
    wb_a_up_ = wb_a_up;
    wb_a_down_ = wb_a_down;
}

void WriteBack::setWB_D(std::bitset<32> wb_d_up, std::bitset<32> wb_d_down) {
    wb_d_up_ = wb_d_up;
    wb_d_down_ = wb_d_down;
}

void WriteBack::setWB_WE(bool wb_we_up, bool wb_we_down) {
    wb_we_up_ = wb_we_up;
    wb_we_down_ = wb_we_down;
}

void WriteBack::setEBREAK(bool eb) {
    ebreak_ = eb;
}
