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
/* Includes ------------------------------------------------------------------*/
#include "ns.h"
#include "ns_iomux.h"

/**
 * @file     ns_iomux.c
 * @brief    This file contains all the functions prototypes for the IOMUX firmware
  */

struct pad_control_t {
	uint32_t ie_sel : 1;
    uint32_t pd_sel : 1;
    uint32_t pu_sel : 1;
    uint32_t oe_sel : 1;
    uint32_t do_sel : 2;
    uint32_t bus_keep_sel : 1;
    uint32_t Reserved : 25;
} __attribute__((packed, aligned(4)));

struct pad_config_t {
    struct pad_physical_config_t pad_phy_cfg;
    struct pad_control_t pad_ctl;
    uint32_t pad_spw;
}__attribute__((packed, aligned(4)));

struct iomux_s{

}__attribute__((packed, aligned(4)));

struct pad_group pad_goup_info[]=
{
                    
    { .pad_id = 0,.group_id=0},
                    
    { .pad_id = 1,.group_id=0},
                    
    { .pad_id = 2,.group_id=0},
                    
    { .pad_id = 3,.group_id=0},
                    
    { .pad_id = 4,.group_id=0},
                    
    { .pad_id = 5,.group_id=0},
                    
    { .pad_id = 6,.group_id=0},
                    
    { .pad_id = 7,.group_id=0},
                    
    { .pad_id = 8,.group_id=0},
                    
    { .pad_id = 9,.group_id=0},
                    
    { .pad_id = 10,.group_id=0},
                    
    { .pad_id = 11,.group_id=0},
                    
    { .pad_id = 12,.group_id=0},
                    
    { .pad_id = 13,.group_id=0},
                    
    { .pad_id = 14,.group_id=0},
                    
    { .pad_id = 15,.group_id=0},
                    
    { .pad_id = 16,.group_id=0},
                    
    { .pad_id = 17,.group_id=0},
                    
    { .pad_id = 18,.group_id=0},
                    
    { .pad_id = 19,.group_id=0},
                    
    { .pad_id = 20,.group_id=0},
                    
    { .pad_id = 21,.group_id=0},
                    
    { .pad_id = 22,.group_id=0},
                    
    { .pad_id = 23,.group_id=0},
                    
    { .pad_id = 24,.group_id=0},
                    
    { .pad_id = 25,.group_id=0},
                    
    { .pad_id = 26,.group_id=0},
                    
    { .pad_id = 27,.group_id=0},
                    
    { .pad_id = 28,.group_id=0},
                    
    { .pad_id = 29,.group_id=0},
                    
    { .pad_id = 30,.group_id=0},
                    
    { .pad_id = 31,.group_id=0},
                    
    { .pad_id = 32,.group_id=0},
                    
    { .pad_id = 33,.group_id=0},
                    
    { .pad_id = 34,.group_id=0},
                    
    { .pad_id = 35,.group_id=0},
                    
    { .pad_id = 36,.group_id=0},
                    
    { .pad_id = 37,.group_id=0},
                    
    { .pad_id = 38,.group_id=0},
                    
    { .pad_id = 39,.group_id=0},
                    
    { .pad_id = 40,.group_id=0},
                    
    { .pad_id = 41,.group_id=0},
                    
    { .pad_id = 42,.group_id=0},
                    
    { .pad_id = 43,.group_id=0},
                    
    { .pad_id = 44,.group_id=0},
                    
    { .pad_id = 45,.group_id=0},
                    
    { .pad_id = 46,.group_id=0},
                    
    { .pad_id = 47,.group_id=0},
                    
    { .pad_id = 48,.group_id=0},
                    
    { .pad_id = 49,.group_id=0},
                    
    { .pad_id = 50,.group_id=0},
                    
    { .pad_id = 51,.group_id=0},
                    
    { .pad_id = 52,.group_id=0},
                    
    { .pad_id = 53,.group_id=0},
};

struct ls_iof_t ls_iof[]={

                    };

struct ls_grp_iof_t ls_grp_iof[]={
    
};

static const struct pad_config_t iomux_physical_config[PAD_NUM]={

    /* PAD0 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD1 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD2 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD3 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD4 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD5 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD6 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD7 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD8 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD9 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD10 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD11 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD12 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD13 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD14 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD15 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD16 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD17 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD18 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD19 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD20 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD21 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD22 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD23 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD24 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD25 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD26 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD27 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD28 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD29 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD30 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD31 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD32 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD33 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD34 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD35 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD36 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD37 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD38 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD39 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD40 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD41 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD42 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD43 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD44 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD45 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD46 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD47 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD48 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD49 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD50 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD51 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD52 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },

    /* PAD53 */
    {
        {.ie_en        = 0 ,
        .ie_inv        = 0 ,
        .oe_en         = 0,
        .oe_inv        = 0,
        .do_val        = 0,
        .do_inv        = 0,
        .ds            = 0,
        .pd            = 0,
        .pu            = 0,
        .spu           = 0,
        .st            = 0,
        .sl            = 0,
        .analog_en     = 0,
        .sw_bus_keep   = 1},
        {.ie_sel       = 1,
        .pd_sel        = 1,
        .pu_sel        = 1,
        .oe_sel        = 1,
        .do_sel        = 1,
        .bus_keep_sel  = 1,},
        .pad_spw       = 1,
    },
};

static uint32_t get_pad_group(uint32_t pad_id)
{
    uint32_t index=0xFFFFFFFF;
    if(pad_goup_info[pad_id].pad_id != pad_id)
    {
        for(int i=0; i<pad_id; i++)
        {
            if(pad_goup_info[i].pad_id == pad_id)
            {
                index=i;
                break;
            }
        }
    }else{
        index=pad_id;
    }
    return index ;
}

static uint32_t iomux_set_io_raw(uint32_t IO_MUX_BASE, uint32_t pad_id, uint32_t per_iof_id)
{
    
    REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS(pad_goup_info[pad_id].group_id) + 0x4 * per_iof_id))= PAD_NUM+1 ;    
    
    return 0;
}

static uint32_t iomux_set_io_func(uint32_t IO_MUX_BASE, uint32_t pad_id,uint32_t hs_ls,uint32_t per_iof_id)
{
    uint32_t pad_index;
    uint32_t * addr;
    pad_index=get_pad_group(pad_id); 
    
    addr = ADDR32P(IO_MUX_BASE + LS_SRC_SEL_OFS(pad_goup_info[pad_index].group_id));
    addr[pad_id] = per_iof_id ; 
    addr = ADDR32P(IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id));
    addr[pad_id] = hs_ls;
    addr = ADDR32P(IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id));  
    addr[pad_id] |= DO_SEL_HS | OE_SEL_HS_CHANNEL  ;    
    
    return 0;
}

static uint32_t set_pad_ls_ival_tie0(unsigned long IO_MUX_BASE,uint32_t pad_id)
{
    uint32_t group_id_tmp;
    group_id_tmp = pad_goup_info[pad_id].group_id;
    for(uint32_t i = ls_grp_iof[group_id_tmp].grp_iof_start_id ;(sizeof(ls_grp_iof) != 0) && (i<ls_grp_iof[group_id_tmp].grp_iof_end_id+1); i++)
    {
        if(REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS(group_id_tmp) + 0x4 * i))== pad_id)
        {
            REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS(group_id_tmp) + 0x4 * i)) = PAD_NUM+1;
        }
    }

    return 0;
}

uint8_t iomux_set_io_oval(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls)
{
    uint32_t pad_index;
    uint32_t * addr;
    pad_index=get_pad_group(pad_id);
    switch(hs_ls)
    {
       case 0:
            
            /* Compare all IO */
            for (int index = 0; index < PAD_NUM; index++) {
                if (REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id)  + 0x4 * pad_id)) == per_iof_id && index != pad_id)
                    iomux_set_io_raw(IO_MUX_BASE,index, per_iof_id);
            }
            iomux_set_io_func(IO_MUX_BASE,pad_id,hs_ls, per_iof_id);
            break;
        case 1:
        case 2:
        case 3: 
            addr = ADDR32P(IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id));
            addr[per_iof_id] = hs_ls;
            addr = ADDR32P(IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id));
            addr[per_iof_id] |=  DO_SEL_HS | OE_SEL_HS_CHANNEL; ;
            
            break;    
        case 4:
        case 5:
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))= hs_ls ;
        default:
            break;                                                                 
    }
    return 0;
}

uint8_t iomux_set_io_ival(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls)
{
    uint32_t pad_index;
    uint32_t * addr;
    pad_index=get_pad_group(pad_id);
    switch(hs_ls)
    {
        case 0:
            
            addr = ADDR32P(IO_MUX_BASE + LS_SRC_SEL_OFS(pad_goup_info[pad_index].group_id));
            addr[pad_id]= per_iof_id ; 
            addr = ADDR32P(IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS(pad_goup_info[pad_index].group_id));
            addr[per_iof_id]= pad_id ;
            addr = ADDR32P(IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id));
            addr[pad_id]|= 0x1 ;
            addr[pad_id]&= ~(BIT(3)) ;
            
            break;            
        case 1:
        case 2:
        case 3: 
            set_pad_ls_ival_tie0(IO_MUX_BASE,pad_id);
            addr = ADDR32P(IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id));
            addr[per_iof_id]= hs_ls ; 
            addr = ADDR32P(IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id));
            addr[per_iof_id] |=  DO_SEL_HS |  IE_SEL;
            
            break;
        default:
            break    ;                                                             
    }   
    return 0;  
}
 
uint32_t iomux_set_io_pull(unsigned long IO_MUX_BASE, uint32_t pad_id, enum iomux_pull_e pull)
{
    uint32_t pad_index;
    struct pad_physical_config_t * addr;
    pad_index=get_pad_group(pad_id);
    addr = (struct pad_physical_config_t *)ADDR32P(IO_MUX_BASE + PHY_CNTRL_OFS(pad_goup_info[pad_index].group_id));
    struct pad_physical_config_t cfg = (addr[pad_id]);

    switch(pull){
        case IOMUX_PULL_NONE:
            cfg.pd =0;
            cfg.pu=0;
            cfg.spu=0;
            break;
        case IOMUX_PULL_DOWN:
            cfg.pd =1;
            cfg.pu=0;
            cfg.spu=0;
            break;
        case IOMUX_PULL_UP:
            cfg.pd =0;
            cfg.pu=1;
            cfg.spu=0;
            break;
        case IOMUX_STRONG_PULL_UP:
            cfg.pd =0;
            cfg.pu=1;
            cfg.spu=1;
            break;
        default:
            break;
    }
    addr[pad_id] = cfg;
    return 0;
}

uint32_t iomux_get_io_pull(unsigned long IO_MUX_BASE, uint32_t pad_id)
{
    enum iomux_pull_e pull;
    uint32_t pad_index;
    struct pad_physical_config_t * addr;
    pad_index=get_pad_group(pad_id);
    addr = (struct pad_physical_config_t *)ADDR32P(IO_MUX_BASE + PHY_CNTRL_OFS(pad_goup_info[pad_index].group_id));
    struct pad_physical_config_t cfg = (addr[pad_id]); 

	if (cfg.pd == 0)
		pull = IOMUX_PULL_NONE;
	else if (cfg.pu == 0)
		pull = IOMUX_PULL_DOWN;
	else if (cfg.spu == 0)
		pull = IOMUX_PULL_UP;
	else
		pull = IOMUX_STRONG_PULL_UP;
	return pull;

}

/**
  * \brief  Configurates the specified pad oval.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  per_iof_id: specifies the iof number from 0 to 31.
  * \param  pad_id:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  pad_grp: pad group.
  * \param  ls_per_grp: per group in ls mode.
  */

uint8_t iomux_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls,uint8_t  pad_grp,uint8_t ls_per_grp)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id);
    switch(hs_ls)
    {
       case 0:
            if(pad_grp != ls_per_grp)
                while(1);
            REG32((IO_MUX_BASE + LS_SRC_SEL_OFS(pad_goup_info[pad_index].group_id)    + 0x4 * pad_id))= per_iof_id ; 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id)  + 0x4 * pad_id))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id)  + 0x4 * pad_id)) |=  DO_SEL_HS | OE_SEL_HS_CHANNEL ;   
            break;
        case 1:
        case 2:
        case 3: 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))|=  DO_SEL_HS | OE_SEL_HS_CHANNEL; 
            break;    
        case 4:
        case 5:
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))= hs_ls ;
        default:
            break;                                                                 
    }
    return 0;
}

/**
  * \brief  Configurates the specified pad ival.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  per_iof_id: specifies the iof number from 0 to 31.
  * \param  pad_id:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  pad_grp: pad group.
  * \param  ls_per_grp: per group in ls mode.
  */
uint8_t iomux_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls ,uint8_t  pad_grp,uint8_t ls_per_grp)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id);
    switch(hs_ls)
    {
        case 0:
            if(pad_grp != ls_per_grp)
                while(1);
            REG32((IO_MUX_BASE + LS_SRC_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))= per_iof_id ; 
            REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS(pad_goup_info[pad_id].group_id) + 0x4 * per_iof_id))= pad_id ;    
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))|= 0x1 ;  
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))&= ~(BIT(3)) ;
            break;            
        case 1:
        case 2:
        case 3: 
            set_pad_ls_ival_tie0(IO_MUX_BASE,pad_id);
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))|=  DO_SEL_HS |  IE_SEL;      
            break;
        default:
            break    ;                                                             
    } 
    return 0;    
}

/**
  * \brief  Configurates the specified pad oval.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  per_iof_id: specifies the iof number from 0 to 31.
  * \param  pad_id:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */

void iomux_ls_iof_oval_cfg(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id);
    switch(hs_ls)
    {
       case 0:
            REG32((IO_MUX_BASE + LS_SRC_SEL_OFS(pad_goup_info[pad_index].group_id)    + 0x4 * pad_id))= per_iof_id ; 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id)) |=  DO_SEL_HS | OE_SEL_HS_CHANNEL ;   
            break;
        case 1:
        case 2:
        case 3: 
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))|=  DO_SEL_HS | OE_SEL_HS_CHANNEL; 
            break;    
     
        default:
            break;                                                                 
    }
    return;
}

/**
  * \brief  Configurates the specified pad ival.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  per_iof_id: specifies the iof number from 0 to 31.
  * \param  pad_id:  specifies the LGPIO pin from 0 to 31.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_ival_cfg(unsigned long IO_MUX_BASE, uint32_t per_iof_id,uint32_t pad_id, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id);
    switch(hs_ls)
    {
        case 0:
            REG32((IO_MUX_BASE + LS_SRC_SEL_OFS(pad_goup_info[pad_id].group_id) + 0x4 * pad_id))= per_iof_id ; 
            REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))= pad_id ;    
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))|= 0x1 ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_id].group_id) + 0x4 * pad_id))&= ~(BIT(3)) ; 
            break;            
        case 1:
        case 2:
        case 3: 

            set_pad_ls_ival_tie0(IO_MUX_BASE,pad_id);
            REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))= hs_ls ; 
            REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_id))|=  DO_SEL_HS |  IE_SEL;      
            break;
        default:
            break    ;                                                             
    }    
    return; 
}

void iomux_pad_phy_ctrl(unsigned long IO_MUX_BASE, uint32_t pad_id ,uint32_t ctrl )
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id);  
    REG32((IO_MUX_BASE + PHY_CNTRL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))|= ctrl ;              
}

/**
  * \brief  Pull up the specified pad.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  pad_id:  specifies the pad number.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_pullup_cfg(unsigned long IO_MUX_BASE, uint32_t pad_id, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{ 
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id);
    REG32((IO_MUX_BASE + PHY_CNTRL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))|= PU ;              
}

/**
  * \brief  Pull down the specified pad.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  pad_id:  specifies the pad number.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_pulldown_cfg(unsigned long IO_MUX_BASE, uint32_t pad_id, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id); 
    REG32((IO_MUX_BASE + PHY_CNTRL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))|= PD ;              
}

/**
  * \brief  Configurates the specified pad only in low speed mode.
  * \param  IO_MUX_BASE: the IO_MUX base address.
  * \param  per_iof_ival_num : specified peripheral the iof number of ival.
  * \param  per_iof_oval_num : specified peripheral the iof number of oval.
  * \param  pad_id:  specifies the pad number.
  * \param  hs_ls: high speed/low speed
  * \param  phy_cntr_sel: reserved.
  * \param  phy_cntr: reserved.
  */
void iomux_ls_iof_loopback_cfg(unsigned long IO_MUX_BASE, uint32_t per_iof_ival_num ,uint32_t per_iof_oval_num ,uint32_t pad_id, uint8_t hs_ls ,uint8_t  phy_cntr_sel,uint32_t phy_cntr)
{   
    uint32_t pad_index;   
    pad_index=get_pad_group(pad_id);         
    REG32((IO_MUX_BASE + LS_SRC_SEL_OFS(pad_goup_info[pad_index].group_id)    + 0x4 * pad_id))= per_iof_oval_num ; 
    REG32((IO_MUX_BASE + HS_CHNL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))= hs_ls ; 
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id)) |=  DO_SEL_HS | OE_SEL_HS_CHANNEL ;  
    REG32((IO_MUX_BASE + LS_SRC0_IVAL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * per_iof_ival_num))= pad_id ;    
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id))|= 0x1 ;               
}

void iomux_pu_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_id)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id); 
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id)) |= PU_SEL ;
}

void iomux_pd_sel_assert(unsigned long IO_MUX_BASE, uint32_t pad_id)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id); 
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id)) |= PD_SEL ;
}

void iomux_pu_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_id)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id); 
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id)) &= (uint32_t)(~PU_SEL) ;
}

void iomux_pd_sel_deassert(unsigned long IO_MUX_BASE, uint32_t pad_id)
{
    uint32_t pad_index;
    pad_index=get_pad_group(pad_id); 
    REG32((IO_MUX_BASE + CNTRL_SEL_OFS(pad_goup_info[pad_index].group_id) + 0x4 * pad_id)) &= (uint32_t)(~PD_SEL) ;
}
