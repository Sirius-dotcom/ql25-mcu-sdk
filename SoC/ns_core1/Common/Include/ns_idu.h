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

#ifndef _NS__IDU_H
#define _NS__IDU_H
/*!
 * @file     ns_idu.h
 * @brief    This file contains all the functions prototypes for the IDU firmware
 */

#ifdef __cplusplus
 extern "C" {
#endif

#include "ns.h"

#define IDU_CORE_INT_STATUS_OFFSET                  0x00
#define IDU_SEMAPHORE_STATUS_OFFSET                 0X80
#define IDU_IRQ_HART0_EVENT_SEL                     0x100
#define IDU_SOC_IRQ_AS_EVENT_EN_GRP0                0x180
#define IDU_IRQ_SEND_RECEIVE_CORE_ID_OFFSET         0X3FFC
#define IDU_SOC_INT_INDICATOR_OFFSET                0X4000
#define IDU_SOC_INDICATOR_MASK_OFFSET               0X8000

#define IDU_NMI_SEL_OFFSET                          0XC000
#define IDU_NMI_LOCK_OFFSET                         0XC080
#define IDU_CLUSTER_NUM_OFFSET                      0XC084
#define IDU_HART_NUM_OFFSET                         0XC088
#define IDU_SEMAPHORE_NUM_OFFSET                    0XC08C

#define CORE_ICI_STATUS                             BITS(0,23)
#define CORE_ICI_INFO                               BITS(24,31)

uint32_t IDU_GetHartNum(unsigned long base);
uint32_t IDU_GetClusterNum(unsigned long base);
uint32_t IDU_GetSemaphoreNum(unsigned long base);

uint32_t IDU_GetProcessxICI_Info(unsigned long base, uint32_t receive_core_id);
uint32_t IDU_GetProcessxICI_Status(unsigned long base, uint32_t receive_core_id);

void IDU_ProcessxClearICI(unsigned long base, uint32_t core_id, uint32_t val);
void IDU_SendICI(unsigned long base, uint32_t send_core_id, uint32_t receive_core_id);
/* int distribution fcfc mode*/
FlagStatus IDU_IsFirstClaim(unsigned long base, uint32_t int_id, uint32_t core_id);
uint32_t IDU_GetClaim(unsigned long base, uint32_t int_id);
void IDU_SetClaim(unsigned long base, uint32_t int_id, uint32_t val);
uint32_t IDU_GetIndicator(unsigned long base, uint32_t int_id);
void IDU_SetIndicator(unsigned long base, uint32_t int_id, uint32_t val);
/* event */
void IDU_SetEventSel(unsigned long base,uint32_t core_id, uint32_t val);
void IDU_SetIrqAsEvent(unsigned long base, uint32_t int_id);
/* semaphore */
void IDU_SetSemaphoreStatus(unsigned long base, uint32_t id,uint32_t val);
uint32_t IDU_GetSemaphoreStatus(unsigned long base, uint32_t id);
uint32_t IDU_CheckSemaphoreStatus(unsigned long base, uint32_t id,uint32_t val);
/* lock idu*/
void IDU_LockNMI(unsigned long base);
void IDU_SetNMISel(unsigned long base,uint32_t core_id,uint32_t val);
uint32_t IDU_GetLockNMI(unsigned long base,uint32_t core_id);

#ifdef __cplusplus
}
#endif
#endif /* _NS__H */
