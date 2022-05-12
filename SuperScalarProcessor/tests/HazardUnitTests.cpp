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

TEST(HazardUnitTests, ByPassFromWB) {
    /*
        li t0, 23
        li t1, 25
        bgt t0, t1, end
        sub a0, t1, t0
        end:
    */

    std::vector<std::bitset<32>> imem = {
        0x01700293,
        0x01900313,
        0x00534463,
        0x40530533
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 23 */ 0x00000017});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 25 */ 0x00000019});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 2 */ 0x00000002});
}

TEST(HazardUnitTests, LoadHazard) {
    /*
        li t2, 5
        li a0, 11
        sw a0, 40(zero)
        lw s0, 40(zero)
        andi t0, s0, 234
        or t1, t0, s0
        sub t2, s0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x00500393,
        0x00b00513,
        0x02a02423,
        0x02802403,
        0x0ea47293,
        0x0082e333,
        0x407403b3
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 11 */ 0x0000000b});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* 11 */ 0x0000000b});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 10 */ 0x0000000a});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 25 */ 0x0000000b});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 6 */ 0x00000006});
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
