#ifndef SIMULATOR_DECODE_H
#define SIMULATOR_DECODE_H

#include "units/Basics.h"
#include "units/ContolUnit.h"

class Decode final : public Stage {
public:
    explicit Decode() = default;
    bool Run(Simulator &cpu) override;
    bool Stall() override;

    [[nodiscard]] ControlUnit::Flags getCUState() const noexcept;
    [[nodiscard]] std::bitset<32> getRD1() const noexcept;
    [[nodiscard]] std::bitset<32> getRD2() const noexcept;
    [[nodiscard]] RISCVInstr getInstr() const noexcept;
    [[nodiscard]] PC getPC() const noexcept;

    void setInstr(const RISCVInstr &instr);
    void setPC(const PC &pc);

    bool is_set{false};
private:
    /*=== units ===*/
    ControlUnit cu_;
    RegisterFile reg_file_;
    /*=============*/

    /*=== inputs ===*/
    RISCVInstr instr_;
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> D1;
    std::bitset<32> D2;
    bool V_DE_{false};
    // cu_ flags state
    // instr_
    /*===============*/

    /*=== fallthrough ===*/
    PC pc_{0};
    /*===================*/
};

#endif //SIMULATOR_DECODE_H
