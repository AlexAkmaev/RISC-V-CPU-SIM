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

    [[nodiscard]] HU_RS HU_RS1(Simulator &cpu);
    [[nodiscard]] HU_RS HU_RS2(Simulator &cpu);

    void setHU_MEM_RD_M(std::bitset<5> wb_a, bool wb_we);
    void setHU_MEM_RD_WB(std::bitset<5> wb_a, bool wb_we);
    void setA1_A2_D(std::bitset<5> a1, std::bitset<5> a2);
    void setA1_A2_EX(std::bitset<5> a1, std::bitset<5> a2);

    PipelineState pl_state;
private:
    /*=== outputs ===*/
    HU_RS hu_rs1_;
    HU_RS hu_rs2_;
    /*===============*/

    /*=== inputs ====*/
    std::bitset<5> a1_d_;  // decode stage
    std::bitset<5> a2_d_;
    std::bitset<5> a1_ex_;  // execute stage
    std::bitset<5> a2_ex_;
    bool hu_pc_redirect;
    bool wb_we_m_{true};
    bool wb_we_wb_{true};
    std::bitset<5> hu_mem_rd_m_;
    std::bitset<5> hu_mem_rd_wb_;
    /*===============*/
};

#endif //SIMULATOR_HAZARDUNIT_H
