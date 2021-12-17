#ifndef SIMULATOR_FETCH_H
#define SIMULATOR_FETCH_H

#include "units/Basics.h"

class Fetch final : public Stage {
public:
    explicit Fetch() : imem_(IMEM{0}), is_set(false) {}
    explicit Fetch(uint32_t instr_count) : imem_(IMEM{instr_count}), is_set(false) {}
    explicit Fetch(std::vector<std::bitset<32>> &&imem) : imem_(std::move(imem)), is_set(true) {}

    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] RISCVInstr getInstr() const noexcept;
    [[nodiscard]] PC getPC() const noexcept;
    [[nodiscard]] PC getNextPC() const noexcept;
    [[nodiscard]] bool PC_R() const noexcept;

    void setIMEM(IMEM &&imem) noexcept;
    void applyPC() noexcept;

    bool is_set{false};
private:
    /*=== units ===*/
    IMEM imem_;
    /*=============*/

    /*=== inputs ===*/
    //  JALR
    //  rs1
    /*==============*/

    /*=== outputs ===*/
    RISCVInstr instr_;
    PC pc_next_{0};
    bool pc_r_{false};
    /*===============*/

    /*=== fallthrough ===*/
    PC pc_{0};
    /*===================*/
};

#endif //SIMULATOR_FETCH_H
