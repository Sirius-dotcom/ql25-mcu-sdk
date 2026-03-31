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

#include "ns_idu.h"

uint32_t IDU_GetHartNum(unsigned long base)
{
    return REG32(base + IDU_HART_NUM_OFFSET);
}

uint32_t IDU_GetClusterNum(unsigned long base)
{
    return REG32(base + IDU_CLUSTER_NUM_OFFSET);
}

uint32_t IDU_GetSemaphoreNum(unsigned long base)
{
    return REG32(base + IDU_SEMAPHORE_NUM_OFFSET);
}

uint32_t IDU_GetProcessxICI_Info(unsigned long base, uint32_t receive_core_id)
{
    return ((REG32(base + IDU_CORE_INT_STATUS_OFFSET+ receive_core_id*4) & CORE_ICI_INFO) >> 23);
}

uint32_t IDU_GetProcessxICI_Status(unsigned long base, uint32_t receive_core_id)
{
    return (REG32(base + IDU_CORE_INT_STATUS_OFFSET + receive_core_id*4) & CORE_ICI_STATUS);
}

void IDU_ProcessxClearICI(unsigned long base, uint32_t core_id, uint32_t val)
{
    REG32(base + IDU_CORE_INT_STATUS_OFFSET + core_id *4) = val;
}

void IDU_SendICI(unsigned long base, uint32_t send_core_id, uint32_t receive_core_id)
{
    REG32(base + IDU_IRQ_SEND_RECEIVE_CORE_ID_OFFSET) = ((send_core_id << 16) | receive_core_id);
}

/* int distribution fcfc mode */
FlagStatus IDU_IsFirstClaim(unsigned long base, uint32_t int_id, uint32_t core_id)
{
    REG32(base + IDU_SOC_INDICATOR_MASK_OFFSET + (int_id) * 4) = BIT(core_id);

    if(BIT(core_id) != REG32(base + IDU_SOC_INDICATOR_MASK_OFFSET + (int_id) * 4)) {
        return RESET;
    }
    return SET;
}

uint32_t IDU_GetClaim(unsigned long base, uint32_t int_id)
{
    return REG32(base + IDU_SOC_INDICATOR_MASK_OFFSET + (int_id) * 4);
}

void IDU_SetClaim(unsigned long base, uint32_t int_id, uint32_t val)
{
    REG32(base + IDU_SOC_INDICATOR_MASK_OFFSET + (int_id) * 4)= val;
}

uint32_t IDU_GetIndicator(unsigned long base, uint32_t int_id)
{
    return REG32(base + IDU_SOC_INT_INDICATOR_OFFSET + (int_id) * 4);
}

void IDU_SetIndicator(unsigned long base, uint32_t int_id, uint32_t val)
{
    REG32(base + IDU_SOC_INT_INDICATOR_OFFSET + (int_id) * 4) = val;
}

/* event */
void IDU_SetEventSel(unsigned long base,uint32_t core_id, uint32_t val)
{
    REG32(base + IDU_IRQ_HART0_EVENT_SEL + core_id *4) = val;
}

void IDU_SetIrqAsEvent(unsigned long base, uint32_t int_id)
{
    REG32(base + IDU_SOC_IRQ_AS_EVENT_EN_GRP0 + (int_id/32)*4) |= BIT(int_id%32);
}

/* semaphore */
void IDU_SetSemaphoreStatus(unsigned long base, uint32_t id, uint32_t val)
{
    REG32(base + IDU_SEMAPHORE_STATUS_OFFSET + id * 4) = val;
}

uint32_t IDU_GetSemaphoreStatus(unsigned long base, uint32_t id)
{
    return REG32(base + IDU_SEMAPHORE_STATUS_OFFSET + id * 4);
}

uint32_t IDU_CheckSemaphoreStatus(unsigned long base, uint32_t id, uint32_t val)
{
    REG32(base + IDU_SEMAPHORE_STATUS_OFFSET + id * 4) = val;

    if(REG32(base + IDU_SEMAPHORE_STATUS_OFFSET + id * 4) == val) {
        return 1;
    } else {
        return 0;
    }
}

/* lock idu*/
void IDU_LockNMI(unsigned long base)
{
    REG32(base + IDU_NMI_LOCK_OFFSET)=0x12345678;
    REG32(base + IDU_NMI_LOCK_OFFSET)=0x90CDEFAB;
}

void IDU_SetNMISel(unsigned long base,uint32_t core_id, uint32_t val)
{
    REG32(base + IDU_NMI_SEL_OFFSET + core_id * 4) = val;
}

uint32_t IDU_GetLockNMI(unsigned long base, uint32_t core_id)
{
    return REG32(base + IDU_NMI_SEL_OFFSET + core_id * 4);
}
