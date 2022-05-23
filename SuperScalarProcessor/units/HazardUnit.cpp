#include "HazardUnit.h"
#include "simulator.h"

HazardUnit::HU_RS HazardUnit::HU_RS1() noexcept {
    // By pass from memory stage
    if (wb_we_m_down_ && a1_ex_ == hu_mem_rd_m_down_) {
        return HU_RS::BP_MEM_Down;
    }

    if (wb_we_m_up_ && a1_ex_ == hu_mem_rd_m_up_) {
        return HU_RS::BP_MEM_Up;
    }

    // By pass from write back stage
    if ((wb_we_wb_down_ && (a1_ex_ == hu_mem_rd_wb_down_)) || bp_rd_rs1_down_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Down;
    }

    if ((wb_we_wb_up_ && (a1_ex_ == hu_mem_rd_wb_up_)) || bp_rd_rs1_up_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Up;
    }

    return HU_RS::D1;
}

HazardUnit::HU_RS HazardUnit::HU_RS2() noexcept {
    // By pass from memory stage
    if (wb_we_m_down_ && a2_ex_ == hu_mem_rd_m_down_) {
        return HU_RS::BP_MEM_Down;
    }

    if (wb_we_m_up_ && a2_ex_ == hu_mem_rd_m_up_) {
        return HU_RS::BP_MEM_Up;
    }

    // By pass from write back stage
    if ((wb_we_wb_down_ && (a2_ex_ == hu_mem_rd_wb_down_)) ||bp_rd_rs2_down_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Down;
    }

    if ((wb_we_wb_up_ && (a2_ex_ == hu_mem_rd_wb_up_)) || bp_rd_rs2_up_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Up;
    }

    return HU_RS::D2;
}

HazardUnit::HU_RS HazardUnit::HU_RS4() noexcept {
    // By pass from memory stage
    if (wb_we_m_down_ && a4_ex_ == hu_mem_rd_m_down_) {
        return HU_RS::BP_MEM_Down;
    }

    if (wb_we_m_up_ && a4_ex_ == hu_mem_rd_m_up_) {
        return HU_RS::BP_MEM_Up;
    }

    // By pass from write back stage
    if ((wb_we_wb_down_ && (a4_ex_ == hu_mem_rd_wb_down_)) || bp_rd_rs4_down_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Down;
    }

    if ((wb_we_wb_up_ && (a4_ex_ == hu_mem_rd_wb_up_)) || bp_rd_rs4_up_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Up;
    }

    return HU_RS::D4;
}

HazardUnit::HU_RS HazardUnit::HU_RS5() noexcept {
    // By pass from memory stage
    if (wb_we_m_up_ && a5_ex_ == hu_mem_rd_m_up_) {
        return HU_RS::BP_MEM_Up;
    }

    if (wb_we_m_down_ && a5_ex_ == hu_mem_rd_m_down_) {
        return HU_RS::BP_MEM_Down;
    }

    // By pass from write back stage
    if ((wb_we_wb_down_ && (a5_ex_ == hu_mem_rd_wb_down_)) || bp_rd_rs5_down_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Down;
    }

    if ((wb_we_wb_up_ && (a5_ex_ == hu_mem_rd_wb_up_)) ||bp_rd_rs5_up_) {
        pl_state = PipelineState::OK;
        return HU_RS::BP_WB_Up;
    }

    return HU_RS::D5;
}

std::bitset<32> HazardUnit::BP_MEM(Way way) const noexcept {
    return way == Way::UP ? bp_mem_up_ : bp_mem_down_;
}

std::bitset<32> HazardUnit::BP_WB(Way way) const noexcept {
    return way == Way::UP ? bp_wb_up_ : bp_wb_down_;
}

std::bitset<32> HazardUnit::BP_RD(Way way) const noexcept {
    return way == Way::UP ? bp_rd_up_ : bp_wb_down_;
}

void HazardUnit::CheckWaysDataDepends(std::bitset<5> rd_up, bool wb_we,
                                      std::bitset<5> A4, std::bitset<5> A5, bool is_ebreak) noexcept {
    if (wb_we && (rd_up == A4 || rd_up == A5) && pl_state != PipelineState::STALL && !is_ebreak) {
        pl_state = PipelineState::STALL_DOWN;
        return;
    }

    if (pl_state == PipelineState::STALL_DOWN) {
        pl_state = PipelineState::OK;
    }
}

bool HazardUnit::CheckForStall(Simulator &cpu) noexcept {
    bool ws_ex_up = cpu.execute_.WS(Way::UP), ws_ex_down{false};
    std::bitset<5> rd_ex_up = cpu.execute_.WB_A(Way::UP), rd_ex_down;
    std::bitset<5> A1_D = cpu.decode_.getInstr(Way::UP).getRs1();
    std::bitset<5> A2_D = cpu.decode_.getInstr(Way::UP).getRs2();
    std::bitset<5> A4_D, A5_D;
    if (cpu.hu_.pl_state != PipelineState::STALL_DOWN) {
        ws_ex_down = cpu.execute_.WS(Way::DOWN);
        rd_ex_down = cpu.execute_.WB_A(Way::DOWN);
        A4_D = cpu.decode_.getInstr(Way::DOWN).getRs1();
        A5_D = cpu.decode_.getInstr(Way::DOWN).getRs2();
    }

    bool is_conflict = (ws_ex_up && (bp_rd_rs1_up_ = rd_ex_up == A1_D || (bp_rd_rs2_up_ = rd_ex_up == A2_D))) ||
                  (ws_ex_up && (bp_rd_rs4_up_ = rd_ex_up == A4_D || (bp_rd_rs5_up_ = rd_ex_up == A5_D))) ||
                  (ws_ex_down && (bp_rd_rs1_down_ = rd_ex_down == A1_D || (bp_rd_rs2_down_ = rd_ex_down == A2_D))) ||
                  (ws_ex_down && (bp_rd_rs4_down_ = rd_ex_down == A4_D || (bp_rd_rs5_down_ = rd_ex_down == A5_D)));

    if (is_conflict) {
        pc_en_ = false;
        fd_en_ = false;
        pl_state = PipelineState::STALL;
        return true;
    }

    bp_rd_rs1_up_ = bp_rd_rs2_up_ = bp_rd_rs4_up_ = bp_rd_rs5_up_ = false;
    bp_rd_rs1_down_ = bp_rd_rs2_down_ = bp_rd_rs4_down_ = bp_rd_rs5_down_ = false;
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

void HazardUnit::setHU_MEM_RD_M(std::bitset<5> wb_a, bool wb_we, Way way) {
    if (way == Way::UP) {
        hu_mem_rd_m_up_ = wb_a;
        wb_we_m_up_ = wb_we;
    } else {
        hu_mem_rd_m_down_ = wb_a;
        wb_we_m_down_ = wb_we;
    }
}

void HazardUnit::setHU_MEM_RD_WB(std::bitset<5> wb_a, bool wb_we, Way way) {
    if (way == Way::UP) {
        hu_mem_rd_wb_up_ = wb_a;
        wb_we_wb_up_ = wb_we;
    } else {
        hu_mem_rd_wb_down_ = wb_a;
        wb_we_wb_down_ = wb_we;
    }
}

void HazardUnit::setBP_MEM(std::bitset<32> wb_d, Way way) {
    if (way == Way::UP) {
        bp_mem_up_ = wb_d;
    } else {
        bp_mem_down_ = wb_d;
    }
}

void HazardUnit::setBP_WB(std::bitset<32> wb_d, Way way) {
    if (way == Way::UP) {
        bp_wb_up_ = wb_d;
    } else {
        bp_wb_down_ = wb_d;
    }
}

void HazardUnit::setA1_A2_EX(std::bitset<5> a1, std::bitset<5> a2) {
    a1_ex_ = a1;
    a2_ex_ = a2;
}

void HazardUnit::setA4_A5_EX(std::bitset<5> a4, std::bitset<5> a5) {
    a4_ex_ = a4;
    a5_ex_ = a5;
}

void HazardUnit::sendEndOfIMEM() {
    pc_en_ = hu_pc_redirect_;
}

void HazardUnit::setHU_PC_REDIECT(bool pc_r) {
    hu_pc_redirect_ = pc_r;
}

void HazardUnit::setBranchPrediction(const PC &cur_pc, const PC &pc_disp, bool comp) {
    branchPredictor_.setPrediction(cur_pc, pc_disp, comp);
}

bool HazardUnit::getPredicton(const PC &pc) const noexcept {
    return branchPredictor_.getPrediction(pc);
}

PC HazardUnit::getTarget(bool pred, const PC &pc) const noexcept {
    return branchPredictor_.getTarget(pred, pc);
}
