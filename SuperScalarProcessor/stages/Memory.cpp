#include "Memory.h"
#include "simulator.h"

PipelineState Memory::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::STALL;
    }

    ebreak_ = we_gen_up_.EBREAK();
    wb_we_up_ = we_gen_up_.WB_WE();
    mem_we_up_ = we_gen_up_.MEM_WE();
    if (mem_we_up_) {
        dmem_.Store(D2, alu_out_up_, lwidth_up_);
    }

    if (ws_up_) {
        out_data_up_ = dmem_.Load(alu_out_up_, lwidth_up_);
    } else {
        out_data_up_ = alu_out_up_;
    }
    cpu.hu_.setHU_MEM_RD_M(wb_a_up_, wb_we_up_, Way::UP);
    cpu.hu_.setBP_MEM(alu_out_up_, Way::UP);

    // Same for down way
    wb_we_down_ = we_gen_down_.WB_WE();
    mem_we_down_ = we_gen_down_.MEM_WE();
    if (mem_we_down_) {
        dmem_.Store(D5, alu_out_down_, lwidth_down_);
    }

    if (ws_down_) {
        out_data_down_ = dmem_.Load(alu_out_down_, lwidth_down_);
    } else {
        out_data_down_ = alu_out_down_;
    }
    cpu.hu_.setHU_MEM_RD_M(wb_a_down_, wb_we_down_, Way::DOWN);
    cpu.hu_.setBP_MEM(alu_out_down_, Way::DOWN);

    cpu.MWBtransmitData();

    is_set = false;
    return PipelineState::OK;
}

std::bitset<32> Memory::ALU_OUT(Way way) const noexcept {
    return way == Way::UP ? alu_out_up_ : alu_out_down_;
}

bool Memory::WB_WE(Way way) const noexcept {
    return way == Way::UP ? wb_we_up_ : wb_we_down_;
}

bool Memory::EBREAK() const noexcept {
    return ebreak_;
}

std::bitset<5> Memory::WB_A(Way way) const noexcept {
    return way == Way::UP ? wb_a_up_ : wb_a_down_;
}

std::bitset<32> Memory::getOutData(Way way) const noexcept {
    return way == Way::UP ? out_data_up_ : out_data_down_;
}

void Memory::setWE_GEN(const WE_GEN &we_gen_up, const WE_GEN &we_gen_down) {
    we_gen_up_ = we_gen_up;
    we_gen_down_ = we_gen_down;
}

void Memory::setD2(std::bitset<32> d2) {
    D2 = d2;
}

void Memory::setD5(std::bitset<32> d5) {
    D5 = d5;
}

void Memory::setWS(bool ws_up, bool ws_down) {
    ws_up_ = ws_up;
    ws_down_ = ws_down;
}

void Memory::setLWidth(DMEM::Width lwidth_up, DMEM::Width lwidth_down) {
    lwidth_up_ = lwidth_up;
    lwidth_down_ = lwidth_down;
}

void Memory::setALU_OUT(std::bitset<32> alu_out_up, std::bitset<32> alu_out_down) {
    alu_out_up_ = alu_out_up;
    alu_out_down_ = alu_out_down;
}

void Memory::setWB_A(std::bitset<5> wb_a_up, std::bitset<5> wb_a_down) {
    wb_a_up_ = wb_a_up;
    wb_a_down_ = wb_a_down;
}

void Memory::storeToDMEM(std::bitset<32> WD, std::bitset<32> A, DMEM::Width w_type) {
    dmem_.Store(WD, A, w_type);
}

std::bitset<32> Memory::loadFromDMEM(std::bitset<32> A, DMEM::Width w_type) {
    return dmem_.Load(A, w_type);
}
