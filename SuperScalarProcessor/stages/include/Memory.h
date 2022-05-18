#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "Basics.h"

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

    // For testing
    void storeToDMEM(std::bitset<32> WD, std::bitset<32> A, DMEM::Width w_type = DMEM::Width::WORD);
    std::bitset<32> loadFromDMEM(std::bitset<32> A, DMEM::Width w_type = DMEM::Width::WORD);

    bool is_set{false};
    bool mem_we_{false};
    std::bitset<32> D2;
private:
    /*=== units ===*/
    DMEM dmem_;
    /*=============*/

    /*=== inputs ===*/
    WE_GEN we_gen_;
    bool ws_{false};
    DMEM::Width lwidth_;
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
