#ifndef SIMULATOR_FETCH_H
#define SIMULATOR_FETCH_H

#include "Basics.hpp"

class Fetch final : public Stage {
public:
    explicit Fetch() : is_set(false), imem_(IMEM{0}) {}
    explicit Fetch(uint32_t instr_count) : is_set(false), imem_(IMEM{instr_count}) {}
    explicit Fetch(std::vector<std::bitset<32>> &&imem) : is_set(true), imem_(std::move(imem)) {}

    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] RISCVInstr getInstr() const noexcept;
    [[nodiscard]] PC getPC() const noexcept;
    [[nodiscard]] PC getNextPC() const noexcept;
    [[nodiscard]] bool PC_R() const noexcept;

    void setPC_R(bool pc_r) noexcept;
    void setJALR(bool jalr) noexcept;
    void setD1(std::bitset<32> d1) noexcept;
    void setPC_EX(PC pc_ex) noexcept;
    void setPC_DISP(PC pc_disp) noexcept;
    void setIMEM(IMEM &&imem) noexcept;
    void applyPC() noexcept;

    bool is_set{false};
private:
    /*=== units ===*/
    IMEM imem_;
    /*=============*/

    /*=== inputs ===*/
    bool pc_r_{false};
    bool jalr_{false};
    std::bitset<32> d1_;
    PC pc_ex_;
    PC pc_disp_;
    /*==============*/

    /*=== outputs ===*/
    RISCVInstr instr_;
    PC pc_next_{0};
    // pc_r
    /*===============*/

    /*=== fallthrough ===*/
    PC pc_{0};
    /*===================*/
};

#endif //SIMULATOR_FETCH_H
