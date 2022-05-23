#include <iomanip>
#include "simulator.h"
#include "macros.h"

Simulator::Simulator(uint32_t instr_count) {
    fetch_ = Fetch{instr_count};
    decode_ = Decode{};
    execute_ = Execute{};
    memory_ = Memory{};
    write_back_ = WriteBack{};
}

Simulator::Simulator(std::vector<std::bitset<32>> &&imem) {
    fetch_ = Fetch{std::move(imem)};
    decode_ = Decode{};
    execute_ = Execute{};
    memory_ = Memory{};
    write_back_ = WriteBack{};
}

PipelineState Simulator::Run() {
    PipelineState state;
    while (true) {
        ASSERT_STATE(hu_.exception_state)
        ASSERT_STATE(write_back_.Run(*this))
        ASSERT_STATE(memory_.Run(*this))
        ASSERT_STATE(execute_.Run(*this))
        ASSERT_STATE(decode_.Run(*this))
        ASSERT_STATE(fetch_.Run(*this))
//        std::string mem_str_up;
//        if (memory_.mem_we_up_) {
//            mem_str_up = "D2: " + std::to_string(memory_.D2.to_ulong()) +
//                         " Adr: " + std::to_string(static_cast<int>(memory_.ALU_OUT(Way::UP).to_ulong()));
//        } else {
//            mem_str_up = "WB_WE: " + std::to_string(memory_.WB_WE(Way::UP)) +
//                         " Reg: " + std::to_string(memory_.WB_A(Way::UP).to_ulong()) +
//                         " Dat: " + std::to_string(memory_.getOutData(Way::UP).to_ulong());
//        }
//        std::string wb_str_up = "WB_WE: " + std::to_string(write_back_.WB_WE(Way::UP)) +
//                                " Reg: " + std::to_string(write_back_.WB_A(Way::UP).to_ulong()) +
//                                " Dat: " + std::to_string(write_back_.WB_D(Way::UP).to_ulong());
//
//        std::string mem_str_down;
//        if (memory_.mem_we_down_) {
//            mem_str_down = "D2: " + std::to_string(memory_.D2.to_ulong()) +
//                         " Adr: " + std::to_string(static_cast<int>(memory_.ALU_OUT(Way::DOWN).to_ulong()));
//        } else {
//            mem_str_down = "WB_WE: " + std::to_string(memory_.WB_WE(Way::DOWN)) +
//                         " Reg: " + std::to_string(memory_.WB_A(Way::DOWN).to_ulong()) +
//                         " Dat: " + std::to_string(memory_.getOutData(Way::DOWN).to_ulong());
//        }
//        std::string wb_str_down = "WB_WE: " + std::to_string(write_back_.WB_WE(Way::DOWN)) +
//                                " Reg: " + std::to_string(write_back_.WB_A(Way::DOWN).to_ulong()) +
//                                " Dat: " + std::to_string(write_back_.WB_D(Way::DOWN).to_ulong());
//        std::cout << std::left
//        << std::setw(24) << "|F" + std::to_string(fetch_.cycle) + ": " + fetch_.getInstr(Way::UP).ToString()
//        << std::setw(24) << "|D" + std::to_string(decode_.cycle) + ": " + decode_.getInstr(Way::UP).ToString()
//        << std::setw(24) << "|E" + std::to_string(execute_.cycle) + ": " + execute_.getInstr(Way::UP).ToString()
//         << std::setw(24) << "|M" + std::to_string(execute_.cycle) + ": " + mem_str_up + "    "
//         << std::setw(24) << "|W" + std::to_string(execute_.cycle) + ": " + wb_str_up << std::endl
//        << std::setw(24) << "|F" + std::to_string(fetch_.cycle) + ": " + fetch_.getInstr(Way::DOWN).ToString()
//        << std::setw(24) << "|D" + std::to_string(decode_.cycle) + ": " + decode_.getInstr(Way::DOWN).ToString()
//        << std::setw(24) << "|E" + std::to_string(execute_.cycle) + ": " + execute_.getInstr(Way::DOWN).ToString()
//         << std::setw(24) << "|M" + std::to_string(execute_.cycle) + ": " + mem_str_down + "    "
//         << std::setw(24) << "|W" + std::to_string(execute_.cycle) + ": " + wb_str_down << std::endl
//        << "========" << std::setw(10) << "a0 = " + std::to_string(decode_.getRegFile().Read(10).to_ulong())
//        << std::setw(10) << "a1 = " + std::to_string(decode_.getRegFile().Read(11).to_ulong())
//        << std::setw(10) << "a5 = " + std::to_string(decode_.getRegFile().Read(15).to_ulong()) << std::endl;
    }
}

void Simulator::FDtransmitData() {
    CYCLE_CONTROL(fetch_.cycle, decode_.cycle)
    if (hu_.pl_state != PipelineState::STALL_DOWN) {
        decode_.setInstr(fetch_.getInstr(Way::UP), Way::UP);
        decode_.setPC_Up(fetch_.getPC_Up());
        decode_.setPC_R_F(fetch_.PC_R());
    }
    decode_.setInstr(fetch_.getInstr(Way::DOWN), Way::DOWN);
    fetch_.applyPC();
    decode_.is_set = true;
}

void Simulator::DEtransmitData() {
    CYCLE_CONTROL(decode_.cycle, execute_.cycle)
    execute_.setV_EX(decode_.V_DE(Way::UP), decode_.V_DE(Way::DOWN));
    execute_.setD1_D2(decode_.getRD1(), decode_.getRD2());
    execute_.setD4_D5(decode_.getRD4(), decode_.getRD5());
    execute_.setInstr(decode_.getInstr(Way::UP), Way::UP);
    execute_.setInstr(decode_.getInstr(Way::DOWN), Way::DOWN);
    execute_.setPC_EX(decode_.getPC_Up());
    execute_.setControl_EX(decode_.getCUState(Way::UP), Way::UP);
    execute_.setControl_EX(decode_.getCUState(Way::DOWN), Way::DOWN);
    execute_.is_set = true;
}

void Simulator::EMtransmitData() {
    CYCLE_CONTROL(execute_.cycle, memory_.cycle)
    memory_.setWE_GEN(execute_.getWE_GEN(Way::UP), execute_.getWE_GEN(Way::DOWN));
    memory_.setWS(execute_.WS(Way::UP), execute_.WS(Way::DOWN));
    memory_.setLWidth(execute_.MEM_WIDTH(Way::UP), execute_.MEM_WIDTH(Way::DOWN));
    memory_.setD2(execute_.RS2V());
    memory_.setD5(execute_.RS5V());
    memory_.setALU_OUT(execute_.ALU_OUT(Way::UP), execute_.ALU_OUT(Way::DOWN));
    memory_.setWB_A(execute_.WB_A(Way::UP), execute_.WB_A(Way::DOWN));
    memory_.is_set = true;
}

void Simulator::MWBtransmitData() {
    CYCLE_CONTROL(memory_.cycle, write_back_.cycle)
    write_back_.setWB_WE(memory_.WB_WE(Way::UP), memory_.WB_WE(Way::DOWN));
    write_back_.setEBREAK(memory_.EBREAK());
    write_back_.setWB_D(memory_.getOutData(Way::UP), memory_.getOutData(Way::DOWN));
    write_back_.setWB_A(memory_.WB_A(Way::UP), memory_.WB_A(Way::DOWN));
    write_back_.is_set = true;
}
