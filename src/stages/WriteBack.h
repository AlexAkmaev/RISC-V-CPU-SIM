#ifndef SIMULATOR_WRITEBACK_H
#define SIMULATOR_WRITEBACK_H

#include "units/Basics.h"

class WriteBack final : public Stage {
public:
    explicit WriteBack() = default;
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] bool WB_WE() const noexcept;
    [[nodiscard]] std::bitset<5> WB_A() const noexcept;
    [[nodiscard]] std::bitset<32> WB_D() const noexcept;

    void setWB_A(std::bitset<5> wb_a);
    void setWB_D(std::bitset<32> wb_d);
    void setWB_WE(bool wb_we);

    bool is_set{false};
private:
    bool wb_we_{false};
    std::bitset<32> wb_d_;
    std::bitset<5> wb_a_;
};

#endif //SIMULATOR_WRITEBACK_H
