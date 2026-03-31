/*
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

#include "eeprom.h"

#define EEPROM_ADDR     EEPROM_BLOCK0_ADDRESS

volatile uint8_t tx_dma_done_flag = 0;
volatile uint8_t rx_dma_done_flag = 0;

/**
  * \brief  write one byte to the I2C EEPROM
  * \param p_buffer pointer to the buffer containing the data to be written to the EEPROM
  * \param write_address EEPROM's internal address to write to
  */
void Eeprom_ByteWrite(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t write_address)
{
    uint32_t timeout = 0xFFFFFFFF;
    I2C_ACK(i2cx, DISABLE);

    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, timeout);
    /* wait until I2C recv ack */
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx, I2C_STATUS_ADDR_MATCH);
    I2C_Write_Done(i2cx, write_address, timeout);
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}

    I2C_Write(i2cx, *p_buffer, timeout);
    /* wait until I2C recv ack */
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}
    I2C_Stop(i2cx);
    I2C_WaitStopOver(i2cx);
}

/**
  * \brief write more than one byte to the EEPROM with a single write cycle
  * \param p_buffer pointer to the buffer containing the data to be written to the EEPROM
  * \param write_address EEPROM's internal address to write to
  * \param number_of_byte number of bytes to write to the EEPROM
  */
void Eeprom_PageWrite(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t write_address, uint8_t number_of_byte)
{
    uint32_t timeout = 0xFFFFFFFF;
    I2C_ACK(i2cx, DISABLE);

    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, timeout);
    /* wait until I2C recv ack */
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx, I2C_STATUS_ADDR_MATCH);

    I2C_Write_Done(i2cx, write_address, timeout);
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}

    /* set tx data num for enhance i2c efficiency(continuous data transmission) */
    I2C_Set_TxSize(i2cx, number_of_byte);
    while (number_of_byte--) {
        /* send a data byte */
        I2C_Write(i2cx, *p_buffer, timeout);
        /* wait until I2C recv ack */
        while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}
        /* point to the next byte to be written */
        p_buffer++;
    }

    /* wait unit enf of transmit */
    while(!I2C_Get_Status(i2cx, I2C_STATUS_EOT)) {}
    /* send stop cmd */
    I2C_Stop(i2cx);
    I2C_Clear_Status(i2cx, I2C_STATUS_EOT);
    I2C_WaitStopOver(i2cx);
}

/**
  * \brief read data from the EEPROM
  * \param p_buffer pointer to the buffer that receives the data read from the EEPROM
  * \param read_address EEPROM's internal address to start reading from
  * \param number_of_byte number of bytes to reads from the EEPROM
  */
void Eeprom_BufferRead(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t read_address, uint16_t number_of_byte)
{
    uint32_t timeout = 0xFFFFFFFF;
    I2C_ACK(i2cx, DISABLE);

    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, timeout);
    /* wait until I2C recv ack */
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx, I2C_STATUS_ADDR_MATCH);

    I2C_Write_Done(i2cx, read_address, timeout);
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}

    I2C_Restart_Cmd();
    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_READ, timeout);
    /* wait until I2C recv ack */
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx, I2C_STATUS_ADDR_MATCH);

    I2C_ACK(i2cx, ENABLE);
    I2C_Read_En(i2cx);
    while (number_of_byte) {
        /* send nack and stop to slave after last byte */
        if(1 == number_of_byte) {
            /* wait for controller exit from ack stage*/
            while(I2C_CHECK_MST_STATE(i2cx, I2C_STATUS_MASTER_STATE_ACK));
            I2C_ACK(i2cx, DISABLE);
            I2C_Stop(i2cx);
        }
        I2C_Read(i2cx, p_buffer, timeout);
        /* point to the next location where the byte read will be saved */
        p_buffer++;
        /* decrement the read bytes counter */
        number_of_byte--;
    }
    /* wait for stop_cmd send over */
    I2C_WaitStopOver(i2cx);
}

/**
  * \brief wait for EEPROM standby state
  */
void Eeprom_WaitStandbyState(I2C_TypeDef* i2cx)
{
    uint32_t timeout = 0xFF;
    while(1)
    {
        /* wait until I2C bus is idle */
        while (I2C_Get_Status(i2cx, I2C_STATUS_BUSY)) {}
        I2C_ACK(i2cx, DISABLE);

        /* send slave address to I2C bus */
        I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, 0xFFFF);
        /* wait until I2C recv ack */
        do {
            timeout --;
        } while ((!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) && timeout);
        
        if(I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {
            I2C_Clear_Status(i2cx, I2C_STATUS_ADDR_MATCH);
            break;
        }

        /* send a stop cmd */
        I2C_Stop(i2cx);
        I2C_WaitStopOver(i2cx);
        I2C_Clear_Status(i2cx, I2C_STATUS_ADDR_MATCH);
        /* wait for 0x3f empty cycle */
        for(int i = 0; i < 0x3f; i++){
            __NOP();
        }
    }
    /* send a stop cmd */
    I2C_Stop(i2cx);
    I2C_WaitStopOver(i2cx);
}

/**
  * \brief write buffer of data to the I2C EEPROM
  * \param p_buffer pointer to the buffer  containing the data to be written to the EEPROM
  * \param write_address EEPROM's internal address to write to
  * \param number_of_byte number of bytes to write to the EEPROM
  */
void Eeprom_BufferWrite(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t write_address, uint16_t number_of_byte)
{
    uint8_t number_of_page = 0, number_of_single = 0, address = 0, count = 0;

    address = write_address % EEPROM_PAGE_SIZE;
    count = EEPROM_PAGE_SIZE - address;
    number_of_page =  number_of_byte / EEPROM_PAGE_SIZE;
    number_of_single = number_of_byte % EEPROM_PAGE_SIZE;

    /* if write_address is EEPROM_PAGE_SIZE aligned  */
    if (0 == address) {
        while (number_of_page--) {
            Eeprom_PageWrite(i2cx, p_buffer, write_address, EEPROM_PAGE_SIZE);
            Eeprom_WaitStandbyState(i2cx);
            write_address +=  EEPROM_PAGE_SIZE;
            p_buffer += EEPROM_PAGE_SIZE;
        }
        if (0 != number_of_single) {
            Eeprom_PageWrite(i2cx, p_buffer, write_address, number_of_single);
            Eeprom_WaitStandbyState(i2cx);
        }
    } else {
        /* if write_address is not EEPROM_PAGE_SIZE aligned */
        if (number_of_byte < count) {
            Eeprom_PageWrite(i2cx, p_buffer, write_address, number_of_byte);
            Eeprom_WaitStandbyState(i2cx);
        } else {
            number_of_byte -= count;
            number_of_page =  number_of_byte / EEPROM_PAGE_SIZE;
            number_of_single = number_of_byte % EEPROM_PAGE_SIZE;
            if (0 != count) {
                Eeprom_PageWrite(i2cx, p_buffer, write_address, count);
                Eeprom_WaitStandbyState(i2cx);
                write_address += count;
                p_buffer += count;
            }
            /* write page */
            while (number_of_page--) {
                Eeprom_PageWrite(i2cx, p_buffer, write_address, EEPROM_PAGE_SIZE);
                Eeprom_WaitStandbyState(i2cx);
                write_address +=  EEPROM_PAGE_SIZE;
                p_buffer += EEPROM_PAGE_SIZE;
            }
            /* write single */
            if (0 != number_of_single) {
                Eeprom_PageWrite(i2cx, p_buffer, write_address, number_of_single);
                Eeprom_WaitStandbyState(i2cx);
            }
        }
    }
}

/**
  * \brief read data from the EEPROM via DMA
  * \param read_address EEPROM's internal address to start reading from
  * \param buffer_size tx data size
  */
void Eeprom_ByteReadDma(I2C_TypeDef* i2cx, uint8_t read_address, uint32_t buffer_size)
{
    uint32_t timeout = 0xFFFFFFFF;
    I2C_ACK(i2cx, DISABLE);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, timeout);
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx,I2C_STATUS_ADDR_MATCH );

    /* send ram addr */
    I2C_Write_Done(i2cx, read_address, timeout);
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}

    I2C_Restart_Cmd();
    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_READ, timeout);
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx,I2C_STATUS_ADDR_MATCH );

    I2C_ACK(i2cx, ENABLE);
    I2C_AutoEnd_En(i2cx, ENABLE);
    I2C_Set_RxSize(i2cx, buffer_size);
    I2C_RxDma_Start(i2cx, ENABLE);
    I2C_Dma_Cfg(i2cx, ENABLE);

    /* wait for trans completed */
    while (rx_dma_done_flag == 1) {}
    while (!I2C_Get_Status(i2cx, I2C_STATUS_EOT)) {}
    
    I2C_Clear_Status(i2cx, I2C_STATUS_EOT);
    
    I2C_Dma_Cfg(i2cx, DISABLE);
    I2C_RxDma_Start(i2cx, DISABLE);
    I2C_AutoEnd_En(i2cx, DISABLE);
    
}

/**
  * \brief  write bytes to the EEPROM via DMA
  * \param write_address EEPROM's internal address to write to
  * \param buffer_size tx data size
  */
void Eeprom_ByteWriteDma(I2C_TypeDef* i2cx, uint8_t write_address, uint32_t buffer_size)
{
    uint32_t timeout = 0xFFFFFFFF;
    /* send a start condition to I2C bus */
    
    I2C_ACK(i2cx, DISABLE);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, timeout);
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx,I2C_STATUS_ADDR_MATCH );

    /* send ram addr */
    I2C_Write_Done(i2cx, write_address, timeout);
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}

    /* dma mode */
    I2C_Set_TxSize(i2cx, buffer_size);
    I2C_AutoEnd_En(i2cx, ENABLE);
    I2C_TxDma_Start(i2cx, ENABLE);
    I2C_Dma_Cfg(i2cx, ENABLE);

    /* wait dma complete trans */
    while (tx_dma_done_flag == RESET) {}
    /* wait i2c complete trans */
    while (!I2C_Get_Status(i2cx, I2C_STATUS_EOT)) {}
    /* send stop cmd */
    
    I2C_Clear_Status(i2cx, I2C_STATUS_EOT);
    
    I2C_Dma_Cfg(i2cx, DISABLE);
    I2C_TxDma_Start(i2cx, DISABLE);
    I2C_AutoEnd_En(i2cx, DISABLE);
    
}

/**
  * \brief read data from the EEPROM via int
  * \param read_address EEPROM's internal address to start reading from
  * \param buffer_size tx data size
  */
void Eeprom_ByteReadInt(I2C_TypeDef* i2cx, uint8_t read_address, uint32_t buffer_size)
{
    uint32_t timeout = 0xFFFFFFFF;
    I2C_ACK(i2cx, DISABLE);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, timeout);
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx,I2C_STATUS_ADDR_MATCH );

    /* send ram addr */
    I2C_Write_Done(i2cx, read_address, timeout);
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}

    I2C_Restart_Cmd();
    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_READ, timeout);
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx,I2C_STATUS_ADDR_MATCH );

    I2C_ACK(i2cx, ENABLE);
    I2C_Read_En(i2cx);

    I2C_Set_RxSize(i2cx, buffer_size);
    I2C_Set_RxWaterMark(i2cx, 0);
    I2C_Interrupt_En(i2cx, I2C_INT_IE_RXFIFO_WM_ENABLE, ENABLE);

    /* wait i2c complete trans */
    while(!I2C_Get_Status(i2cx, I2C_STATUS_EOT));
    /* send nack and stop cmd */
    I2C_ACK(i2cx, DISABLE);
    I2C_Stop(i2cx);
    I2C_Clear_Status(i2cx, I2C_STATUS_EOT);
    I2C_WaitStopOver(i2cx);
}

/**
  * \brief  write bytes to the EEPROM via int
  * \param write_address EEPROM's internal address to write to
  * \param buffer_size tx data size
  */
void Eeprom_ByteWriteInt(I2C_TypeDef* i2cx, uint8_t write_address, uint32_t buffer_size)
{
    uint32_t timeout = 0xFFFFFFFF;
    /* send a start condition to I2C bus */
    
    I2C_ACK(i2cx, DISABLE);

    /* send slave address to I2C bus */
    I2C_Send_SlaveAddr(i2cx, EEPROM_ADDR, I2C_DIR_WRITE, timeout);
    while (!I2C_Get_Status(i2cx, I2C_STATUS_ADDR_MATCH)) {}
    I2C_Clear_Status(i2cx,I2C_STATUS_ADDR_MATCH );
    /* send ram addr */
    I2C_Write_Done(i2cx, write_address, timeout);
    while (I2C_Get_Status(i2cx, I2C_STATUS_NACK)) {}

    I2C_Set_TxSize(i2cx, buffer_size);
    I2C_Set_TxWaterMark(i2cx, 1);
    I2C_Interrupt_En(i2cx, I2C_INT_IE_TXFIFO_WM_ENABLE, ENABLE);

    /* wait i2c complete trans */
    while (!I2C_Get_Status(i2cx, I2C_STATUS_EOT)) {}
    /* send stop cmd */
    I2C_Stop(i2cx);
    I2C_Clear_Status(i2cx, I2C_STATUS_EOT);
    I2C_WaitStopOver(i2cx);
}
