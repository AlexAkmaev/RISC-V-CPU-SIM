#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "units/Basics.h"

class Memory final : public Stage {
public:
    bool Run(Simulator &cpu) override;
    bool Stall() override;

    [[nodiscard]] std::bitset<32> ALU_OUT() const noexcept;
    [[nodiscard]] bool WB_WE() const noexcept;
    [[nodiscard]] std::bitset<5> WB_A() const noexcept;
    [[nodiscard]] std::bitset<32> getOutData() const noexcept;

    void setWE_GEN(const WE_GEN &we_gen);
    void setD2(std::bitset<32> d2);
    void setWS(bool ws);
    void setALU_OUT(std::bitset<32> alu_out);
    void setWB_A(std::bitset<5> wb_a);

    bool is_set{false};
private:
    /*=== units ===*/
    DMEM dmem_;
    /*=============*/

    /*=== inputs ===*/
    WE_GEN we_gen_;
    bool ws_{false};
    std::bitset<32> D2;
    std::bitset<32> alu_out_;
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> out_data_;
    bool wb_we_;
    /*===============*/

    /*=== fallthrough ===*/
    std::bitset<5> wb_a_;
    /*===================*/
};

#endif //SIMULATOR_MEMORY_H
