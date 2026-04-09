/*
 * Copyright (c) 2025 QuickLink Ltd.
 *
 * Author: zhaowei <zhaowei@xlianzn.com>
 *
 * UTB Driver struct definition
 */
 
#ifndef QL25_MAC_DRIVER_H
#define QL25_MAC_DRIVER_H

typedef struct {
    uint16_t available_slices;
    uint16_t committed_slices;
    uint16_t unwriten_bytes;
    uint16_t writen_bytes;
} mac_spi_fifo_state_t;

typedef enum {
    IRQID_MAC_TX_FIFO_NOT_FULL,
    IRQID_MAC_RX_FIFO_NOT_EMPTY,
    IRQID_MCU2MAC_LEN_ERR,
    IRQID_PHY2MAC_LEN_ERR,
    IRQID_MAC_TX_FIFO_FULL_WRITE_ERR,
    IRQID_MAC2PHY_LEN_ERR,
    IRQID_MAC_RX_FIFO_FULL_WRITE_ERR,
    IRQID_MAC_TX_FSM_TIMEOUT,
    IRQID_MAC_RX_FSM_TIMEOUT,
    IRQID_PHY_RX_FSM_TIMEOUT,
    IRQID_MAX
} mac_irq_type_t;

enum mac_cnt_type {
    MAC_CRC_0_CNT,                    /* mac统计crc为0的报文个数复位 */
    MAC_CRC_1_CNT,                    /* mac统计crc为1的报文个数复位 */
    MCU_LEN_WRONG_CNT,                /* mac统计MCU发送的长度不对的报文个数复位 */
    PHY_LEN_WRONG_CNT,                /* mac统计phy发来的长度不对的报文个数复位 */
    MAC_TX_FULL_WRITE_WRONG_CNT,      /* mac统计TX FIFO满写的字节个数复位 */
    MAC_SPI_STATUS,                   /* SPI统计状态寄存器mac_spi_status复位 */
    MAC_RX_FULL_WRITE_WRONG_CNT,      /* mac统计RX FIFO满写的字节个数复位 */
    MAC_TX_TIMEOUT_WRONG_CNT,         /* mac tx状态机超时的报文个数复位 */
    MAC_STATUS_TIMEOUT_WRONG_CNT,     /* 状态机超时的报文个数复位 */
    MAC_ALL_SLICES_FROM_PHY_CNT,      /* 从PHY收到的所有切片个数复位 */
    MAC_EMPTY_SLICES_FROM_PHY_CNT,    /* 从PHY收到的空切片个数复位，空切片判断：前8个字节都是0且总长度大于8B且crc校验成功的切片 */
    MAC_VALID_SLICES_FROM_MCU_CNT,    /* 从rx fifo读出来发送到MCU的切片个数复位 */
    MAC_EMPTY_SLICES_TO_MCU_CNT,      /* 因rx fifo空发送到MCU的空数据片个数复位 */
    MAC_TOTAL_SLICE_TO_PHY_CNT,       /* mac统计发送到PHY的总共数据报文个数复位 */
    MAC_VALID_SLICE_TO_PHY_CNT,       /* mac统计发送到PHY的有效数据报文个数复位 */
    MAC_EMPTY_SLICE_TO_PHY_CNT,       /* mac统计发送到PHY的空数据报文个数复位 */
    MAC_MCU_VALID_SLICE_CNT,          /* mac统计MCU发送的Header.valid=1的报文个数复位 */
    MAC_MCU_INVALID_SLICE_CNT,        /* mac统计MCU发送的Header.valid=0的报文个数复位 */
    MAC_MCU_LENGTH_RIGHT_CNT,         /* mac统计MCU发送的长度正确的报文个数复位 */
    MAC_ALL_CNT,
};

typedef struct {
    uint32_t phy_len_wrong_num;         /* mac统计phy发来的长度不对的报文个数 */
    uint32_t tx_full_write_wrong_num;   /* mac统计TX FIFO满写的字节个数 */
    uint32_t rx_full_write_wrong_num;   /* mac统计RX FIFO满写的字节个数 */
    uint32_t status_timeout_wrong_num;  /* 状态机超时的报文个数 */
} mac_drop_cnt_t;

typedef struct {
    uint32_t empty_slices_from_phy_num;  /* 从PHY收到的空切片个数，空切片判断：前8个字节都是0且总长度大于8B且crc校验成功的切片 */   
    uint32_t all_slices_from_phy_num;    /* 从PHY收到的所有切片个数 */ 
} mac_slices_from_phy_cnt_t;

typedef struct {
    uint32_t total_slice_num_2phy;       /* mac统计发送到PHY的总共数据报文个数 */ 
    uint32_t empty_slice_num_2phy;       /* mac统计发送到PHY的空数据报文个数 */ 
    uint32_t valid_slice_num_2phy;       /* mac统计发送到PHY的有效数据报文个数 */ 
} mac_slices_to_phy_cnt_t;

typedef struct {
    uint32_t empty_slices_to_mcu_num;    /* fifo空发送到MCU的空数据片个数 */ 
    uint32_t valid_slices_to_mcu_num;    /* 从rx fifo读出来发送到MCU的切片个数 */ 
} mac_slices_to_mcu_cnt_t;

typedef struct {
    uint32_t mcu_invld_slice_num;        /* mac统计MCU发送的Header.valid=0的报文个数 */ 
    uint32_t mcu_len_right_num;          /* mac统计MCU发送的长度正确的报文个数 */ 
    uint32_t mcu_len_wrong_num;          /* mac统计MCU发送的长度不对的报文个数 */ 
    uint32_t mcu_vld_slice_num;          /* mac统计MCU发送的Header.valid=1的报文个数 */ 
} mac_slices_from_mcu_cnt_t;

int ql25_mac_set_fixed_length(utb_ql25_ctrl_t *ql25_ctrl, uint32_t frag_size);
int ql25_mac_init(utb_ql25_ctrl_t *ql25_ctrl);
void ql25_mac_set_all_int_en(utb_ql25_ctrl_t *ql25_ctrl, uint32_t en);
void ql25_mac_get_all_int(utb_ql25_ctrl_t *ql25_ctrl, uint8_t *int_sts);
int ql25_mac_init_selftest(utb_ql25_ctrl_t *ql25_ctrl);
int ql25_mac_clr_cnt(utb_ql25_ctrl_t *ql25_ctrl);
void ql25_mac_clr_all_int(utb_ql25_ctrl_t *ql25_ctrl);

#endif //QL25_MAC_DRIVER_H