#ifndef SIMULATOR_EXECUTE_H
#define SIMULATOR_EXECUTE_H

#include "Basics.h"
#include "HazardUnit.h"
#include "ContolUnit.h"

class Execute final : public Stage {
public:
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] RISCVInstr getInstr() const noexcept;
    [[nodiscard]] WE_GEN getWE_GEN() const noexcept;
    [[nodiscard]] std::bitset<32> ALU_OUT() const noexcept;
    [[nodiscard]] std::bitset<32> D1() const noexcept;
    [[nodiscard]] std::bitset<32> RS2V() const noexcept;
    [[nodiscard]] bool isRestore() const noexcept;
    [[nodiscard]] bool PC_R() const noexcept;
    [[nodiscard]] PC PC_EX() const noexcept;
    [[nodiscard]] PC PC_DISP() const noexcept;
    [[nodiscard]] bool JALR() const noexcept;
    [[nodiscard]] bool WS() const noexcept;
    [[nodiscard]] DMEM::Width MEM_WIDTH() const noexcept;
    [[nodiscard]] std::bitset<5> WB_A() const noexcept;

    void setPC_EX(const PC &pc);
    void setD1_D2(std::bitset<32> d1, std::bitset<32> d2);
    void setInstr(const RISCVInstr &instr);
    void setV_EX(bool v_ex);
    void setControl_EX(const ControlUnit::Flags &flags);

    bool is_set{false};
private:
    [[nodiscard]] std::bitset<32> ChooseRS(
        const HazardUnit::HU_RS &hu_rs, Simulator &cpu) const;  // rs - select D1 or D2
    [[nodiscard]] std::bitset<32> ChooseALU_SRC1(std::bitset<32> RS1V);
    [[nodiscard]] std::bitset<32> ChooseALU_SRC2(std::bitset<32> RS2V);

    /*=== units ===*/
    //  static ALU
    //  static CMP
    IMM imm_;
    /*=============*/

    /*=== inputs ===*/
    std::bitset<32> d1_;
    std::bitset<32> d2_;
    RISCVInstr instr_;
    PC PC_EX_;
    bool v_ex_{true};
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> alu_out_;
    PC PC_DISP_;
    bool PC_R_{false};
    bool restore_{false};
    bool BP_WRONG_{false};  //  Branch Prediction was wrong
    WE_GEN we_gen_;
    /*===============*/

    /*=== fallthrough ===*/
    ControlUnit::Flags CONTROL_EX_;
    std::bitset<5> wb_a_;
    std::bitset<32> rs2v_;
    /*===================*/
};

#endif //SIMULATOR_EXECUTE_H
