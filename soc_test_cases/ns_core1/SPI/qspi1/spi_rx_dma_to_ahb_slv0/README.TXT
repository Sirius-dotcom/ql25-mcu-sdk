/*!
    \file  README.TXT
    \brief SPI rx dma to ahb_slv0_ratio_mem test
    \version 2026-4-1, v1.0.0
*/

Function description:
    This demo is used to test external SPI packet receive in slave mode.
    External EDA SPI master sends a PKT_LEN-byte packet to QSPI1.
    QSPI1 receives the packet by RX DMA into SRAM first.
    Then core0 copies the SRAM buffer to ahb_slv0_ratio_mem at 0x18000000.
    Finally, the demo compares both SRAM data and ahb_slv0_ratio_mem readback data
    against the expected packet pattern.

Expected packet format:
    Byte0 = 0x55
    Byte1 = 0xAA
    Byte2 = PKT_LEN low byte
    Byte3 = PKT_LEN high byte
    Byte4..Byte(PKT_LEN-1) = (index ^ 0x5A) & 0xFF

Test result:
    The test results are printed out via USART PASS/FAIL/TODO.
    PASS requires:
      1. RX DMA completion
      2. SRAM receive buffer compare pass
      3. ahb_slv0_ratio_mem readback compare pass

IO Connection information:
    Connect EDA SPI master SCK   to QSPI1SCK .
    Connect EDA SPI master CS    to QSPI1CS  .
    Connect EDA SPI master MOSI  to QSPI1DQ0 .
    Connect EDA SPI master MISO  to QSPI1DQ1 if needed by the testbench.

Observe points:
    Check console log:
      [SPI_RX_DMA_AHB] dma done
      [SPI_RX_DMA_AHB] sram compare pass
      [SPI_RX_DMA_AHB] ahb compare pass
    Check memory window:
      0x18000000 ~ 0x18000000 + PKT_LEN - 1
