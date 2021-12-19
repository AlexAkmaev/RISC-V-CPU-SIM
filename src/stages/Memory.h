#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "units/Basics.h"

class Memory final : public Stage {
public:
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] std::bitset<32> ALU_OUT() const noexcept;
    [[nodiscard]] bool WB_WE() const noexcept;
    [[nodiscard]] bool EBREAK() const noexcept;
    [[nodiscard]] std::bitset<5> WB_A() const noexcept;
    [[nodiscard]] std::bitset<32> getOutData() const noexcept;

    void setWE_GEN(const WE_GEN &we_gen);
    void setD2(std::bitset<32> d2);
    void setWS(bool ws);
    void setLWidth(DMEM::Width lwidth);
    void setALU_OUT(std::bitset<32> alu_out);
    void setWB_A(std::bitset<5> wb_a);
    void setEBREAK(bool eb);

    bool is_set{false};
private:
    /*=== units ===*/
    DMEM dmem_;
    /*=============*/

    /*=== inputs ===*/
    WE_GEN we_gen_;
    bool ws_{false};
    DMEM::Width lwidth_;
    std::bitset<32> D2;
    std::bitset<32> alu_out_;
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> out_data_;
    bool wb_we_;
    /*===============*/

    /*=== fallthrough ===*/
    std::bitset<5> wb_a_;
    bool ebreak_;
    /*===================*/
};

#endif //SIMULATOR_MEMORY_H
