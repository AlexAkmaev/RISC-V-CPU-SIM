#include "simulator.h"
#include <gtest/gtest.h>

TEST(HazardUnitTests, ByPassFromMem) {
    /*
        li t0, 6
        xori a1, t0, 3
    */

    std::vector<std::bitset<32>> imem = {
        0x00600293,
        0x0032c593
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 6 */ 0x00000006});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 5 */ 0x00000005});
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
