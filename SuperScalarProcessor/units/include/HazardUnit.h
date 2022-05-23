#ifndef SIMULATOR_HAZARDUNIT_H
#define SIMULATOR_HAZARDUNIT_H

#include "Basics.h"
#include "BranchPredictor.h"

class HazardUnit final {
public:
    enum class HU_RS {
        D1,
        D2,
        D4,
        D5,
        BP_MEM_Up,
        BP_MEM_Down,
        BP_WB_Up,
        BP_WB_Down
    };

    bool CheckForStall(Simulator &cpu) noexcept;
    void CheckWaysDataDepends(std::bitset<5> rd_up, bool wb_we,
                              std::bitset<5> A4, std::bitset<5> A5, bool is_ebreak) noexcept;  // For decode stage

    [[nodiscard]] HU_RS HU_RS1() noexcept;
    [[nodiscard]] HU_RS HU_RS2() noexcept;
    [[nodiscard]] HU_RS HU_RS4() noexcept;
    [[nodiscard]] HU_RS HU_RS5() noexcept;
    [[nodiscard]] std::bitset<32> BP_MEM(Way way) const noexcept;
    [[nodiscard]] std::bitset<32> BP_WB(Way way) const noexcept;
    [[nodiscard]] std::bitset<32> BP_RD(Way way) const noexcept;
    [[nodiscard]] bool FD_EN() const noexcept;
    [[nodiscard]] bool PC_EN() const noexcept;
    [[nodiscard]] bool getPredicton(const PC &pc) const noexcept;
    [[nodiscard]] PC getTarget(bool pred, const PC &pc) const noexcept;

    void setBP_MEM(std::bitset<32> wb_d, Way way);
    void setBP_WB(std::bitset<32> wb_d, Way way);
    void setHU_MEM_RD_M(std::bitset<5> wb_a, bool wb_we, Way way);
    void setHU_MEM_RD_WB(std::bitset<5> wb_a, bool wb_we, Way way);
    void setA1_A2_EX(std::bitset<5> a1, std::bitset<5> a2);
    void setA4_A5_EX(std::bitset<5> a4, std::bitset<5> a5);
    void setHU_PC_REDIECT(bool pc_r);
    void setBranchPrediction(const PC &cur_pc, const PC &pc_disp, bool comp);
    void sendEndOfIMEM();

    PipelineState pl_state{PipelineState::OK};
    PipelineState exception_state{PipelineState::OK};
private:
    /*=== outputs ===*/
    //  HU_RS1;
    //  HU_RS2;
    //  HU_RS4;
    //  HU_RS5;
    //  isStall
    bool pc_en_{true};
    bool fd_en_{true};
    // Prediction
    // Target
    /*===============*/

    /*=== inputs ====*/
    std::bitset<5> a1_ex_;  // execute stage
    std::bitset<5> a2_ex_;
    std::bitset<5> a4_ex_;
    std::bitset<5> a5_ex_;
    bool hu_pc_redirect_{false};
    std::bitset<32> bp_mem_up_;
    std::bitset<32> bp_mem_down_;
    std::bitset<32> bp_wb_up_;
    std::bitset<32> bp_wb_down_;
    // By pass from wb can be from up or down previous stage
    bool bp_rd_rs1_up_{false};
    bool bp_rd_rs2_up_{false};
    bool bp_rd_rs4_up_{false};
    bool bp_rd_rs5_up_{false};
    bool bp_rd_rs1_down_{false};
    bool bp_rd_rs2_down_{false};
    bool bp_rd_rs4_down_{false};
    bool bp_rd_rs5_down_{false};
    std::bitset<32> bp_rd_up_;
    std::bitset<32> bp_rd_down_;
    bool wb_we_m_up_{false};
    bool wb_we_wb_up_{false};
    bool wb_we_m_down_{false};
    bool wb_we_wb_down_{false};
    std::bitset<5> hu_mem_rd_m_up_;
    std::bitset<5> hu_mem_rd_wb_up_;
    std::bitset<5> hu_mem_rd_m_down_;
    std::bitset<5> hu_mem_rd_wb_down_;
    // PC_EX
    // PC_DISP
    // COMP
    /*===============*/

    /*==== Units ====*/
    BranchPredictor branchPredictor_;
    /*===============*/
};

#endif //SIMULATOR_HAZARDUNIT_H
