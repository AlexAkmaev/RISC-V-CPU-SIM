#include "simulator.h"
#include <gtest/gtest.h>

TEST(BlocksTest, NoConflicts) {
    /*
        lw t0, 40(s0)
        add t1, s1, s2
        sub t2, s1, s3
        and t3, s3, s4
        or t4, s1, s5
        sw s4, 80(s0)
    */

    std::vector<std::bitset<32>> imem = {
        0x02842283,
        0x01248333,
        0x413483b3,
        0x0149fe33,
        0x0154eeb3,
        0x05442823
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // s1 = 5
    // s2 = 10
    // s3 = 2
    // s4 = 12
    // s5 = 7
    cpu.decode_.writeToRF({9}, {5}, true);
    cpu.decode_.writeToRF({18}, {10}, true);
    cpu.decode_.writeToRF({19}, {2}, true);
    cpu.decode_.writeToRF({20}, {12}, true);
    cpu.decode_.writeToRF({21}, {7}, true);
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 15 */ 0x0000000f});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 3 */ 0x00000003});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t3 */ 28}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t4 */ 29}), std::bitset<32>{/* 7 */ 0x00000007});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({80}), std::bitset<32>{/* 12 */ 0x0000000c});
}

TEST(BlocksTest, DataDepends_UpUp) {
    /*
        lw t0, 40(zero)
        add t1, t0, s1
        sub t0, s2, s3
        and t2, s4, t0
        or t3, s5, s6
        sw s7, 80(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x02802283,
        0x00928333,
        0x413902b3,
        0x005a73b3,
        0x016aee33,
        0x05702823
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // s1 = 5
    // s2 = 10
    // s3 = 2
    // s4 = 12
    // s5 = 7
    // s6 = 6
    // s7 = 55
    cpu.decode_.writeToRF({9}, {5}, true);
    cpu.decode_.writeToRF({18}, {10}, true);
    cpu.decode_.writeToRF({19}, {2}, true);
    cpu.decode_.writeToRF({20}, {12}, true);
    cpu.decode_.writeToRF({21}, {7}, true);
    cpu.decode_.writeToRF({22}, {6}, true);
    cpu.decode_.writeToRF({23}, {55}, true);
    /*===========================*/

    /*==== Data Memory setup ====*/
    // Mem[x0 + 40] = 5927859
    cpu.memory_.storeToDMEM({0x005a73b3}, {40});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({40}), std::bitset<32>{/* 5927859 */ 0x005a73b3});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 8 */ 0x00000008});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{0x005a73b8});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 8 */ 0x00000008});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t3 */ 28}), std::bitset<32>{/* 7 */ 0x00000007});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({80}), std::bitset<32>{/* 55 */ 0x00000037});
}

TEST(BlocksTest, DataDepends_UpDown) {
    /*
        lw t0, 40(zero)     <== t0 Up
        li a0, -2
        add t1, s2, s1
        sub a0, t0, s3      <== t0 down
        and t2, s4, t0
        or t3, s5, s6
        sw s7, 80(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x02802283,
        0xffe00513,
        0x00990333,
        0x41328533,
        0x005a73b3,
        0x016aee33,
        0x05702823
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // s1 = 5
    // s2 = 10
    // s3 = 2
    // s4 = 12134
    // s5 = 7
    // s6 = 6
    // s7 = 55
    cpu.decode_.writeToRF({9}, {5}, true);
    cpu.decode_.writeToRF({18}, {10}, true);
    cpu.decode_.writeToRF({19}, {2}, true);
    cpu.decode_.writeToRF({20}, {12134}, true);
    cpu.decode_.writeToRF({21}, {7}, true);
    cpu.decode_.writeToRF({22}, {6}, true);
    cpu.decode_.writeToRF({23}, {55}, true);
    /*===========================*/

    /*==== Data Memory setup ====*/
    // Mem[x0 + 40] = 5927859
    cpu.memory_.storeToDMEM({0x005a73b3}, {40});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({40}), std::bitset<32>{/* 5927859 */ 0x005a73b3});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -4 */ 0x005a73b1});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{0x005a73b3});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 15 */ 0x0000000f});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 8994 */ 0x00002322});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t3 */ 28}), std::bitset<32>{/* 7 */ 0x00000007});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({80}), std::bitset<32>{/* 55 */ 0x00000037});
}

TEST(BlocksTest, DataDepends_DownUp) {
    /*
        li a0, -2
        lw t0, 40(zero)     <== t0 down
        sub a0, t0, s3      <== t0 up
        add t1, s2, s1
        and t2, s4, t0
        or t3, s5, s6
        sw s7, 80(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0xffe00513,
        0x02802283,
        0x41328533,
        0x00990333,
        0x005a73b3,
        0x016aee33,
        0x05702823
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // s1 = 5
    // s2 = 10
    // s3 = 2
    // s4 = 12
    // s5 = 7
    // s6 = 6
    // s7 = 55
    cpu.decode_.writeToRF({9}, {5}, true);
    cpu.decode_.writeToRF({18}, {10}, true);
    cpu.decode_.writeToRF({19}, {2}, true);
    cpu.decode_.writeToRF({20}, {12}, true);
    cpu.decode_.writeToRF({21}, {7}, true);
    cpu.decode_.writeToRF({22}, {6}, true);
    cpu.decode_.writeToRF({23}, {55}, true);
    /*===========================*/

    /*==== Data Memory setup ====*/
    // Mem[x0 + 40] = 5927859
    cpu.memory_.storeToDMEM({0x005a73b3}, {40});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({40}), std::bitset<32>{/* 5927859 */ 0x005a73b3});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -4 */ 0x005a73b1});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{0x005a73b3});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 15 */ 0x0000000f});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t3 */ 28}), std::bitset<32>{/* 7 */ 0x00000007});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({80}), std::bitset<32>{/* 55 */ 0x00000037});
}

TEST(BlocksTest, DataDepends_DownDown) {
    /*
        li a0, -2
        lw t0, 40(zero)     <== t0 down
        add t1, s2, s1
        sub a0, t0, s3      <== t0 down
        and t2, s4, t0
        or t3, s5, s6
        sw s7, 80(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0xffe00513,
        0x02802283,
        0x00990333,
        0x41328533,
        0x005a73b3,
        0x016aee33,
        0x05702823
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // s1 = 5
    // s2 = 10
    // s3 = 2
    // s4 = 12
    // s5 = 7
    // s6 = 6
    // s7 = 55
    cpu.decode_.writeToRF({9}, {5}, true);
    cpu.decode_.writeToRF({18}, {10}, true);
    cpu.decode_.writeToRF({19}, {2}, true);
    cpu.decode_.writeToRF({20}, {12}, true);
    cpu.decode_.writeToRF({21}, {7}, true);
    cpu.decode_.writeToRF({22}, {6}, true);
    cpu.decode_.writeToRF({23}, {55}, true);
    /*===========================*/

    /*==== Data Memory setup ====*/
    // Mem[x0 + 40] = 5927859
    cpu.memory_.storeToDMEM({0x005a73b3}, {40});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({40}), std::bitset<32>{/* 5927859 */ 0x005a73b3});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -4 */ 0x005a73b1});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{0x005a73b3});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 15 */ 0x0000000f});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t3 */ 28}), std::bitset<32>{/* 7 */ 0x00000007});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({80}), std::bitset<32>{/* 55 */ 0x00000037});
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
