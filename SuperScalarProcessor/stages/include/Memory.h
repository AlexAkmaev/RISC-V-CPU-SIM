#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include "Basics.h"

class Memory final : public Stage {
public:
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] std::bitset<32> ALU_OUT(Way way) const noexcept;
    [[nodiscard]] bool WB_WE(Way way) const noexcept;
    [[nodiscard]] bool EBREAK() const noexcept;
    [[nodiscard]] std::bitset<5> WB_A(Way way) const noexcept;
    [[nodiscard]] std::bitset<32> getOutData(Way way) const noexcept;

    void setWE_GEN(const WE_GEN &we_gen_up, const WE_GEN &we_gen_down);
    void setD2(std::bitset<32> d2);
    void setD5(std::bitset<32> d5);
    void setWS(bool ws_up, bool ws_down);
    void setLWidth(DMEM::Width lwidth_up, DMEM::Width lwidth_down);
    void setALU_OUT(std::bitset<32> alu_out_up, std::bitset<32> alu_out_down);
    void setWB_A(std::bitset<5> wb_a_up, std::bitset<5> wb_a_down);

    // For testing
    void storeToDMEM(std::bitset<32> WD, std::bitset<32> A, DMEM::Width w_type = DMEM::Width::WORD);
    std::bitset<32> loadFromDMEM(std::bitset<32> A, DMEM::Width w_type = DMEM::Width::WORD);

    bool is_set{false};
    bool mem_we_up_{false};
    std::bitset<32> D2;
    bool mem_we_down_{false};
    std::bitset<32> D5;
private:
    /*=== units ===*/
    DMEM dmem_;
    /*=============*/

    /*=== inputs ===*/
    WE_GEN we_gen_up_;
    bool ws_up_{false};
    DMEM::Width lwidth_up_;
    std::bitset<32> alu_out_up_;
    // same for down
    WE_GEN we_gen_down_;
    bool ws_down_{false};
    DMEM::Width lwidth_down_;
    std::bitset<32> alu_out_down_;
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> out_data_up_;
    bool wb_we_up_;
    // same for down
    std::bitset<32> out_data_down_;
    bool wb_we_down_;
    /*===============*/

    /*=== fallthrough ===*/
    std::bitset<5> wb_a_up_;
    std::bitset<5> wb_a_down_;
    bool ebreak_;
    /*===================*/
};

#endif //SIMULATOR_MEMORY_H
