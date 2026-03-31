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

#include "ns_i2c.h"
#ifndef _NSEEPROM_H
#define _NSEEPROM_H
#ifdef __cplusplus
extern "C" {
#endif

#define EEPROM_BLOCK0_ADDRESS  (0x50)
#define EEPROM_BLOCK1_ADDRESS  (0x52)
#define EEPROM_BLOCK2_ADDRESS  (0x54)
#define EEPROM_BLOCK3_ADDRESS  (0x56)

#define EEPROM_PAGE_SIZE       (16)
void Eeprom_ByteWriteInt(I2C_TypeDef* i2cx, uint8_t write_address, uint32_t buffer_size);
void Eeprom_ByteReadInt(I2C_TypeDef* i2cx, uint8_t read_address, uint32_t buffer_size);
void Eeprom_ByteReadDma(I2C_TypeDef* i2cx, uint8_t read_address, uint32_t buffer_size);
void Eeprom_ByteWriteDma(I2C_TypeDef* i2cx, uint8_t write_address, uint32_t buffer_size);
void Eeprom_ByteWrite(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t write_address);
void Eeprom_PageWrite(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t write_address, uint8_t number_of_byte);
void Eeprom_BufferRead(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t read_address, uint16_t number_of_byte);
void Eeprom_BufferWrite(I2C_TypeDef* i2cx, uint8_t* p_buffer, uint8_t write_address, uint16_t number_of_byte);
void Eeprom_WaitStandbyState(I2C_TypeDef* i2cx);
#ifdef __cplusplus
}
#endif

#endif /* _NSI2C_H */
