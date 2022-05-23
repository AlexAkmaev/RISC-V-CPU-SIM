#ifndef RISCV_SIMULATOR_LOGGER_H
#define RISCV_SIMULATOR_LOGGER_H

#include <cstdint>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>

class Logger {
public:
    enum Stage : uint8_t {
        FETCH,
        DECODE,
        EXECUTE,
        MEMORY,
        WRITE_BACK
    };

    enum LogLevel : uint8_t {
        L0,
        L1,
        L2
    };

    class Message {
    public:
        explicit Message(Stage stage, uint32_t cycle, LogLevel lvl = L0) : stage_(stage), cycle_(cycle), lvl_(lvl) {
            std::string stage_str;
            switch (stage) {
                case FETCH:
                    stage_str = "F";
                    break;
                case DECODE:
                    stage_str = "D";
                    break;
                case EXECUTE:
                    stage_str = "E";
                    break;
                case MEMORY:
                    stage_str = "M";
                    break;
                case WRITE_BACK:
                    stage_str = "W";
                    break;
            }
            stream_ << std::setprecision(6) << "| " << stage_str << " |";
        }

        Stage getStage() {
            return stage_;
        }

        uint32_t getCycle() {
            return cycle_;
        }

        std::ostream &GetStream() {
            return stream_;
        }

    private:
        Stage stage_;
        uint32_t cycle_;
        LogLevel lvl_;
        std::ostringstream stream_;
    };

    // TODO:
    void Dump() {
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
//                           " Adr: " + std::to_string(static_cast<int>(memory_.ALU_OUT(Way::DOWN).to_ulong()));
//        } else {
//            mem_str_down = "WB_WE: " + std::to_string(memory_.WB_WE(Way::DOWN)) +
//                           " Reg: " + std::to_string(memory_.WB_A(Way::DOWN).to_ulong()) +
//                           " Dat: " + std::to_string(memory_.getOutData(Way::DOWN).to_ulong());
//        }
//        std::string wb_str_down = "WB_WE: " + std::to_string(write_back_.WB_WE(Way::DOWN)) +
//                                  " Reg: " + std::to_string(write_back_.WB_A(Way::DOWN).to_ulong()) +
//                                  " Dat: " + std::to_string(write_back_.WB_D(Way::DOWN).to_ulong());
//        std::cout << std::left
//                  << std::setw(24) << "|F" + std::to_string(fetch_.cycle) + ": " + fetch_.getInstr(Way::UP).ToString()
//                  << std::setw(24) << "|D" + std::to_string(decode_.cycle) + ": " + decode_.getInstr(Way::UP).ToString()
//                  << std::setw(24) << "|E" + std::to_string(execute_.cycle) + ": " + execute_.getInstr(Way::UP).ToString()
//                  << std::setw(24) << "|M" + std::to_string(execute_.cycle) + ": " + mem_str_up + "    "
//                  << std::setw(24) << "|W" + std::to_string(execute_.cycle) + ": " + wb_str_up << std::endl
//                  << std::setw(24) << "|F" + std::to_string(fetch_.cycle) + ": " + fetch_.getInstr(Way::DOWN).ToString()
//                  << std::setw(24) << "|D" + std::to_string(decode_.cycle) + ": " + decode_.getInstr(Way::DOWN).ToString()
//                  << std::setw(24) << "|E" + std::to_string(execute_.cycle) + ": " + execute_.getInstr(Way::DOWN).ToString()
//                  << std::setw(24) << "|M" + std::to_string(execute_.cycle) + ": " + mem_str_down + "    "
//                  << std::setw(24) << "|W" + std::to_string(execute_.cycle) + ": " + wb_str_down << std::endl
//                  << "========" << std::setw(10) << "a0 = " + std::to_string(decode_.getRegFile().Read(10).to_ulong())
//                  << std::setw(10) << "a1 = " + std::to_string(decode_.getRegFile().Read(11).to_ulong())
//                  << std::setw(10) << "a5 = " + std::to_string(decode_.getRegFile().Read(15).to_ulong()) << std::endl;
    }

private:
    // cycles -> F, D, E, M, WB
    std::map<uint32_t, std::set<std::string>> out_log_;
};

#endif // RISCV_SIMULATOR_LOGGER_H
