#ifndef SIMULATOR_HAZARDUNIT_H
#define SIMULATOR_HAZARDUNIT_H

#include "units/Basics.h"

class HazardUnit final {
public:
    enum class HU_RS {
        D1,
        BP_MEM,
        BP_WB
    };

    struct Flags final {
        bool HU_MEM_RD;
    };

    [[nodiscard]] HU_RS HU_RS1() const noexcept;
    [[nodiscard]] HU_RS HU_RS2() const noexcept;

private:
    HU_RS hu_rs1_;
    HU_RS hu_rs2_;
    PC HU_PC_REDIRECT;
};

#endif //SIMULATOR_HAZARDUNIT_H
