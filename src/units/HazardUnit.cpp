#include "HazardUnit.h"
#include "simulator.h"

HazardUnit::HU_RS HazardUnit::HU_RS1(Simulator &cpu) noexcept {
    if (wb_we_m_ && a1_ex_ == hu_mem_rd_m_) {
        return HU_RS::BP_MEM;
    }

    if (wb_we_wb_ && (a1_ex_ == hu_mem_rd_wb_ || pl_state == PipelineState::STALL)) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB;
    }

    return HU_RS::D1;
}

HazardUnit::HU_RS HazardUnit::HU_RS2(Simulator &cpu) noexcept {
    if (wb_we_m_ && a2_ex_ == hu_mem_rd_m_) {
        return HU_RS::BP_MEM;
    }

    if (wb_we_wb_ && (a2_ex_ == hu_mem_rd_wb_ || pl_state == PipelineState::STALL)) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB;
    }

    return HU_RS::D2;
}

bool HazardUnit::CheckForStall(bool ws_ex, std::bitset<5> rd) noexcept {
    if (ws_ex && (rd == a1_d_ || rd == a2_d_)) {
        pc_en_ = false;
        fd_en_ = false;
        pl_state = PipelineState::STALL;
        return true;
    }

    pc_en_ = true;
    fd_en_ = true;
    pl_state = PipelineState::OK;

    return false;
}

bool HazardUnit::PC_EN() const noexcept {
    return pc_en_;
}

bool HazardUnit::FD_EN() const noexcept {
    return fd_en_;
}

void HazardUnit::setHU_MEM_RD_M(std::bitset<5> wb_a, bool wb_we) {
    hu_mem_rd_m_ = wb_a;
    wb_we_m_ = wb_we;
}

void HazardUnit::setHU_MEM_RD_WB(std::bitset<5> wb_a, bool wb_we) {
    hu_mem_rd_wb_ = wb_a;
    wb_we_wb_ = wb_we;
    if (wb_we && wb_a == std::bitset<5>{/* x0 */ 0}) {
        exception_state = PipelineState::BREAK;
    }
}

void HazardUnit::setA1_A2_D(std::bitset<5> a1, std::bitset<5> a2) {
    a1_d_ = a1;
    a2_d_ = a2;
}

void HazardUnit::setA1_A2_EX(std::bitset<5> a1, std::bitset<5> a2) {
    a1_ex_ = a1;
    a2_ex_ = a2;
}

void HazardUnit::sendEndOfIMEM() {
    pc_en_ = false;
}
