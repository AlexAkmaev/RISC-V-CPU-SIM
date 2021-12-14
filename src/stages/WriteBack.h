#ifndef SIMULATOR_WRITEBACK_H
#define SIMULATOR_WRITEBACK_H

#include "units/Basics.h"

class WriteBack final : public Stage {
public:
    explicit WriteBack() = default;
    bool Run(Simulator &cpu) override;
    bool Stall() override;

    [[nodiscard]] std::bitset<32> BP_WB() const noexcept;
    [[nodiscard]] std::bitset<5> WB_A() const noexcept;
    [[nodiscard]] std::bitset<32> WB_D() const noexcept;
    [[nodiscard]] bool WB_WE() const noexcept;
    void setWB_A(std::bitset<5> wb_a);
    void setWB_D(std::bitset<32> wb_d);
    void setWB_WE(bool wb_we);
private:
    std::bitset<32> bp_wb_;
    std::bitset<5> wb_a_;
    std::bitset<32> wb_d_;
    bool wb_we_{false};
};

#endif //SIMULATOR_WRITEBACK_H
