#include "HazardUnit.h"
#include "simulator.h"

HazardUnit::HU_RS HazardUnit::HU_RS1(Simulator &cpu) {
    if (wb_we_m_ && a1_ex_ == hu_mem_rd_m_) {
        return HU_RS::BP_MEM;
    }

    if (wb_we_wb_ && a1_ex_ == hu_mem_rd_wb_) {
        return HU_RS::BP_WB;
    }

    return HU_RS::D1;
}

HazardUnit::HU_RS HazardUnit::HU_RS2(Simulator &cpu) {
    if (wb_we_m_ && a2_ex_ == hu_mem_rd_m_) {
        return HU_RS::BP_MEM;
    }

    if (wb_we_wb_ && a2_ex_ == hu_mem_rd_wb_) {
        return HU_RS::BP_WB;
    }

    return HU_RS::D2;
}

void HazardUnit::setHU_MEM_RD_M(std::bitset<5> wb_a, bool wb_we) {
    hu_mem_rd_m_ = wb_a;
    wb_we_m_ = wb_we;
}

void HazardUnit::setHU_MEM_RD_WB(std::bitset<5> wb_a, bool wb_we) {
    hu_mem_rd_wb_ = wb_a;
    wb_we_wb_ = wb_we;
}

void HazardUnit::setA1_A2_D(std::bitset<5> a1, std::bitset<5> a2) {
    a1_d_ = a1;
    a2_d_ = a2;
}

void HazardUnit::setA1_A2_EX(std::bitset<5> a1, std::bitset<5> a2) {
    a1_ex_ = a1;
    a2_ex_ = a2;
}
