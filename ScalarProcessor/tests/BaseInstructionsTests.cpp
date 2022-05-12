#include "simulator.h"
#include <gtest/gtest.h>

/*====================================================================*/
/*========== RV32I R-type arithmetic instructions tests ==============*/
/*====================================================================*/

TEST(BaseInstructionsTest, ADD) {
    /*
        add a0, t1, t0
    */

    std::vector<std::bitset<32>> imem = {
        0x00530533
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // t0 = 67
    // t1 = 13
    cpu.decode_.writeToRF({5}, {67}, true);
    cpu.decode_.writeToRF({6}, {13}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 67 */ 0x00000043});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 6}), std::bitset<32>{/* 13 */ 0x0000000d});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 80 */ 0x00000050});
}

TEST(BaseInstructionsTest, SUB) {
    /*
        sub a0, t1, t0
    */

    std::vector<std::bitset<32>> imem = {
        0x40530533
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // t0 = 67
    // t1 = 13
    cpu.decode_.writeToRF({5}, {67}, true);
    cpu.decode_.writeToRF({6}, {13}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 67 */ 0x00000043});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 6}), std::bitset<32>{/* 13 */ 0x0000000d});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -54 */ 0xffffffca});
}

TEST(BaseInstructionsTest, XOR) {
    /*
        xor a0, a0, t5
    */

    std::vector<std::bitset<32>> imem = {
        0x01e54533
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = 41
    // t5 = 42
    cpu.decode_.writeToRF({10}, {41}, true);
    cpu.decode_.writeToRF({30}, {42}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 41 */ 0x00000029});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t5 */ 30}), std::bitset<32>{/* 42 */ 0x0000002a});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 3 */ 0x00000003});
}

TEST(BaseInstructionsTest, OR) {
    /*
        or s0, a0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x00756433
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = 2
    // t2 = 4
    cpu.decode_.writeToRF({10}, {2}, true);
    cpu.decode_.writeToRF({7}, {4}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 2 */ 0x00000002});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 4 */ 0x00000004});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* 6 */ 0x00000006});
}

TEST(BaseInstructionsTest, AND) {
    /*
        and s0, a0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x00757433
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -45
    // t2 = 1342
    cpu.decode_.writeToRF({10}, {0xffffffd3}, true);
    cpu.decode_.writeToRF({7}, {1342}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -45 */ 0xffffffd3});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 1342 */ 0x0000053e});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* 1298 */ 0x00000512});
}

TEST(BaseInstructionsTest, SLL) {
    /*
        sll s0, a0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x00751433
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = 20
    // t2 = 5
    cpu.decode_.writeToRF({10}, {20}, true);
    cpu.decode_.writeToRF({7}, {5}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 20 */ 0x00000014});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 5 */ 0x00000005});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* 640 */ 0x00000280});
}

TEST(BaseInstructionsTest, SRL) {
    /*
        srl s0, a0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x00755433
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -100
    // t2 = 5
    cpu.decode_.writeToRF({10}, {0xffffff9c}, true);
    cpu.decode_.writeToRF({7}, {5}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -100 */ 0xffffff9c});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 5 */ 0x00000005});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* 134217724 */ 0x07fffffc});
}

TEST(BaseInstructionsTest, SRA) {
    /*
        sra s0, a0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x40755433
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -100
    // t2 = 5
    cpu.decode_.writeToRF({10}, {0xffffff9c}, true);
    cpu.decode_.writeToRF({7}, {5}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -100 */ 0xffffff9c});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 5 */ 0x00000005});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* -4 */ 0xfffffffc});
}

TEST(BaseInstructionsTest, SLT) {
    /*
        slt s0, a0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x00752433
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -100
    // t2 = 5
    cpu.decode_.writeToRF({10}, {0xffffff9c}, true);
    cpu.decode_.writeToRF({7}, {5}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -100 */ 0xffffff9c});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 5 */ 0x00000005});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* true */ 0x00000001});
}

TEST(BaseInstructionsTest, SLTU) {
    /*
        sltu s0, a0, t2
    */

    std::vector<std::bitset<32>> imem = {
        0x00753433
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -100
    // t2 = 5
    cpu.decode_.writeToRF({10}, {0xffffff9c}, true);
    cpu.decode_.writeToRF({7}, {5}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -100 */ 0xffffff9c});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t2 */ 7}), std::bitset<32>{/* 5 */ 0x00000005});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* false */ 0x00000000});
}

/*====================================================================*/
/*========== RV32I I-type arithmetic instructions tests ==============*/
/*====================================================================*/

TEST(BaseInstructionsTest, ADDI) {
    /*
        addi a0, x0, 15
    */

    std::vector<std::bitset<32>> imem = {
        0x00f00513,
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 15 */ 0x0000000f});
}

TEST(BaseInstructionsTest, XORI) {
    /*
        xori a1, t0, 3
    */

    std::vector<std::bitset<32>> imem = {
        0x0032c593
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // t0 = 6
    cpu.decode_.writeToRF({5}, {6}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* t0 */ 5}), std::bitset<32>{/* 6 */ 0x00000006});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 5 */ 0x00000005});
}

TEST(BaseInstructionsTest, SRAI) {
    /*
        srai s0, a0, 5
    */

    std::vector<std::bitset<32>> imem = {
        0x40555413
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -100
    cpu.decode_.writeToRF({10}, {0xffffff9c}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -100 */ 0xffffff9c});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* -4 */ 0xfffffffc});
}

TEST(BaseInstructionsTest, SLTI) {
    /*
        slti s0, a0, 5
    */

    std::vector<std::bitset<32>> imem = {
        0x00552413
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -100
    cpu.decode_.writeToRF({10}, {0xffffff9c}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -100 */ 0xffffff9c});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* true */ 0x00000001});
}

TEST(BaseInstructionsTest, SLTIU) {
    /*
        sltiu s0, a0, 5
    */

    std::vector<std::bitset<32>> imem = {
        0x00553413
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = -100
    cpu.decode_.writeToRF({10}, {0xffffff9c}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* -100 */ 0xffffff9c});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* s0 */ 8}), std::bitset<32>{/* false */ 0x00000000});
}

/*====================================================================*/
/*============= RV32I I-type load instructions tests =================*/
/*====================================================================*/

TEST(BaseInstructionsTest, LW) {
    /*
        lw a1, 16(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x01002583
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Data Memory setup ===*/
    // Mem[x0 + 16] = -500
    cpu.memory_.storeToDMEM({0xfffffe0c}, {16});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({16}), std::bitset<32>{/* -500 */ 0xfffffe0c});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* -500 */ 0xfffffe0c});
}

TEST(BaseInstructionsTest, LH) {
    /*
        lh a1, 4(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x00401583
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Data Memory setup ===*/
    // Mem[x0 + 4] = 45076
    cpu.memory_.storeToDMEM({45076}, {4});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({4}), std::bitset<32>{/* 45076 */ 0x0000b014});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* -20460 */ 0xffffb014});
}

TEST(BaseInstructionsTest, LHU) {
    /*
        lhu a1, 4(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x00405583
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Data Memory setup ===*/
    // Mem[x0 + 4] = 45076
    cpu.memory_.storeToDMEM({45076}, {4});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({4}), std::bitset<32>{/* 45076 */ 0x0000b014});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 45076 */ 0x0000b014});
}

TEST(BaseInstructionsTest, LB) {
    /*
        lb a1, 32(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x02000583
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Data Memory setup ===*/
    // Mem[x0 + 32] = 2737
    cpu.memory_.storeToDMEM({2737}, {32});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({32}), std::bitset<32>{/* 2737 */ 0x00000ab1});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 20 */ 0xffffffb1});
}

TEST(BaseInstructionsTest, LBU) {
    /*
        lbu a1, 32(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x02004583
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Data Memory setup ===*/
    // Mem[x0 + 32] = 2737
    cpu.memory_.storeToDMEM({2737}, {32});
    ASSERT_EQ(cpu.memory_.loadFromDMEM({32}), std::bitset<32>{/* 2737 */ 0x00000ab1});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 20 */ 0x000000b1});
}

/*====================================================================*/
/*============= RV32I S-type store instructions tests ================*/
/*====================================================================*/

TEST(BaseInstructionsTest, SW) {
    /*
        sw a0, 8(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x00a02423
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = 132017
    cpu.decode_.writeToRF({10}, {132017}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 132017 */ 0x000203b1});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.memory_.loadFromDMEM({8}), std::bitset<32>{/* 132017 */ 0x000203b1});
}

TEST(BaseInstructionsTest, SH) {
    /*
        sh a0, 8(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x00a01423
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = 132017
    cpu.decode_.writeToRF({10}, {132017}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 132017 */ 0x000203b1});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.memory_.loadFromDMEM({8}, DMEM::Width::HALF_U), std::bitset<32>{/* 945 */ 0x000003b1});
}

TEST(BaseInstructionsTest, SB) {
    /*
        sb a0, 8(zero)
    */

    std::vector<std::bitset<32>> imem = {
        0x00a00423
    };

    Simulator cpu = Simulator{std::move(imem)};
    /*=== Register file setup ===*/
    // a0 = 132017
    cpu.decode_.writeToRF({10}, {132017}, true);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 132017 */ 0x000203b1});
    /*===========================*/

    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.memory_.loadFromDMEM({8}, DMEM::Width::BYTE_U), std::bitset<32>{/* 177 */ 0x000000b1});
}

/*====================================================================*/
/*================ RV32I U-type instructions tests ===================*/
/*====================================================================*/

TEST(BaseInstructionsTest, LUI) {
    /*
        lui a1, 42
    */

    std::vector<std::bitset<32>> imem = {
        0x0002a5b7
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 172032 */ 0x0002a000});
}

TEST(BaseInstructionsTest, AUIPC) {
    /*
        li a0, 2
        auipc a1, 42
    */

    std::vector<std::bitset<32>> imem = {
        0x00200513,
        0x0002a597
    };

    Simulator cpu = Simulator{std::move(imem)};
    ASSERT_NE(cpu.Run(), PipelineState::ERR);
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a0 */ 10}), std::bitset<32>{/* 2 */ 0x00000002});
    ASSERT_EQ(cpu.decode_.getRegFile().Read({/* a1 */ 11}), std::bitset<32>{/* 172036 */ 0x0002a004});
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
