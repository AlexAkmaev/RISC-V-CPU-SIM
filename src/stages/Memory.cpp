#include "Memory.h"
#include "simulator.h"

PipelineState Memory::Run(Simulator &cpu) {
    if (!is_set) {
        return PipelineState::ERR;
    }

    ++cycle;
    if (ws_) {
        wb_we_ = we_gen_.WB_WE();
        if (we_gen_.MEM_WE()) {
            dmem_.Store(D2, alu_out_, lwidth_);
        } else {
            out_data_ = dmem_.Load(alu_out_, lwidth_);
        }
    } else {
        out_data_ = alu_out_;
    }
    cpu.hu_.setHU_MEM_RD_M(wb_a_, wb_we_);

    cpu.MWBtransmitData();

    is_set = false;
    return PipelineState::OK;
}

std::bitset<32> Memory::ALU_OUT() const noexcept {
    return alu_out_;
}

bool Memory::WB_WE() const noexcept {
    return wb_we_;
}

std::bitset<5> Memory::WB_A() const noexcept {
    return wb_a_;
}

std::bitset<32> Memory::getOutData() const noexcept {
    return out_data_;
}

void Memory::setWE_GEN(const WE_GEN &we_gen) {
    we_gen_ = we_gen;
}

void Memory::setD2(std::bitset<32> d2) {
    D2 = d2;
}

void Memory::setWS(bool ws) {
    ws_ = ws;
}

void Memory::setLWidth(DMEM::Width lwidth) {
    lwidth_ = lwidth;
}

void Memory::setALU_OUT(std::bitset<32> alu_out) {
    alu_out_ = alu_out;
}

void Memory::setWB_A(std::bitset<5> wb_a) {
    wb_a_ = wb_a;
}
