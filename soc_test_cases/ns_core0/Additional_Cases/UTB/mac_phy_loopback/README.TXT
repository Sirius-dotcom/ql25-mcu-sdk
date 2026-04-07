/*!
    \file  README.TXT
    \brief UTB MAC PHY internal loopback test
    \version 2026-4-7, v1.0.0
*/

Function description:
    This demo verifies the UTB MAC/PHY loopback path by chaining three checks:
    1. QSPI1 CPU TX + RX DMA self-loopback to build a known-good reference packet.
    2. UDMA M2M writes that packet to mac_tx_symb at 0x18000100, then reads back
       from mac_rx_symb at 0x18000104 after MAC+PHY internal loopback.
    3. QSPI1 TX DMA + RX DMA self-loopback sends the UTB receive packet again and
       confirms the final packet is still identical.

UTB register setting:
    MAC base = 0x18000000
    PHY base = 0x18004000
    MAC/PHY configuration values are written directly according to the register
    table provided for this test case.
    PHY_Tx_Mem at PHY base + 0x800 is filled as 64 symbols x 8 groups:
      symbol 0..7   = 1,2,0,0,0,0,0,0
      symbol 8      = 1
      symbol 9..61  = 3
      symbol 62..63 = 0

Important assumption:
    mac_tx_symb and mac_rx_symb are treated as 8-bit streaming MMIO ports.
    The M2M DMA writes/reads one byte per beat against fixed addresses
    0x18000100 and 0x18000104.

Expected packet format:
    Byte0 = 0x55
    Byte1 = 0xAA
    Byte2 = PKT_LEN low byte
    Byte3 = PKT_LEN high byte
    Byte4..Byte(PKT_LEN-1) = (index ^ 0x5A) & 0xFF
    PKT_LEN = 472 bytes

Test result:
    PASS requires:
      1. SPI CPU self-loopback compare pass
      2. UTB MAC/PHY loopback compare pass
      3. SPI DMA self-loopback compare pass

IO Connection information:
    EDA testbench shall apply:
      +TEST_LOOPBACK=qspi1_autotest_connect
    This means:
      QSPI1DQ0 (MOSI) is looped back to QSPI1DQ1 (MISO)

Observe points:
    Check console log:
      [UTB_MAC_PHY_LOOPBACK] spi cpu loopback pass
      [UTB_MAC_PHY_LOOPBACK] utb mac/phy loopback pass
      [UTB_MAC_PHY_LOOPBACK] spi dma loopback pass
      [UTB_MAC_PHY_LOOPBACK] PASS
