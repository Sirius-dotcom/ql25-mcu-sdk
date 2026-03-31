#include <stdio.h>
#include "nuclei_sdk_hal.h"

uint8_t state0 = 1;
uint8_t state1 = 1;
uint8_t state2 = 1;

#define TEST_MEM_SIZE          0x100
#ifdef SRAM2_MEM
#define TEST_MEM_BASE          (SRAM2_MEM_BASE)
#else
#ifdef SRAM0_MEM_BASE
#define TEST_MEM_BASE          (SRAM0_MEM_BASE)
#else
#ifdef TEST_SRAM_MEM_BASE
#define TEST_MEM_BASE          (TEST_SRAM_MEM_BASE)
#endif
#endif
#endif

/**
  * \brief    set/clear bit one by one in word, and then read out to verify
  * \param    test_addr: the read/write address
  */
void bit_set_clear_in_word_test(uint32_t test_addr)
{
    uint32_t i, wdat, rdat;

    for (i = 0; i < 32; i++) {
        wdat = 1 << i;
        REG32(test_addr) = wdat;
        rdat = REG32(test_addr);    
        if (wdat != rdat) {    
            printf("1<< ERR A: %08x, w: %08x, r %08x\r\n", test_addr, wdat, rdat);
            state1 = 0;
            break;
        }
    }

    for (i = 0; i < 32; i++) {
        wdat = ~(1 << i);
        REG32(test_addr) = wdat;
        rdat = REG32(test_addr);
        if (wdat != rdat) {         
            printf("0<< ERR A: %08x, w: %08x, r %08x\r\n", test_addr, wdat, rdat);
            state1 = 0;
            break;
        }
    }
}

/**
  * \brief    set/clear bit one by one in half word, and then read out to verify
  * \param    test_addr: the read/write address
  */
void bit_set_clear_in_hword_test(uint32_t test_addr)
{
    uint16_t i, wdat, rdat;

    for (i = 0; i < 16; i++) {
        wdat = 1 << i;
        REG16(test_addr) = wdat;
        rdat = REG16(test_addr);    
        if (wdat != rdat) {    
            printf("1<< ERR A: %08x, w: %04x, r %04x\r\n", test_addr, wdat, rdat);
            state1 = 0;
            break;
        }
    }

    for (i = 0; i < 16; i++) {
        wdat = ~(1 << i);
        REG16(test_addr) = wdat;
        rdat = REG16(test_addr);
        if (wdat != rdat) {         
            printf("0<< ERR A: %08x, w: %04x, r %04x\r\n", test_addr, wdat, rdat);
            state1 = 0;
            break;
        }
    }
}

/**
  * \brief    set/clear bit one by one in byte, and then read out to verify
  * \param    test_addr: the read/write address
  */
void bit_set_clear_in_byte_test(uint32_t test_addr)
{
    uint8_t i, wdat, rdat;

    for (i = 0; i < 8; i++) {
        wdat = 1 << i;
        REG8(test_addr) = wdat;
        rdat = REG8(test_addr);    
        if (wdat != rdat) {    
            printf("1<< ERR A: %08x, w: %02x, r %02x\r\n", test_addr, wdat, rdat);
            state1 = 0;
            break;
        }
    }

    for (i = 0; i < 16; i++) {
        wdat = ~(1 << i);
        REG8(test_addr) = wdat;
        rdat = REG8(test_addr);
        if (wdat != rdat) {         
            printf("0<< ERR A: %08x, w: %02x, r %02x\r\n", test_addr, wdat, rdat);
            state1 = 0;
            break;
        }
    }
}

/**
  * \brief    write in word and then read out continully to verify
  *
  * \param    addr: the read/write start address
  * \param    length: the length of the continue address
  */
void ContinueWriteWordFastToReadCompareSRAM(uint32_t addr, uint32_t length)
{
    uint32_t i;
    uint32_t val;
    uint32_t w_dat = 0;

    for (i = 0; i < length; i += 4) {
        
        w_dat = (i << 24) + (i << 16) + (i << 8) + i;
        REG32(addr + i) = w_dat;
        val = REG32(addr + i);
        if(w_dat != val) {
            printf("Fast RW addr %08x match err, write: %08x, read : %08x\r\n", addr + i, w_dat, val);
            state0 = 0;
            break;
        }
    }
}

/**
  * \brief    write in half-word and then read out continully to verify
  *
  * \param    addr: the read/write start address
  * \param    length: the length of the continue address
  */
void ContinueWriteHwordFastToReadCompareSRAM(uint32_t addr, uint32_t length)
{
    uint32_t i = 0;
    uint16_t val = 0;
    uint16_t w_dat = 0;
    uint16_t default_val = 0;

    for (i = 0; i < length; i += 2) {
        w_dat = i + 0x3 + (uint16_t)((i + 0x3) << 8);
        default_val = REG16(addr + i);
        
        REG16(addr + i) = w_dat;
        val = REG16(addr + i);
        if(w_dat != val) {
            printf("Fast RW addr %08x match err, default_val: %04x, write: %04x, read : %04x\r\n", addr + i, default_val, w_dat, val);
            state0 = 0;
            break;
        }
    }
}

/**
  * \brief    write in byte and then read out continully to verify
  *
  * \param    addr: the read/write start address
  * \param    length: the length of the continue address
  */
void ContinueWriteByteFastToReadCompareSRAM(uint32_t addr, uint32_t length)
{
    uint32_t i;
    uint8_t val;
    uint8_t w_dat = 0;
    uint8_t default_val = 0;

    for (i = 0; i < length; i += 1) {
        w_dat = i + 0x5;
        default_val = REG8(addr + i);
        REG8(addr + i) = w_dat;
        val = REG8(addr + i);
        if(w_dat != val) {
            printf("Fast RW addr %08x match err, default_val: %02x, write: %02x, read : %02x\r\n", addr + i, default_val, w_dat, val);
            state0 = 0;
            break;
        }
    }
}

/**
  * \brief      Write byte data continuously to SRAM
  * \param[in]  val: the data writed to SRAM
  * \param[in]  addr: write the starting address of SRAM
  * \param[in]  length: the length of the addrrss range
  */
void WriteByteDiffDataToSRAM(uint8_t val, uint32_t addr, uint16_t length)
{
    uint16_t i;
    for (i = 0;i < length; i += 1){
        REG8(addr + i) = val + i;
    }
}

/**
  * \brief      read out byte data continuously from SRAM to compare
  * \param[in]  startAddr: read the starting address of SRAM
  * \param[in]  length: the length of the addrrss range
  * \param[in]  dat: the data to be compared
  */
void ReadByteDiffDataFromSRAMToCompare(uint32_t startAddr, uint32_t length, uint8_t dat)
{
    uint32_t i;

    for (i = 0; i < length; i += 1) {
        if ((dat + i) != REG8(startAddr + i)) {
            printf("WThenR err A: %08x, r: %02x \r\n", startAddr + i, REG8(startAddr + i));
            state2 = 0;
            break;
        }
    }
}

/**
  * \brief      Write half-word data continuously to SRAM
  * \param[in]  val: the data writed to SRAM
  * \param[in]  addr: write the starting address of SRAM
  * \param[in]  length: the length of the addrrss range
  */
void WriteHwordDiffDataToSRAM(uint16_t val, uint32_t addr, uint16_t length)
{
    uint16_t i;
    for (i = 0;i < length; i += 2){
        REG16(addr + i) = val + i;
    }
}

/**
  * \brief      read out half-word data continuously from SRAM to compare
  * \param[in]  startAddr: read the starting address of SRAM
  * \param[in]  length: the length of the addrrss range
  * \param[in]  dat: the data to be compared
  */
void ReadHwordDiffDataFromSRAMToCompare(uint32_t startAddr, uint32_t length, uint16_t dat)
{
    uint32_t i;

    for (i = 0; i < length; i += 2) {
        if ((dat + i) != REG16(startAddr + i)) {
            printf("WThenR err A: %08x, r: %04x \r\n", startAddr + i, REG16(startAddr + i));
            state2 = 0;
            break;
        }
    }
}

/**
  * \brief      Write word data continuously to SRAM
  * \param[in]  val: the data writed to SRAM
  * \param[in]  addr: write the starting address of SRAM
  * \param[in]  length: the length of the addrrss range
  */
void WriteWordDiffDataToSRAM(uint32_t val, uint32_t addr, uint16_t length)
{
    uint16_t i;
    for (i = 0;i < length; i += 4){
        REG32(addr + i) = val + i;
    }
}

/**
  * \brief      read out word data continuously from SRAM to compare
  * \param[in]  startAddr: read the starting address of SRAM
  * \param[in]  length: the length of the addrrss range
  * \param[in]  dat: the data to be compared
  */
void ReadWordDiffDataFromSRAMToCompare(uint32_t startAddr, uint32_t length, uint32_t dat)
{
    uint32_t i;

    for (i = 0; i < length; i += 4) {
        if ((dat + i) != REG32(startAddr + i)) {
            printf("WThenR err A: %08x, r: %08x \r\n", startAddr + i, REG32(startAddr + i));
            state2 = 0;
            break;
        }
    }
}

void main(void)
{
#ifdef TEST_MEM_BASE
    /* write & fast read to compare continuelly in word, half word and byte */
    ContinueWriteWordFastToReadCompareSRAM(TEST_MEM_BASE, TEST_MEM_SIZE); 
    ContinueWriteHwordFastToReadCompareSRAM(TEST_MEM_BASE + TEST_MEM_SIZE/4, 0x10); 
    ContinueWriteByteFastToReadCompareSRAM(TEST_MEM_BASE + TEST_MEM_SIZE/2, 0x10);

    /* set/clear bit one by one step by word, half word and byte, and then read out to verify */ 
    bit_set_clear_in_word_test(TEST_MEM_BASE); 
    bit_set_clear_in_hword_test(TEST_MEM_BASE); 
    bit_set_clear_in_byte_test(TEST_MEM_BASE);
    /* align test */
    bit_set_clear_in_word_test(TEST_MEM_BASE + 1);
    bit_set_clear_in_word_test(TEST_MEM_BASE + 2);
    bit_set_clear_in_word_test(TEST_MEM_BASE + 3);
    bit_set_clear_in_hword_test(TEST_MEM_BASE + 1); 
    bit_set_clear_in_hword_test(TEST_MEM_BASE + 3);

    /* write different word data continuously */
    WriteWordDiffDataToSRAM(0xf0f0f0f0, TEST_MEM_BASE, 0x10);
    WriteWordDiffDataToSRAM(0xc0c3c0c3, TEST_MEM_BASE + TEST_MEM_SIZE/4, 0x10);
    WriteWordDiffDataToSRAM(0x3c3c3c3c, TEST_MEM_BASE + TEST_MEM_SIZE/2, 0x10);
    /* read out continuously to compare */
    ReadWordDiffDataFromSRAMToCompare(TEST_MEM_BASE, 0x10, 0xf0f0f0f0);
    ReadWordDiffDataFromSRAMToCompare(TEST_MEM_BASE + TEST_MEM_SIZE/4, 0x10, 0xc0c3c0c3);
    ReadWordDiffDataFromSRAMToCompare(TEST_MEM_BASE + TEST_MEM_SIZE/2, 0x10, 0x3c3c3c3c);

    /* byte write one region then read to compare test */
    WriteByteDiffDataToSRAM(0xd0, TEST_MEM_BASE, 0x10);
    ReadByteDiffDataFromSRAMToCompare(TEST_MEM_BASE, 0x10, 0xd0);

    /* half-word write one region then read to compare test */
    WriteHwordDiffDataToSRAM(0xd0d0, TEST_MEM_BASE, 0x10);
    ReadHwordDiffDataFromSRAMToCompare(TEST_MEM_BASE, 0x10, 0xd0d0);

    if (state0 && state1 && state2) {
        simulation_pass();
    } else {
        simulation_fail();
    }
#else
    simulation_fail();
#endif

    while (1) {}
}
