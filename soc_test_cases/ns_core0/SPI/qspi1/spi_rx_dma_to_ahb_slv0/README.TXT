/*!
    \file  README.TXT
    \brief SPI rx dma to ahb_slv0_ratio_mem test
    \version 2026-4-1, v1.0.0
*/

Function description:
    This demo is used to test QSPI1 single-port self transmit and loopback receive.
    QSPI1 works in single-line master mode and sends a PKT_LEN-byte packet by CPU.
    EDA testbench loops QSPI1DQ0 back to QSPI1DQ1 by +TEST_LOOPBACK=qspi1_autotest_connect.
    QSPI1 receives the looped-back packet by RX DMA into SRAM first.
    Then core0 copies the SRAM buffer to ahb_slv0_ratio_mem at 0x18000000.
    Finally, the demo compares both SRAM data and ahb_slv0_ratio_mem readback data
    against the locally generated packet pattern.

Expected packet format:
    Byte0 = 0x55
    Byte1 = 0xAA
    Byte2 = PKT_LEN low byte
    Byte3 = PKT_LEN high byte
    Byte4..Byte(PKT_LEN-1) = (index ^ 0x5A) & 0xFF

Test result:
    The test results are printed out via USART PASS/FAIL/TODO.
    PASS requires:
      1. CPU TX completion
      2. RX DMA completion
      3. SRAM receive buffer compare pass
      4. ahb_slv0_ratio_mem readback compare pass

IO Connection information:
    EDA testbench shall apply:
      +TEST_LOOPBACK=qspi1_autotest_connect
    This means:
      QSPI1DQ0 (MOSI) is looped back to QSPI1DQ1 (MISO)
    No external SPI master packet driver is needed for this case.

Observe points:
    Check console log:
      [SPI_RX_DMA_AHB] tx done
      [SPI_RX_DMA_AHB] rx dma done
      [SPI_RX_DMA_AHB] sram compare pass
      [SPI_RX_DMA_AHB] ahb compare pass
    Check memory window:
      0x18000000 ~ 0x18000000 + PKT_LEN - 1
