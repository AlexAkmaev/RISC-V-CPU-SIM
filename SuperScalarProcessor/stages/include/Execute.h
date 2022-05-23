#ifndef SIMULATOR_EXECUTE_H
#define SIMULATOR_EXECUTE_H

#include "Basics.h"
#include "HazardUnit.h"
#include "ContolUnit.h"

class Execute final : public Stage {
public:
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] RISCVInstr getInstr(Way way) const noexcept;
    [[nodiscard]] WE_GEN getWE_GEN(Way way) const noexcept;
    [[nodiscard]] std::bitset<32> ALU_OUT(Way way) const noexcept;
    [[nodiscard]] std::bitset<32> D1() const noexcept;
    [[nodiscard]] std::bitset<32> D4() const noexcept;
    [[nodiscard]] std::bitset<32> RS2V() const noexcept;
    [[nodiscard]] std::bitset<32> RS5V() const noexcept;
    [[nodiscard]] bool isRestore(Way way) const noexcept;
    [[nodiscard]] bool PC_R() const noexcept;
    [[nodiscard]] PC PC_EX_Up() const noexcept;
    [[nodiscard]] PC PC_DISP(Way way) const noexcept;
    [[nodiscard]] bool JALR(Way way) const noexcept;
    [[nodiscard]] bool WS(Way way) const noexcept;
    [[nodiscard]] DMEM::Width MEM_WIDTH(Way way) const noexcept;
    [[nodiscard]] std::bitset<5> WB_A(Way way) const noexcept;

    void setPC_EX(const PC &pc);
    void setD1_D2(std::bitset<32> d1, std::bitset<32> d2);
    void setD4_D5(std::bitset<32> d4, std::bitset<32> d5);
    void setInstr(const RISCVInstr &instr, Way way);
    void setV_EX(bool v_ex_up, bool v_ex_down);
    void setControl_EX(const ControlUnit::Flags &flags, Way way);

    bool is_set{false};
private:
    // Choose resource with hazard unit
    [[nodiscard]] std::bitset<32> ChooseRS(const HazardUnit::HU_RS &hu_rs, Simulator &cpu) const;
    [[nodiscard]] std::bitset<32> ChooseALU_SRC1(std::bitset<32> RS1V);
    [[nodiscard]] std::bitset<32> ChooseALU_SRC2(std::bitset<32> RS2V);
    [[nodiscard]] std::bitset<32> ChooseALU_SRC4(std::bitset<32> RS4V);
    [[nodiscard]] std::bitset<32> ChooseALU_SRC5(std::bitset<32> RS5V);

    void ProcessPrediction(Simulator &cpu, Way way, bool compUp, bool compDown) noexcept;

    /*=== units ===*/
    //  static ALU1 and ALU2
    //  static CMP1 and CMP2
    IMM immUp_;
    IMM immDown_;
    /*=============*/

    /*=== inputs ===*/
    std::bitset<32> d1_;
    std::bitset<32> d2_;
    std::bitset<32> d4_;
    std::bitset<32> d5_;
    RISCVInstr instrUp_;
    RISCVInstr instrDown_;
    PC PC_EX_Up_;
    bool v_ex_up_{true};
    bool v_ex_down_{true};
    /*==============*/

    /*=== outputs ===*/
    std::bitset<32> alu_out_up_;
    std::bitset<32> alu_out_down_;
    PC PC_DISP_Up_;
    PC PC_DISP_Down_;
    bool PC_R_{false};
    bool restoreUp_{false};
    bool restoreDown_{false};
    WE_GEN we_gen_up_;
    WE_GEN we_gen_down_;
    /*===============*/

    /*=== fallthrough ===*/
    ControlUnit::Flags CONTROL_EX_Up_;
    ControlUnit::Flags CONTROL_EX_Down_;
    std::bitset<5> wb_a_up_;
    std::bitset<5> wb_a_down_;
    std::bitset<32> rs2v_;
    std::bitset<32> rs5v_;
    /*===================*/
};

#endif //SIMULATOR_EXECUTE_H
