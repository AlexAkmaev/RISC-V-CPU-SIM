#ifndef SIMULATOR_HAZARDUNIT_H
#define SIMULATOR_HAZARDUNIT_H

#include "units/Basics.h"

class HazardUnit final {
public:
    enum class HU_RS {
        D1,
        D2,
        BP_MEM,
        BP_WB
    };

    [[nodiscard]] HU_RS HU_RS1(Simulator &cpu) noexcept;
    [[nodiscard]] HU_RS HU_RS2(Simulator &cpu) noexcept;
    [[nodiscard]] std::bitset<32> BP_MEM() const noexcept;
    [[nodiscard]] std::bitset<32> BP_WB() const noexcept;
    [[nodiscard]] std::bitset<32> BP_RD() const noexcept;
    bool CheckForStall(bool ws_ex, std::bitset<5> rd, Simulator &cpu) noexcept;
    [[nodiscard]] bool FD_EN() const noexcept;
    [[nodiscard]] bool PC_EN() const noexcept;

    void setBP_MEM(std::bitset<32> wb_d);
    void setBP_WB(std::bitset<32> wb_d);
    void setHU_MEM_RD_M(std::bitset<5> wb_a, bool wb_we);
    void setHU_MEM_RD_WB(std::bitset<5> wb_a, bool wb_we);
    void setA1_A2_EX(std::bitset<5> a1, std::bitset<5> a2);
    void setHU_PC_REDIECT(bool pc_r);
    void sendEndOfIMEM();

    PipelineState pl_state{PipelineState::OK};
    PipelineState exception_state{PipelineState::OK};
private:
    /*=== outputs ===*/
    //  HU_RS1;
    //  HU_RS2;
    //  isStall
    bool pc_en_{true};
    bool fd_en_{true};
    /*===============*/

    /*=== inputs ====*/
    std::bitset<5> a1_ex_;  // execute stage
    std::bitset<5> a2_ex_;
    bool hu_pc_redirect_{false};
    std::bitset<32> bp_mem_;
    std::bitset<32> bp_wb_;
    bool bp_rd_rs1_{false};
    bool bp_rd_rs2_{false};
    std::bitset<32> bp_rd_;
    bool wb_we_m_{false};
    bool wb_we_wb_{false};
    std::bitset<5> hu_mem_rd_m_;
    std::bitset<5> hu_mem_rd_wb_;
    /*===============*/
};

#endif //SIMULATOR_HAZARDUNIT_H
