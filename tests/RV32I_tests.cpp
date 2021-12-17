#include "simulator.h"
#include <gtest/gtest.h>

TEST(SimulatorTest, test1) {
    /*
        addi a0, x0, 15
        jr x0
    */

    std::vector<std::bitset<32>> imem = {
        0x00f00513,
        0x00000067
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 15 */ 0x0000000f});
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
