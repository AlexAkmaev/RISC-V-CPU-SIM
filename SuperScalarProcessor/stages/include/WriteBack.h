#ifndef SIMULATOR_WRITEBACK_H
#define SIMULATOR_WRITEBACK_H

#include "Basics.h"

class WriteBack final : public Stage {
public:
    explicit WriteBack() = default;
    PipelineState Run(Simulator &cpu) override;

    [[nodiscard]] bool WB_WE(Way way) const noexcept;
    [[nodiscard]] std::bitset<5> WB_A(Way way) const noexcept;
    [[nodiscard]] std::bitset<32> WB_D(Way way) const noexcept;

    void setWB_A(std::bitset<5> wb_a_up, std::bitset<5> wb_a_down);
    void setWB_D(std::bitset<32> wb_d_up, std::bitset<32> wb_d_down);
    void setWB_WE(bool wb_we_up, bool wb_we_down);
    void setEBREAK(bool eb);

    bool is_set{false};
private:
    bool wb_we_up_{false};
    bool wb_we_down_{false};
    bool ebreak_{false};
    std::bitset<32> wb_d_up_;
    std::bitset<32> wb_d_down_;
    std::bitset<5> wb_a_up_;
    std::bitset<5> wb_a_down_;
};

#endif //SIMULATOR_WRITEBACK_H
