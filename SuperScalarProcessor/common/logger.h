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
//        std::string mem_str;
//        if (memory_.mem_we_) {
//            mem_str = "D2: " + std::to_string(memory_.D2.to_ulong()) +
//                " Adr: " + std::to_string(static_cast<int>(memory_.ALU_OUT().to_ulong()));
//        } else {
//            mem_str = "WB_WE: " + std::to_string(memory_.WB_WE()) +
//                " Reg: " + std::to_string(memory_.WB_A().to_ulong()) +
//                " Dat: " + std::to_string(memory_.getOutData().to_ulong());
//        }
//        std::string wb_str = "WB_WE: " + std::to_string(write_back_.WB_WE()) +
//                             " Reg: " + std::to_string(write_back_.WB_A().to_ulong()) +
//                             " Dat: " + std::to_string(write_back_.WB_D().to_ulong());
//        std::cout << std::left
//        << std::setw(24) << "|F" + std::to_string(fetch_.cycle) + ": " + fetch_.getInstr().ToString()
//        << std::setw(24) << "|D" + std::to_string(decode_.cycle) + ": " + decode_.getInstr().ToString()
//        << std::setw(24) << "|E" + std::to_string(execute_.cycle) + ": " + execute_.getInstr().ToString()
//        << std::setw(24) << "|M" + std::to_string(execute_.cycle) + ": " + mem_str + "    "
//        << std::setw(24) << "|W" + std::to_string(execute_.cycle) + ": " + wb_str << std::endl
//        << "========" << std::setw(10) << "a0 = " + std::to_string(decode_.getRegFile().Read(10).to_ulong())
//        << std::setw(10) << "a1 = " + std::to_string(decode_.getRegFile().Read(11).to_ulong())
//        << std::setw(10) << "a5 = " + std::to_string(decode_.getRegFile().Read(15).to_ulong()) << std::endl;
    }

private:
    // cycles -> F, D, E, M, WB
    std::map<uint32_t, std::set<std::string>> out_log_;
};

#endif // RISCV_SIMULATOR_LOGGER_H
