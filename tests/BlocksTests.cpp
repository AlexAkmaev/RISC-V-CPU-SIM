#include "simulator.h"
#include <gtest/gtest.h>

TEST(BlocksTest, Branch1) {
    /*
        li t0, 1
        li t1, 1
        bne t0, t1, end
        add a0, t1, t0
        end:
        add a1, a0, t1
    */

    std::vector<std::bitset<32>> imem = {
        0x00100293,
        0x00100313,
        0x00629463,
        0x00530533,
        0x006505b3
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 1 */ 0x00000001});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 1 */ 0x00000001});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 2 */ 0x00000002});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 3 */ 0x00000003});
}

TEST(BlocksTest, Branch2) {
    /*
        li t0, 0
        li t1, 1
        bne t0, t1, end
        add a0, t1, t0
        end:
        add a1, a0, t1
    */

    std::vector<std::bitset<32>> imem = {
        0x00000293,
        0x00100313,
        0x00629463,
        0x00530533,
        0x006505b3
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 1 */ 0x00000001});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 1 */ 0x00000001});
}

TEST(BlocksTest, Branch3) {
    /*
        li t0, 0
        li t1, 1
        li t2, 2
        bge t0, t1, end
        add a0, t1, t0
        add a0, a0, t1
        add a0, a0, t2
        addi a0, a0, 10
        end:
    */

    std::vector<std::bitset<32>> imem = {
        0x00000293, 0x00100313, 0x00200393, 0x0062da63, 0x00530533,
        0x00650533, 0x00750533, 0x00a50513
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 1 */ 0x00000001});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 1 */ 0x00000002});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 14 */ 0x0000000e});
}

TEST(BlocksTest, Branch4) {
    /*
        li t0, 0
        li t1, 1
        li t2, 2
        blt t0, t1, end
        add a0, t1, t0
        add a0, a0, t1
        add a0, a0, t2
        addi a0, a0, 10
        end:
        sub a1, t2, t1
        addi a1, a1, -2
    */

    std::vector<std::bitset<32>> imem = {
        0x00000293, 0x00100313, 0x00200393, 0x0062ca63, 0x00530533,
        0x00650533, 0x00750533, 0x00a50513, 0x406385b3, 0xffe58593
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 1 */ 0x00000001});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 1 */ 0x00000002});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* -1 */ 0xffffffff});
}

TEST(BlocksTest, IfElse) {
    /*
        li t3, 22
        li t2, 2
        li t1, 42
        li t0, 42
        bne t0, t1, Else
        addi a0, t2, 6
        j Exit
        Else: sub a0, t2, t3
        Exit:
    */

    std::vector<std::bitset<32>> imem = {
        0x01600e13, 0x00200393, 0x02a00313, 0x02a00293, 0x00629663,
        0x00638513, 0x0080006f, 0x41c38533
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 42 */ 0x0000002a});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 42 */ 0x0000002a});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 2 */ 0x00000002});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t3 */ 28}), std::bitset<32>{/* 22 */ 0x00000016});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 8 */ 0x00000008});
}

TEST(BlocksTest, Jumps) {
    /*
        li t0, 0
        li t1, 10
        j second
        first:
        addi a0, t1, 5
        addi a0, a0, 6
        add a0, a0, t1
        j end
        second:
        addi a0, t0, 3
        addi a0, a0, 9
        j first
        end:
        add a0, a0, t1
    */

    std::vector<std::bitset<32>> imem = {
        0x00000293, 0x00a00313, 0x0140006f, 0x00530513, 0x00650513, 0x00650533,
        0x0100006f, 0x00328513, 0x00950513, 0xfe9ff06f, 0x00650533
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 10 */ 0x0000000a});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 41 */ 0x00000029});
}

TEST(BlocksTest, Loop1) {
    /*
        li t0, 0
        li t2, 10
        loop_head:
        bge t0, t2, loop_end
        addi t0, t0, 1
        j loop_head
        loop_end:
    */

    std::vector<std::bitset<32>> imem = {
        0x00000293,
        0x00a00393,
        0x0072d663,
        0x00128293,
        0xff9ff06f
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 10 */ 0x0000000a});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 10 */ 0x0000000a});
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
