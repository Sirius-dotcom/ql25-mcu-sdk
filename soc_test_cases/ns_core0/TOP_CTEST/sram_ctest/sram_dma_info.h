
        /*
        NOTE: this file is automacally generated from csv file.
        */
            
#ifndef SRAM_DMA_INFO_H

#define SRAM_DMA_INFO_H

#include <stdio.h>
#define READ_ONLY_NUM 1
#define ADDR_SIZE 32
#define UDMA_NUM 1
#define UDMA_M2M0_IRQn UDMA0_IRQn 
#define UDMA_M2M0_CH0_M2M_IRQ UDMA0_CH0_M2M_IRQ 
#define UDMA_M2M0_IRQHandler UDMA0_IRQHandler 
addr_xlen udma_base[1]={0x10020000};

#define SRAM_NUM 13

addr_xlen sram_base[13]={0x13000000, 0x13004000, 0x13008000, 0x18000000, 0x18004000, 0x18008000, 0x18010000, 0x30000000, 0x30020000, 0x30040000, 0x30060000, 0x40000000, 0x40002000};
#ifdef CFG_SIMULATION
addr_xlen sram_size[13]={0x3fff, 0x3fff, 0x3fff, 0x3fff, 0x3fff, 0x3fff, 0x3fff, 0x1ffff, 0x1ffff, 0xffff, 0xffff, 0x1fff, 0x1fff};
#else
addr_xlen sram_size[13]={0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7fff, 0x7fff, 0x3fff, 0x3fff, 0x1fff, 0x1fff};
#endif
#define READ_ONLY_NUM 1

addr_xlen read_only_start[1]={0x12000000};
addr_xlen read_only_end[1]={0x12000FFF};
#define SLV_PORT_NUM 1

addr_xlen slv_port_start[1]={0x08000000};
addr_xlen slv_port_end[1]={0x0801FFFF};
#define WRITE_ONLY_NUM 13

addr_xlen write_only_start[13]={0x13000000, 0x13004000, 0x13008000, 0x18000000, 0x18004000, 0x18008000, 0x18010000, 0x30000000, 0x30020000, 0x30040000, 0x30060000, 0x40000000, 0x40002000};
#ifdef CFG_SIMULATION
addr_xlen write_only_end[13]={0x13003fff, 0x13007fff, 0x1300bfff, 0x18003fff, 0x18007fff, 0x1800bfff, 0x18013fff, 0x3001ffff, 0x3003ffff, 0x3004ffff, 0x3006ffff, 0x40001fff, 0x40003fff};
#else
addr_xlen write_only_end[13]={0x130000ff, 0x130040ff, 0x130080ff, 0x180000ff, 0x180040ff, 0x180080ff, 0x180100ff, 0x30007fff, 0x30027fff, 0x30043fff, 0x30063fff, 0x40001fff, 0x40003fff};
#endif
#define DO_NOTHING_NUM 2

addr_xlen do_nothing_start[2]={0x10060000, 0x20000000};
addr_xlen do_nothing_end[2]={0x10060FFF, 0x2FFFFFFF};
#endif
