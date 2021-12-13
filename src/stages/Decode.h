#ifndef SIMULATOR_DECODE_H
#define SIMULATOR_DECODE_H

#include "units/Basics.h"

class Decode final : public Stage {
public:
    bool Run() override;

    [[nodiscard]] ControlUnit::Flags getCUState() const noexcept;
    [[nodiscard]] std::bitset<32> getRD1() const noexcept;
    [[nodiscard]] std::bitset<32> getRD2() const noexcept;
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
    // cu_ flags state
    /*===============*/

    /*=== fallthrough ===*/
    PC pc_{0};
    /*===================*/
};

#endif //SIMULATOR_DECODE_H
