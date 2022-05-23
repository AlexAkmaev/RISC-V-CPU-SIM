#ifndef SIMULATOR_DECODE_H
#define SIMULATOR_DECODE_H

#include "Basics.h"
#include "ContolUnit.h"

class Decode final : public Stage {
public:
    explicit Decode() = default;
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] ControlUnit::Flags getCUState(Way way) const noexcept;
    [[nodiscard]] std::bitset<32> getRD1() const noexcept;
    [[nodiscard]] std::bitset<32> getRD2() const noexcept;
    [[nodiscard]] std::bitset<32> getRD4() const noexcept;
    [[nodiscard]] std::bitset<32> getRD5() const noexcept;
    [[nodiscard]] std::bitset<5> getA1() const noexcept;
    [[nodiscard]] std::bitset<5> getA2() const noexcept;
    [[nodiscard]] std::bitset<5> getA4() const noexcept;
    [[nodiscard]] std::bitset<5> getA5() const noexcept;
    [[nodiscard]] RISCVInstr getInstr(Way way) const noexcept;
    [[nodiscard]] PC getPC_Up() const noexcept;
    [[nodiscard]] bool V_DE(Way way) const noexcept;  //  Is valid state for instruction

    void setInstr(const RISCVInstr &instr, Way way);
    void setPC_Up(const PC &pc);
    void setPC_R_F(bool pc_f);
    void setPC_R(bool pc_r);
    void writeToRF(std::bitset<5> A, std::bitset<32> D, bool wb_we);  //  A is A3 or A6 and D is D3 or D6

    // for tests
    [[nodiscard]] const RegisterFile& getRegFile() const noexcept;

    bool is_set{false};
private:
    void ShiftData();

    /*=== units ===*/
    ControlUnit cu_up_;
    ControlUnit cu_down_;
    RegisterFile reg_file_;
    /*=============*/

    /*=== inputs ===*/
    bool pc_f_{false};
    bool pc_r_{false};
    RISCVInstr instrUp_;
    RISCVInstr instrDown_;
    /*==============*/

    /*=== outputs ===*/
    // For up way
    std::bitset<32> D1;
    std::bitset<32> D2;
    // For down way
    std::bitset<32> D4;
    std::bitset<32> D5;
    bool v_de_up_{true};
    bool v_de_down_{true};
    // cu_up_ and cu_down_ flags state
    // instrUp_ and instrDown_
    /*===============*/

    /*=== fallthrough ===*/
    PC pc_up_{0};
    /*===================*/
};

#endif //SIMULATOR_DECODE_H
