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

TEST(BlocksTest, Jalr) {
    /*
        li t0, 5
        li t1, 20
        jalr t1
        add a0, t1, t0
        addi a0, a0, -2
        addi a1, a0, 10
    */

    std::vector<std::bitset<32>> imem = {
        0x00500293,
        0x01400313,
        0x000300e7,
        0x00530533,
        0xffe50513,
        0x00a50593
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 5 */ 0x00000005});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t1 */ 6}), std::bitset<32>{/* 20 */ 0x00000014});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 0 */ 0x00000000});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 10 */ 0x0000000a});
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

TEST(BlocksTest, Loop2) {
    /* C code:
                int num = 20;
                int i = 0;
                int sum = 0;
                for (i = 0; i < num; ++i) {
                    if (i < 5) {
                        sum += i * 2;
                    } else {
                        sum += 1;
                    }
                }
                sum = sum - 1;
     */
    /*  RV32I assembly:
                    addi    sp, sp, -32
                    sw      ra, 28(sp)
                    sw      s0, 24(sp)
                    addi    s0, sp, 32
                    addi    a0, zero, 20
                    sw      a0, -12(s0)
                    mv      a0, zero
                    sw      a0, -16(s0)
                    sw      a0, -20(s0)
                    sw      a0, -16(s0)
                    j       .LBB0_1
            .LBB0_1:                                # =>This Inner Loop Header: Depth=1
                    lw      a0, -16(s0)
                    lw      a1, -12(s0)
                    bge     a0, a1, .LBB0_7
                    j       .LBB0_2
            .LBB0_2:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    addi    a1, zero, 4
                    blt     a1, a0, .LBB0_4
                    j       .LBB0_3
            .LBB0_3:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    slli    a0, a0, 1
                    lw      a1, -20(s0)
                    add     a0, a0, a1
                    sw      a0, -20(s0)
                    j       .LBB0_5
            .LBB0_4:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -20(s0)
                    addi    a0, a0, 1
                    sw      a0, -20(s0)
                    j       .LBB0_5
            .LBB0_5:                                #   in Loop: Header=BB0_1 Depth=1
                    j       .LBB0_6
            .LBB0_6:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    addi    a0, a0, 1
                    sw      a0, -16(s0)
                    j       .LBB0_1
            .LBB0_7:
                    lw      a0, -20(s0)
                    addi    a0, a0, -1
                    sw      a0, -20(s0)
                    lw      s0, 24(sp)
                    lw      ra, 28(sp)
                    addi    sp, sp, 32
    */

    std::vector<std::bitset<32>> imem = {
        0xfe010113,
        0x00112e23,
        0x00812c23,
        0x02010413,
        0x01400513,
        0xfea42a23,
        0x00000513,
        0xfea42823,
        0xfea42623,
        0xfea42823,
        0x0040006f,
        0xff042503,
        0xff442583,
        0x04b55a63,
        0x0040006f,
        0xff042503,
        0x00400593,
        0x02a5c063,
        0x0040006f,
        0xff042503,
        0x00151513,
        0xfec42583,
        0x00b50533,
        0xfea42623,
        0x0140006f,
        0xfec42503,
        0x00150513,
        0xfea42623,
        0x0040006f,
        0x0040006f,
        0xff042503,
        0x00150513,
        0xfea42823,
        0xfa9ff06f,
        0xfec42503,
        0xfff50513,
        0xfea42623,
        0x01812403,
        0x01c12083,
        0x02010113
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 34 */ 0x00000022});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 20 */ 0x00000014});
}

TEST(BlocksTest, Loop3) {
    /* C code:
                int num = 7;
                int i = 0;
                int sum = 0;
                for (i = 0; i < num; ++i) {
                    if (i < 5) {
                        sum += i * 2;
                    } else {
                        sum += 1;
                    }
                }
                sum = sum - 1;
     */
    /*  RV32I assembly:
                    addi    sp, sp, -32
                    sw      ra, 28(sp)
                    sw      s0, 24(sp)
                    addi    s0, sp, 32
                    addi    a0, zero, 7
                    sw      a0, -12(s0)
                    mv      a0, zero
                    sw      a0, -16(s0)
                    sw      a0, -20(s0)
                    sw      a0, -16(s0)
                    j       .LBB0_1
            .LBB0_1:                                # =>This Inner Loop Header: Depth=1
                    lw      a0, -16(s0)
                    lw      a1, -12(s0)
                    bge     a0, a1, .LBB0_7
                    j       .LBB0_2
            .LBB0_2:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    addi    a1, zero, 4
                    blt     a1, a0, .LBB0_4
                    j       .LBB0_3
            .LBB0_3:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    slli    a0, a0, 1
                    addi    a5, a0, 0
                    lw      a1, -20(s0)
                    add     a0, a0, a1
                    sw      a0, -20(s0)
                    j       .LBB0_5
            .LBB0_4:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -20(s0)
                    addi    a0, a0, 1
                    addi    a5, a0, 0
                    sw      a0, -20(s0)
                    j       .LBB0_5
            .LBB0_5:                                #   in Loop: Header=BB0_1 Depth=1
                    j       .LBB0_6
            .LBB0_6:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    addi    a0, a0, 1
                    sw      a0, -16(s0)
                    j       .LBB0_1
            .LBB0_7:
                    lw      a0, -20(s0)
                    addi    a0, a0, -1
                    sw      a0, -20(s0)
                    lw      s0, 24(sp)
                    lw      ra, 28(sp)
                    addi    sp, sp, 32
    */

    std::vector<std::bitset<32>> imem = {
        0xfe010113,
        0x00112e23,
        0x00812c23,
        0x02010413,
        0x00700513,
        0xfea42a23,
        0x00000513,
        0xfea42823,
        0xfea42623,
        0xfea42823,
        0x0040006f,
        0xff042503,
        0xff442583,
        0x04b55e63,
        0x0040006f,
        0xff042503,
        0x00400593,
        0x02a5c263,
        0x0040006f,
        0xff042503,
        0x00151513,
        0x00050793,
        0xfec42583,
        0x00b50533,
        0xfea42623,
        0x0180006f,
        0xfec42503,
        0x00150513,
        0x00050793,
        0xfea42623,
        0x0040006f,
        0x0040006f,
        0xff042503,
        0x00150513,
        0xfea42823,
        0xfa1ff06f,
        0xfec42503,
        0xfff50513,
        0xfea42623,
        0x01812403,
        0x01c12083,
        0x02010113
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    cpu.decode_.writeToRF({2}, {0x7ffffff0}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 21 */ 0x00000015});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 7 */ 0x00000007});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a5 */ 15}), std::bitset<32>{/* 22 */ 0x00000016});
}

TEST(BlocksTest, Loop4) {
    /* C code:
                int num = 2;
                int i = 0;
                int sum = 0;
                for (i = 0; i < num; ++i) {
                    if (i < 1) {
                        sum += i * 2;
                    } else {
                        sum += 1;
                    }
                }
                sum = sum - 1;
     */
    /*  RV32I assembly:
                    addi    sp, sp, -32
                    sw      ra, 28(sp)
                    sw      s0, 24(sp)
                    addi    s0, sp, 32
                    addi    a0, zero, 7
                    sw      a0, -12(s0)
                    mv      a0, zero
                    sw      a0, -16(s0)
                    sw      a0, -20(s0)
                    sw      a0, -16(s0)
                    j       .LBB0_1
            .LBB0_1:                                # =>This Inner Loop Header: Depth=1
                    lw      a0, -16(s0)
                    lw      a1, -12(s0)
                    bge     a0, a1, .LBB0_7
                    j       .LBB0_2
            .LBB0_2:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    addi    a1, zero, 4
                    blt     a1, a0, .LBB0_4
                    j       .LBB0_3
            .LBB0_3:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    slli    a0, a0, 1
                    addi    a5, a0, 0
                    lw      a1, -20(s0)
                    add     a0, a0, a1
                    sw      a0, -20(s0)
                    j       .LBB0_5
            .LBB0_4:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -20(s0)
                    addi    a0, a0, 1
                    addi    a5, a0, 0
                    sw      a0, -20(s0)
                    j       .LBB0_5
            .LBB0_5:                                #   in Loop: Header=BB0_1 Depth=1
                    j       .LBB0_6
            .LBB0_6:                                #   in Loop: Header=BB0_1 Depth=1
                    lw      a0, -16(s0)
                    addi    a0, a0, 1
                    sw      a0, -16(s0)
                    j       .LBB0_1
            .LBB0_7:
                    lw      a0, -20(s0)
                    addi    a0, a0, -1
                    sw      a0, -20(s0)
                    lw      s0, 24(sp)
                    lw      ra, 28(sp)
                    addi    sp, sp, 32
    */

    std::vector<std::bitset<32>> imem = {
        0xfe010113,
        0x00112e23,
        0x00812c23,
        0x02010413,
        0x00300513,
        0xfea42a23,
        0x00000513,
        0xfea42823,
        0xfea42623,
        0xfea42823,
        0x0040006f,
        0xff042503,
        0xff442583,
        0x04b55a63,
        0x0040006f,
        0xff042503,
        0x00100593,
        0x02a5c063,
        0x0040006f,
        0xff042503,
        0x00151513,
        0xfec42583,
        0x00b50533,
        0xfea42623,
        0x0140006f,
        0xfec42503,
        0x00150513,
        0xfea42623,
        0x0040006f,
        0x0040006f,
        0xff042503,
        0x00150513,
        0xfea42823,
        0xfa9ff06f,
        0xfec42503,
        0xfff50513,
        0xfea42623,
        0x01812403,
        0x01c12083,
        0x02010113
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    cpu.decode_.writeToRF({2}, {0x7ffffff0}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 21 */ 0x0000002});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 7 */ 0x00000003});
//    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a5 */ 15}), std::bitset<32>{/* 22 */ 0x00000003});
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
