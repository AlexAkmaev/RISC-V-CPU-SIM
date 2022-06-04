#ifndef SIMULATOR_DECODE_H
#define SIMULATOR_DECODE_H

#include "Basics.hpp"
#include "ContolUnit.hpp"

class Decode final : public Stage {
public:
    explicit Decode() = default;
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] ControlUnit::Flags getCUState() const noexcept;
    [[nodiscard]] std::bitset<32> getRD1() const noexcept;
    [[nodiscard]] std::bitset<32> getRD2() const noexcept;
    [[nodiscard]] std::bitset<5> getA1() const noexcept;
    [[nodiscard]] std::bitset<5> getA2() const noexcept;
    [[nodiscard]] RISCVInstr getInstr() const noexcept;
    [[nodiscard]] PC getPC() const noexcept;
    [[nodiscard]] bool V_DE() const noexcept;

    void setInstr(const RISCVInstr &instr);
    void setPC(const PC &pc);
    void setPC_R_F(bool pc_f);
    void setPC_R(bool pc_r);
    void writeToRF(std::bitset<5> A3, std::bitset<32> D3, bool wb_we);

    // for tests
    [[nodiscard]] const RegisterFile& getRegFile() const noexcept;

    bool is_set{false};
private:
    /*=== units ===*/
    ControlUnit cu_;
    RegisterFile reg_file_;
    /*=============*/

    /*=== inputs ===*/
    bool pc_f_{false};
    bool pc_r_{false};
    RISCVInstr instr_;
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> D1;
    std::bitset<32> D2;
    bool v_de_{true};
    // cu_ flags state
    // instr_
    /*===============*/

    /*=== fallthrough ===*/
    PC pc_{0};
    /*===================*/
};

#endif //SIMULATOR_DECODE_H
