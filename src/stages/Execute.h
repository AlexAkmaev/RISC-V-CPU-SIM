#ifndef SIMULATOR_EXECUTE_H
#define SIMULATOR_EXECUTE_H

#include "units/Basics.h"
#include "units/HazardUnit.h"
#include "units/ContolUnit.h"

class Execute final : public Stage {
public:
    bool Run(Simulator &cpu) override;
    bool Stall() override;

    [[nodiscard]] bool PC_R() const noexcept;
    [[nodiscard]] PC PC_EX() const noexcept;
    [[nodiscard]] PC PC_DISP() const noexcept;
    [[nodiscard]] bool V_EX() const noexcept;

    void setD1_D2(std::bitset<32> d1, std::bitset<32> d2);
    void setInstr(const RISCVInstr &instr);
    void setPC_EX(const PC &pc);
    void setControl_EX(const ControlUnit::Flags &flags);

    bool is_set{false};
private:
    [[nodiscard]] std::bitset<32> ChooseRS(
        bool rs, const HazardUnit::HU_RS &hu_rs, Simulator &cpu) const;  // rs - select D1 or D2

    /*=== units ===*/
    //  static ALU
    //  static CMP
    IMM imm_;
    /*=============*/

    /*=== inputs ===*/
    std::bitset<32> D1;
    std::bitset<32> D2;
    RISCVInstr instr_;
    PC PC_EX_;
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> alu_out_;
    PC PC_DISP_;
    bool PC_R_{false};
    bool V_EX_{false};
    /*===============*/

    /*=== fallthrough ===*/
    ControlUnit::Flags CONTROL_EX_;
    // D2
    /*===================*/
};

#endif //SIMULATOR_EXECUTE_H
