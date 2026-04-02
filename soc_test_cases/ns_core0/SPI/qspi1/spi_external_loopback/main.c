/**
  * Copyright (c) 2019 Nuclei Limited. All rights reserved.
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  * Licensed under the Apache License, Version 2.0 (the License); you may
  * not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  * www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

/**
  * SPI External Loopback Test
  *
  * Purpose:
  *   Test external SPI data path without any device model or testbench.
  *   Uses QSPI1 (master, PAD0-PAD5) and QSPI2 (slave, PAD21-PAD26).
  *
  * Hardware wiring required (FPGA eval board):
  *   QSPI1 (Master)          QSPI2 (Slave)
  *   ─────────────────────────────────────────
  *   PAD0  (SCK)     ──────> PAD21 (SCK)
  *   PAD1  (CS0)     ──────> PAD22 (CS0)
  *   PAD2  (DQ0/MOSI)──────> PAD23 (DQ0/MOSI)
  *   PAD3  (DQ1/MISO)<────── PAD24 (DQ1/MISO)
  *
  * Test flow:
  *   1. Configure QSPI1 as master, QSPI2 as slave
  *   2. Master sends test pattern, slave receives
  *   3. Slave pre-loads response, master reads back
  *   4. Compare sent and received data
  *
  * This test verifies:
  *   - IOMUX pad routing for both controllers
  *   - SPI clock generation and propagation
  *   - CS assertion/deassertion
  *   - MOSI/MISO data integrity
  *   - Full-duplex data exchange
  */

#include <stdio.h>
#include <string.h>
#include "ns_sdk_hal.h"

/* ──────── Test parameters ──────── */
#define TEST_SIZE       16
#define TIMEOUT_COUNT   100000

/* ──────── Test patterns ──────── */
static uint8_t master_tx[TEST_SIZE];
static uint8_t master_rx[TEST_SIZE];
static uint8_t slave_tx[TEST_SIZE];
static uint8_t slave_rx[TEST_SIZE];

/* ──────── Result tracking ──────── */
static int test_pass_count = 0;
static int test_fail_count = 0;

static void report(const char *name, int pass)
{
    if (pass) {
        printf("[PASS] %s\n", name);
        test_pass_count++;
    } else {
        printf("[FAIL] %s\n", name);
        test_fail_count++;
    }
}

static void print_hex(const char *label, const uint8_t *buf, int len)
{
    printf("%s: ", label);
    for (int i = 0; i < len; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

/* ──────── IOMUX configuration ──────── */
static void iomux_config(void)
{
    /* QSPI1 master output pins */
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_SCK_IOF_OVAL,
                          QSPI1_SCK_PAD_SEL, QSPI1_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_CS_0_IOF_OVAL,
                          QSPI1_CS_0_PAD_SEL, QSPI1_CS_0_HS_SEL, 0, 0);

    /* QSPI1 DQ0 (MOSI) - output */
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI1_DQ_0_IOF_OVAL,
                          QSPI1_DQ_0_PAD_SEL, QSPI1_DQ_0_HS_SEL, 0, 0);
    /* QSPI1 DQ1 (MISO) - input */
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI1_DQ_1_IOF_IVAL,
                          QSPI1_DQ_1_PAD_SEL, QSPI1_DQ_1_HS_SEL, 0, 0);

    /* QSPI2 slave input pins */
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_SCK_IOF_IVAL,
                          QSPI2_SCK_PAD_SEL, QSPI2_SCK_HS_SEL, 0, 0);
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_CS_0_IOF_IVAL,
                          QSPI2_CS_0_PAD_SEL, QSPI2_CS_0_HS_SEL, 0, 0);

    /* QSPI2 DQ0 (MOSI) - input */
    iomux_ls_iof_ival_cfg(IOMUX_BASE, QSPI2_DQ_0_IOF_IVAL,
                          QSPI2_DQ_0_PAD_SEL, QSPI2_DQ_0_HS_SEL, 0, 0);
    /* QSPI2 DQ1 (MISO) - output */
    iomux_ls_iof_oval_cfg(IOMUX_BASE, QSPI2_DQ_1_IOF_OVAL,
                          QSPI2_DQ_1_PAD_SEL, QSPI2_DQ_1_HS_SEL, 0, 0);
}

/* ──────── Peripheral init ──────── */
static void qspi_master_init(void)
{
    QSPI_InitTypeDef init = {0};
    QSPI_StructInit(&init);

    init.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    init.DataSize   = QSPI_FMT_LEN_8B;
    init.CPOL       = QSPI_SCKMODE_CPOL_LOW;
    init.CPHA       = QSPI_SCKMODE_CPHA_EDGE1;
    init.SCKDIV     = QSPI_SCKDIV_PRESCALER_8;
    init.Endian     = QSPI_FMT_ENDIAN_MSB;
    init.SSM        = QSPI_CR_SSM_HARD;
    init.DevMode    = QSPI_CR_MODE_MASTER;

    QSPI_Init(QSPI1, &init);
}

static void qspi_slave_init(void)
{
    QSPI_InitTypeDef init = {0};
    QSPI_StructInit(&init);

    init.ProtolMode = QSPI_FMT_PROTO_SINGLE;
    init.DataSize   = QSPI_FMT_LEN_8B;
    init.CPOL       = QSPI_SCKMODE_CPOL_LOW;
    init.CPHA       = QSPI_SCKMODE_CPHA_EDGE1;
    init.Endian     = QSPI_FMT_ENDIAN_MSB;
    init.SSM        = QSPI_CR_SSM_HARD;
    init.DevMode    = QSPI_CR_MODE_SLAVE;

    QSPI_Init(QSPI2, &init);
}

/* ──────── Clock and reset ──────── */
static void peripheral_clock_init(void)
{
#ifdef MISC_HAS_QSPI1_HAS_CLK
    qspi1_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI2_HAS_CLK
    qspi2_clk_en(ENABLE);
#endif
#ifdef MISC_HAS_QSPI1_CLK_DIV
    qspi1_clk_div(0);
#endif
#ifdef MISC_HAS_QSPI2_CLK_DIV
    qspi2_clk_div(0);
#endif
#ifdef MISC_HAS_QSPI1_RST
    qspi1_set_rst(DISABLE);
    qspi1_set_rst(ENABLE);
#endif
#ifdef MISC_HAS_QSPI2_RST
    qspi2_set_rst(DISABLE);
    qspi2_set_rst(ENABLE);
#endif
}

/* ──────── Test 1: Master TX → Slave RX ──────── */
static int test_master_to_slave(void)
{
    uint32_t timeout;

    printf("\n--- Test 1: Master TX -> Slave RX ---\n");

    /* Prepare test pattern */
    for (int i = 0; i < TEST_SIZE; i++) {
        master_tx[i] = 0xA0 + i;
    }
    memset(slave_rx, 0, TEST_SIZE);

    /* Re-init both sides */
    qspi_master_init();
    qspi_slave_init();

    /* Full-duplex exchange: master sends, slave receives */
    for (int i = 0; i < TEST_SIZE; i++) {
        /* Master: wait TX not full, then send */
        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {
            if (--timeout == 0) {
                printf("  ERROR: Master TX FIFO full timeout at byte %d\n", i);
                return 0;
            }
        }
        QSPI_SendData(QSPI1, master_tx[i]);

        /* Slave: wait RX not empty, then read */
        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {
            if (--timeout == 0) {
                printf("  ERROR: Slave RX empty timeout at byte %d\n", i);
                printf("  (Check wiring: PAD0->PAD21, PAD1->PAD22, PAD2->PAD23)\n");
                return 0;
            }
        }
        slave_rx[i] = (uint8_t)QSPI_ReceiveData(QSPI2);
    }

    /* Compare */
    if (0 == memcmp(master_tx, slave_rx, TEST_SIZE)) {
        return 1;
    }

    print_hex("  master_tx", master_tx, TEST_SIZE);
    print_hex("  slave_rx ", slave_rx, TEST_SIZE);
    return 0;
}

/* ──────── Test 2: Full-duplex bidirectional ──────── */
static int test_fullduplex(void)
{
    uint32_t timeout;

    printf("\n--- Test 2: Full-duplex bidirectional ---\n");

    /* Prepare different patterns for each direction */
    for (int i = 0; i < TEST_SIZE; i++) {
        master_tx[i] = 0x10 + i;       /* master -> slave */
        slave_tx[i]  = 0xE0 + i;       /* slave -> master */
    }
    memset(master_rx, 0, TEST_SIZE);
    memset(slave_rx, 0, TEST_SIZE);

    qspi_master_init();
    qspi_slave_init();

    /* Pre-load slave TX FIFO (slave must have data ready before master clocks) */
    for (int i = 0; i < QSPI_BUF_DEPTH && i < TEST_SIZE; i++) {
        QSPI_SendData(QSPI2, slave_tx[i]);
    }

    int slave_tx_idx = QSPI_BUF_DEPTH < TEST_SIZE ? QSPI_BUF_DEPTH : TEST_SIZE;

    for (int i = 0; i < TEST_SIZE; i++) {
        /* Master sends one byte (this generates the clock) */
        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {
            if (--timeout == 0) {
                printf("  ERROR: Master TX timeout at byte %d\n", i);
                return 0;
            }
        }
        QSPI_SendData(QSPI1, master_tx[i]);

        /* Master reads what slave sent back on MISO */
        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_RX_EMPTY)) {
            if (--timeout == 0) {
                printf("  ERROR: Master RX timeout at byte %d\n", i);
                printf("  (Check wiring: PAD24->PAD3 for MISO return path)\n");
                return 0;
            }
        }
        master_rx[i] = (uint8_t)QSPI_ReceiveData(QSPI1);

        /* Slave reads what master sent on MOSI */
        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {
            if (--timeout == 0) {
                printf("  ERROR: Slave RX timeout at byte %d\n", i);
                return 0;
            }
        }
        slave_rx[i] = (uint8_t)QSPI_ReceiveData(QSPI2);

        /* Refill slave TX FIFO if more data to send */
        if (slave_tx_idx < TEST_SIZE) {
            if (RESET == QSPI_GetFlag(QSPI2, QSPI_STATUS_TX_FULL)) {
                QSPI_SendData(QSPI2, slave_tx[slave_tx_idx++]);
            }
        }
    }

    /* Compare both directions */
    int m2s_ok = (0 == memcmp(master_tx, slave_rx, TEST_SIZE));
    int s2m_ok = (0 == memcmp(slave_tx, master_rx, TEST_SIZE));

    if (!m2s_ok) {
        printf("  Master->Slave data mismatch:\n");
        print_hex("    master_tx", master_tx, TEST_SIZE);
        print_hex("    slave_rx ", slave_rx, TEST_SIZE);
    }
    if (!s2m_ok) {
        printf("  Slave->Master data mismatch:\n");
        print_hex("    slave_tx ", slave_tx, TEST_SIZE);
        print_hex("    master_rx", master_rx, TEST_SIZE);
    }

    return m2s_ok && s2m_ok;
}

/* ──────── Test 3: Walking-ones pattern ──────── */
static int test_walking_ones(void)
{
    uint32_t timeout;

    printf("\n--- Test 3: Walking-ones data integrity ---\n");

    qspi_master_init();
    qspi_slave_init();

    /* Test each bit position: 0x01, 0x02, 0x04, ..., 0x80 */
    for (int bit = 0; bit < 8; bit++) {
        uint8_t pattern = (1U << bit);

        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {
            if (--timeout == 0) {
                printf("  ERROR: TX timeout at bit %d\n", bit);
                return 0;
            }
        }
        QSPI_SendData(QSPI1, pattern);

        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {
            if (--timeout == 0) {
                printf("  ERROR: RX timeout at bit %d (pattern=0x%02X)\n", bit, pattern);
                return 0;
            }
        }
        uint8_t received = (uint8_t)QSPI_ReceiveData(QSPI2);

        if (received != pattern) {
            printf("  Bit %d: sent 0x%02X, got 0x%02X\n", bit, pattern, received);
            return 0;
        }
    }

    /* Also test 0x00 and 0xFF */
    uint8_t edge_cases[] = {0x00, 0xFF, 0x55, 0xAA};
    for (int i = 0; i < 4; i++) {
        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {
            if (--timeout == 0) return 0;
        }
        QSPI_SendData(QSPI1, edge_cases[i]);

        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {
            if (--timeout == 0) return 0;
        }
        uint8_t received = (uint8_t)QSPI_ReceiveData(QSPI2);

        if (received != edge_cases[i]) {
            printf("  Edge case: sent 0x%02X, got 0x%02X\n", edge_cases[i], received);
            return 0;
        }
    }

    return 1;
}

/* ──────── Test 4: Clock polarity / phase combinations ──────── */
static int test_cpol_cpha(void)
{
    uint32_t timeout;
    uint8_t test_byte = 0x5A;
    uint8_t received;

    printf("\n--- Test 4: CPOL/CPHA mode combinations ---\n");

    uint32_t cpol_vals[] = {QSPI_SCKMODE_CPOL_LOW,  QSPI_SCKMODE_CPOL_LOW,
                            QSPI_SCKMODE_CPOL_HIGH, QSPI_SCKMODE_CPOL_HIGH};
    uint32_t cpha_vals[] = {QSPI_SCKMODE_CPHA_EDGE1, QSPI_SCKMODE_CPHA_EDGE2,
                            QSPI_SCKMODE_CPHA_EDGE1, QSPI_SCKMODE_CPHA_EDGE2};
    const char *mode_names[] = {"Mode 0 (CPOL=0,CPHA=0)",
                                "Mode 1 (CPOL=0,CPHA=1)",
                                "Mode 2 (CPOL=1,CPHA=0)",
                                "Mode 3 (CPOL=1,CPHA=1)"};
    int all_ok = 1;

    for (int m = 0; m < 4; m++) {
        /* Reinit both sides with same CPOL/CPHA */
        QSPI_InitTypeDef init = {0};
        QSPI_StructInit(&init);
        init.ProtolMode = QSPI_FMT_PROTO_SINGLE;
        init.DataSize   = QSPI_FMT_LEN_8B;
        init.CPOL       = cpol_vals[m];
        init.CPHA       = cpha_vals[m];
        init.SCKDIV     = QSPI_SCKDIV_PRESCALER_8;
        init.Endian     = QSPI_FMT_ENDIAN_MSB;
        init.SSM        = QSPI_CR_SSM_HARD;

        init.DevMode = QSPI_CR_MODE_MASTER;
        QSPI_Init(QSPI1, &init);

        init.DevMode = QSPI_CR_MODE_SLAVE;
        QSPI_Init(QSPI2, &init);

        /* Send one byte */
        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI1, QSPI_STATUS_TX_FULL)) {
            if (--timeout == 0) {
                printf("  %s: TX timeout\n", mode_names[m]);
                all_ok = 0;
                goto next_mode;
            }
        }
        QSPI_SendData(QSPI1, test_byte);

        timeout = TIMEOUT_COUNT;
        while (SET == QSPI_GetFlag(QSPI2, QSPI_STATUS_RX_EMPTY)) {
            if (--timeout == 0) {
                printf("  %s: RX timeout\n", mode_names[m]);
                all_ok = 0;
                goto next_mode;
            }
        }
        received = (uint8_t)QSPI_ReceiveData(QSPI2);

        if (received == test_byte) {
            printf("  %s: OK\n", mode_names[m]);
        } else {
            printf("  %s: FAIL (sent 0x%02X, got 0x%02X)\n",
                   mode_names[m], test_byte, received);
            all_ok = 0;
        }
next_mode:
        ;
    }

    return all_ok;
}

/* ──────── Main ──────── */
void main(void)
{
    printf("\n========================================\n");
    printf("  SPI External Loopback Test\n");
    printf("  QSPI1 (Master, PAD0-3) <-> QSPI2 (Slave, PAD21-24)\n");
    printf("========================================\n\n");

    peripheral_clock_init();
    iomux_config();

    printf("Wiring check:\n");
    printf("  PAD0  (QSPI1 SCK)  -> PAD21 (QSPI2 SCK)\n");
    printf("  PAD1  (QSPI1 CS0)  -> PAD22 (QSPI2 CS0)\n");
    printf("  PAD2  (QSPI1 DQ0)  -> PAD23 (QSPI2 DQ0)  [MOSI]\n");
    printf("  PAD3  (QSPI1 DQ1)  <- PAD24 (QSPI2 DQ1)  [MISO]\n");
    printf("\n");

    /* Run test suite */
    report("Master TX -> Slave RX",      test_master_to_slave());
    report("Full-duplex bidirectional",   test_fullduplex());
    report("Walking-ones data integrity", test_walking_ones());
    report("CPOL/CPHA mode combinations", test_cpol_cpha());

    /* Summary */
    printf("\n========================================\n");
    printf("  Results: %d passed, %d failed\n", test_pass_count, test_fail_count);
    printf("========================================\n");

    if (test_fail_count == 0) {
        printf("ALL TESTS PASSED\n");
        simulation_pass();
    } else {
        printf("SOME TESTS FAILED\n");
        simulation_fail();
    }

    while (1) {}
}
