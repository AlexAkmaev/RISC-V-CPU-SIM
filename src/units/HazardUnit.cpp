#include "HazardUnit.h"
#include "simulator.h"

HazardUnit::HU_RS HazardUnit::HU_RS1(Simulator &cpu) noexcept {
    if (wb_we_m_ && a1_ex_ == hu_mem_rd_m_) {
        return HU_RS::BP_MEM;
    }

    if (wb_we_wb_ && (a1_ex_ == hu_mem_rd_wb_) || bp_rd_rs1_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB;
    }

    return HU_RS::D1;
}

HazardUnit::HU_RS HazardUnit::HU_RS2(Simulator &cpu) noexcept {
    if (wb_we_m_ && a2_ex_ == hu_mem_rd_m_) {
        return HU_RS::BP_MEM;
    }

    if (wb_we_wb_ && (a2_ex_ == hu_mem_rd_wb_) || bp_rd_rs2_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB;
    }

    return HU_RS::D2;
}

std::bitset<32> HazardUnit::BP_MEM() const noexcept {
    return bp_mem_;
}

std::bitset<32> HazardUnit::BP_WB() const noexcept {
    return bp_wb_;
}

std::bitset<32> HazardUnit::BP_RD() const noexcept {
    return bp_rd_;
}

bool HazardUnit::CheckForStall(bool ws_ex, std::bitset<5> rd, Simulator &cpu) noexcept {
    if (ws_ex && (bp_rd_rs1_ = rd == cpu.decode_.getInstr().getRs1() || (bp_rd_rs2_ = rd == cpu.decode_.getInstr().getRs2()))) {
        pc_en_ = false;
        fd_en_ = false;
        pl_state = PipelineState::STALL;
        return true;
    }

    bp_rd_rs1_ = false;
    bp_rd_rs2_ = false;
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
}

void HazardUnit::setBP_MEM(std::bitset<32> wb_d) {
    bp_mem_ = wb_d;
}

void HazardUnit::setBP_WB(std::bitset<32> wb_d) {
    bp_wb_ = wb_d;
}

void HazardUnit::setA1_A2_EX(std::bitset<5> a1, std::bitset<5> a2) {
    a1_ex_ = a1;
    a2_ex_ = a2;
}

void HazardUnit::sendEndOfIMEM() {
    pc_en_ = hu_pc_redirect_;
}

void HazardUnit::setHU_PC_REDIECT(bool pc_r) {
    hu_pc_redirect_ = pc_r;
}
