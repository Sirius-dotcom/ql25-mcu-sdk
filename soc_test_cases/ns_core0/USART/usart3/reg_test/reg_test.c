
/*
NOTE: this file is automacally generated from reg_test_gen.py.
source file may include vpp cfg define and define vpp file.
main register info are extracted from ip reg csv file.
*/
            
#include <stdio.h>
#include "nuclei_sdk_soc.h"

//#define DEBUG
#ifdef DEBUG
#define debug(argc,argv...) ({printf(argc,##argv);} )
#else
#define debug(argc,argv...)
#endif // DEBUG

#define ERR_LOG    30
uint32_t err_number[ERR_LOG] ;
uint32_t err_value[ERR_LOG] ;
uint32_t Index;
uint32_t volatile index_for_clk_diable=0  ;

void err_log(addr_xlen addr)
{
    if(Index>ERR_LOG)
    {
       // debug("Err : Out of Array Boundary\r\n");
        return ;
    }
    err_number[Index++] = addr;
    err_value[Index++] = REG32(addr);

}


static inline void REG_EWR_CHECK(addr_xlen addr,uint32_t ref_value)
{
    REG32(addr)=ref_value;
    if(ref_value == REG32(addr))
    {
        #ifndef CFG_SIMULATION
       // debug("reg  addr:%x # PASS #\r\n",addr);
        #endif
    }
    else
    {
        debug("reg addr:%x , actual_value :%x <-> expected_value :%x, FAIL #\r\n",addr,REG32(addr),ref_value);
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif

            err_log(addr);
    }
}

static inline void REG_EWR_DOUBLE_CHECK(addr_xlen addr,uint32_t ref_value)
{
    REG32(addr)=ref_value;
    if(ref_value == REG32(addr))
    {
        if(ref_value == REG32(addr))
        {            
            #ifndef CFG_SIMULATION
            // debug("reg  addr:%x # PASS #\r\n",addr);
            #endif
        } else
        {
            debug("reg addr:%x , actual_value :%x <-> test value :%x, FAIL #\r\n",addr,REG32(addr),ref_value);
            #ifdef CFG_SIMULATION
            simulation_fail();
            #endif
            err_log(addr);
        }
    }
    else
    {
        debug("reg addr:%x , actual_value :%x <-> expected_value :%x, FAIL #\r\n",addr,REG32(addr),ref_value);
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif

            err_log(addr);

    }
    
}

static inline void REG_PWR_CHECK(addr_xlen addr,uint32_t test_val,uint32_t ref_val ,uint32_t field_mask)
{
    uint32_t reg_tmp;
    REG32(addr) = test_val;
    reg_tmp = REG32(addr);
    if(ref_val == ( reg_tmp & field_mask ) )
    {
        #ifndef CFG_SIMULATION
        //debug("reg  addr:%x REG_PWR # PASS #\r\n",addr);
        #endif
    }
    else
    {
        debug("reg addr:%x , actual_value :%x <-> expected_value :%x, FAIL #\r\n",addr,( reg_tmp & field_mask ),ref_val);  
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif
        err_log(addr)  ;
    }
}

static inline void REG_PWR_DOUBLE_CHECK(addr_xlen addr,uint32_t test_val,uint32_t ref_val ,uint32_t field_mask)
{
    uint32_t reg_tmp;
    REG32(addr) = test_val;
    reg_tmp = REG32(addr);
    if(ref_val == ( reg_tmp & field_mask ) )
    {
        if(ref_val == ( reg_tmp & field_mask ) )
        {
            #ifndef CFG_SIMULATION
            //debug("reg  addr:%x REG_PWR # PASS #\r\n",addr);
            #endif
        } else
        {
            debug("reg addr:%x , actual_value :%x <-> test value :%x, FAIL #\r\n",addr,REG32(addr),test_val);
            #ifdef CFG_SIMULATION
            simulation_fail();
            #endif
            err_log(addr);
        }
    }
    else
    {
        debug("reg addr:%x , actual_value :%x <-> expected_value :%x, FAIL #\r\n",addr,( reg_tmp & field_mask ),ref_val);  
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif
        err_log(addr)  ;
    }
}
static inline void REG_RO_CHECK(addr_xlen addr,uint32_t test_val)
{
    uint32_t tmp ;
    tmp = REG32(addr);
    REG32(addr) = test_val;
    if(tmp == REG32(addr))
    {
        #ifndef CFG_SIMULATION
        //debug("reg  addr:%x # PASS #\r\n",addr);
        #endif
    }
    else
    {
         debug("reg addr:%x , actual_value :%x <-> test value :%x, FAIL #\r\n",addr,REG32(addr),test_val);
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif
        err_log(addr);
    }
}

static inline void REG_RO_DOUBLE_CHECK(addr_xlen addr,uint32_t test_val)
{
    uint32_t tmp ;
    tmp = REG32(addr);
    REG32(addr) = test_val;
    if(tmp == REG32(addr))
    {
        if(tmp == REG32(addr))
        {
            #ifndef CFG_SIMULATION
            //debug("reg  addr:%x # PASS #\r\n",addr);
            #endif
        } else
        {
            debug("reg addr:%x , actual_value :%x <-> test value :%x, FAIL #\r\n",addr,REG32(addr),test_val);
            #ifdef CFG_SIMULATION
            simulation_fail();
            #endif
            err_log(addr);
        }
    }
    else
    {
         debug("reg addr:%x , actual_value :%x <-> test value :%x, FAIL #\r\n",addr,REG32(addr),test_val);
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif
        err_log(addr);
    }
}

static inline void REG_RC_CHECK(addr_xlen addr, uint32_t default_val, uint32_t test_val)
{
    REG32(addr) = test_val;
    if(default_val == REG32(addr))
    {
        #ifndef CFG_SIMULATION
        //debug("reg  addr:%x # PASS #\r\n",addr);
         #endif
    }
    else
    {
         debug("reg addr:%x , actual_value :%x <-> test_value :%x, FAIL #\r\n",addr,REG32(addr),test_val);
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif
        err_log(addr);
    }
}

static inline void REG_RC_DOUBLE_CHECK(addr_xlen addr, uint32_t default_val, uint32_t test_val)
{
    REG32(addr) = test_val;
    if(default_val == REG32(addr))
    {
        if(default_val == REG32(addr))
        {
            #ifndef CFG_SIMULATION
            //debug("reg  addr:%x # PASS #\r\n",addr);
            #endif
        } else
        {
            debug("reg addr:%x , actual_value :%x <-> test value :%x, FAIL #\r\n",addr,REG32(addr),test_val);
            #ifdef CFG_SIMULATION
            simulation_fail();
            #endif
            err_log(addr);
        }
    }
    else
    {
         debug("reg addr:%x , actual_value :%x <-> test_value :%x, FAIL #\r\n",addr,REG32(addr),test_val);
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif
        err_log(addr);
    }
}

static inline void __attribute__((optimize("O0"))) REG_CLK_DIS_CHECK(addr_xlen addr)
{
    uint8_t volatile state1, state2, state3=0;
    
    state1=(0==REG32(addr));
    
    for(int i =0;i<20;i++)
    {
        __NOP();
    }
    if(index_for_clk_diable==1)
    {
        index_for_clk_diable=0;
        #ifndef CFG_SIMULATION
        //debug("reg  addr:%x CLOCK DIS CHECK #### PASS ####\r\n",addr);
        #endif
    }
    else
    {
        printf("reg  addr:%x CLOCK DIS CHECK #### FAIL ####\r\n",addr);
        err_log(addr);
    }

}


static inline void REG_Default_CHECK(addr_xlen addr,uint32_t default_val)
{
    if(default_val == REG32(addr))
    {
        #ifndef CFG_SIMULATION
        //debug("reg  addr:%x DefVal_CKECK # PASS #\r\n",addr);
        #endif
    }
    else
    {
         debug("reg addr:%x , actual_value :%x <-> default_val :%x, FAIL #\r\n",addr,REG32(addr),default_val);
        #ifdef CFG_SIMULATION
        simulation_fail();
        #endif
        err_log(addr);
    }
}


static inline void REG_Write_Defval_back(addr_xlen addr,uint32_t default_val)
{
    REG32(addr)=default_val;
}


uint8_t REG_DefVal_CHECK(addr_xlen addr,uint32_t default_val)
{
    REG_Default_CHECK( addr,  default_val);
}


//EWR : The entire field of the register is readable and writable
//PWR : Parts of the register are readable and writable
//RO : register is  read only
//WO :  register is  write only


/*
    default_val : register defult value
    test_val : register write value
    ref_val  : register ref value , contrast  with the actual read value
*/

//EWR
uint8_t REG_EWR_Check(addr_xlen addr,uint32_t default_val)
{
    REG_EWR_CHECK(addr,0xFFFFFFFF);
    REG_EWR_CHECK(addr,0x0);
    REG_EWR_CHECK(addr,0xFFFFFFFF);
    REG_EWR_CHECK(addr,0x0);
    REG_EWR_CHECK(addr,0x5a5a5a5a);
    REG_EWR_CHECK(addr,0xa5a5a5a5);
    REG_EWR_CHECK(addr,0x5a5a5a5a);
    REG_EWR_CHECK(addr,0xa5a5a5a5);
    REG_EWR_CHECK(addr,0xa5a55a5a);
    REG_EWR_CHECK(addr,0x5a5aa5a5);
    REG_Write_Defval_back(addr,default_val);
}

uint8_t REG_EWR_Double_Check(addr_xlen addr,uint32_t default_val)
{
    REG_EWR_DOUBLE_CHECK(addr,0xFFFFFFFF);
    REG_EWR_DOUBLE_CHECK(addr,0x0);
    REG_EWR_DOUBLE_CHECK(addr,0xFFFFFFFF);
    REG_EWR_DOUBLE_CHECK(addr,0x0);
    REG_EWR_DOUBLE_CHECK(addr,0x5a5a5a5a);
    REG_EWR_DOUBLE_CHECK(addr,0xa5a5a5a5);
    REG_EWR_DOUBLE_CHECK(addr,0x5a5a5a5a);
    REG_EWR_DOUBLE_CHECK(addr,0xa5a5a5a5);
    REG_EWR_DOUBLE_CHECK(addr,0xa5a55a5a);
    REG_EWR_DOUBLE_CHECK(addr,0x5a5aa5a5);
    REG_Write_Defval_back(addr,default_val);
}
//PWR
uint8_t REG_PWR_Check(addr_xlen addr, uint32_t default_val,uint32_t test_val, uint32_t ref_val, uint32_t field_mask)
{
    REG_PWR_CHECK(addr,test_val,ref_val ,field_mask);
    REG_Write_Defval_back(addr,default_val);
}

uint8_t REG_PWR_Double_Check(addr_xlen addr, uint32_t default_val,uint32_t test_val, uint32_t ref_val, uint32_t field_mask)
{
    REG_PWR_DOUBLE_CHECK(addr,test_val,ref_val ,field_mask);
    REG_Write_Defval_back(addr,default_val);
}
//RO
uint8_t REG_RO_Check(addr_xlen addr,uint32_t default_val,uint32_t test_val)
{
    REG_RO_CHECK(addr,test_val);
}

uint8_t REG_RO_Double_Check(addr_xlen addr,uint32_t default_val,uint32_t test_val)
{
    REG_RO_DOUBLE_CHECK(addr,test_val);
}

//RC
uint8_t REG_RC_Check(addr_xlen addr,uint32_t default_val,uint32_t test_val)
{
    REG_RC_CHECK(addr,default_val, test_val);
}

uint8_t REG_RC_Double_Check(addr_xlen addr,uint32_t default_val,uint32_t test_val)
{
    REG_RC_DOUBLE_CHECK(addr,default_val, test_val);
}


void __attribute__((optimize("O0"))) buserror_handler(unsigned long cause, unsigned long sp)
{
    index_for_clk_diable++;
}

void env_init(void)
{
    Exception_Register_EXC(5, (unsigned long)buserror_handler);
    Exception_Register_EXC(7, (unsigned long)buserror_handler);
}
void main(void)
{
    uint8_t state0=0;

    env_init();

uint32_t volatile start_mcycle2;
uint32_t volatile start_mcycle1;
uint32_t volatile delta_mcycle1;
uint32_t volatile delta_mcycle2;
    #ifdef MISC_HAS_USART3_HAS_CLK

        usart3_clk_en(ENABLE);

    #endif

    /*********************TEST FOR POR DEVAL *********************/

    /*testing for  TXDATA: addr=0x10033000*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: tx_data                  field_range: 8:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033000,0x0);

    /*testing for RO RXDATA: addr=0x10033004*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: rx_data                  field_range: 8:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x10033004,0x0);

    /*testing for  TXCTRL: addr=0x10033008*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: txcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: txen                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033008,0x0);

    /*testing for  RXCTRL: addr=0x1003300c*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: rxcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: rxen                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1003300c,0x0);

    /*testing for  INT_EN: addr=0x10033010*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: modbus_ascii_match_irq_en field_range: 20:20            field_access: rw  
    field_name: modbus_rtu_tout_irq_en   field_range: 19:19            field_access: rw  
    field_name: lin_lbdie_irq_en         field_range: 18:18            field_access: rw  
    field_name: frame_err_irq_en         field_range: 17:17            field_access: rw  
    field_name: rx_fifo_full_irq_en      field_range: 16:16            field_access: rw  
    field_name: rx_fifo_empty_irq_en     field_range: 15:15            field_access: rw  
    field_name: tx_fifo_full_irq_en      field_range: 14:14            field_access: rw  
    field_name: tx_fifo_empty_irq_en     field_range: 13:13            field_access: rw  
    field_name: rx_eot_irq_en            field_range: 12:12            field_access: rw  
    field_name: tx_eot_irq_en            field_range: 11:11            field_access: rw  
    field_name: rx_wm_tout_irq_en        field_range: 10:10            field_access: rw  
    field_name: rx_idle_tout_irq_en      field_range: 9:9              field_access: rw  
    field_name: spi_slv_over_run_irq_en  field_range: 8:8              field_access: rw  
    field_name: spi_slv_under_run_irq_en field_range: 7:7              field_access: rw  
    field_name: stop_bit_irq_en          field_range: 6:6              field_access: rw  
    field_name: ctsie_fall_en            field_range: 5:5              field_access: rw  
    field_name: ctsie_rise_en            field_range: 4:4              field_access: rw  
    field_name: parity_irq_en            field_range: 3:3              field_access: rw  
    field_name: ow_irq_en                field_range: 2:2              field_access: rw  
    field_name: rxie                     field_range: 1:1              field_access: rw  
    field_name: txie                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033010,0x0);

    /*testing for  STATUS: addr=0x10033014*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: ne_flag                  field_range: 24:24            field_access: w1c 
    field_name: frame_err_flag           field_range: 23:23            field_access: w1c 
    field_name: rx_fifo_full             field_range: 22:22            field_access: ro  
    field_name: tx_fifo_empty            field_range: 21:21            field_access: ro  
    field_name: cts_pin                  field_range: 20:20            field_access: ro  
    field_name: spi_slv_over_run         field_range: 19:19            field_access: w1c 
    field_name: spi_slv_under_run        field_range: 18:18            field_access: w1c 
    field_name: rx_eot                   field_range: 17:17            field_access: w1c 
    field_name: tx_eot                   field_range: 16:16            field_access: w1c 
    field_name: rx_fifo_empty            field_range: 15:15            field_access: ro  
    field_name: tx_fifo_full             field_range: 14:14            field_access: ro  
    field_name: rx_wm_tout_status        field_range: 13:13            field_access: w1c 
    field_name: rx_idle_tout_status      field_range: 12:12            field_access: w1c 
    field_name: reserved                 field_range: 11:9             field_access: ro  
    field_name: stop_bit_err_flag        field_range: 8:8              field_access: w1c 
    field_name: cts_fall_flag            field_range: 7:7              field_access: w1c 
    field_name: cts_rise_flag            field_range: 6:6              field_access: w1c 
    field_name: rx_error_parity          field_range: 5:5              field_access: w1c 
    field_name: rx_error_over_flow       field_range: 4:4              field_access: w1c 
    field_name: rx_busy                  field_range: 3:3              field_access: ro  
    field_name: tx_busy                  field_range: 2:2              field_access: ro  
    field_name: rxip                     field_range: 1:1              field_access: ro  
    field_name: txip                     field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x10033014,0x208000);

    /*testing for  DIV: addr=0x10033018*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: baud_div                 field_range: 20:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10033018,0x0);

    /*testing for  SETUP: addr=0x1003301c*/
    /*
    field_name: cfg_stop_bit             field_range: 31:30            field_access: rw  
    field_name: swap                     field_range: 29:29            field_access: rw  
    field_name: rxinv                    field_range: 28:28            field_access: rw  
    field_name: txinv                    field_range: 27:27            field_access: rw  
    field_name: datainv                  field_range: 26:26            field_access: rw  
    field_name: msbfirst                 field_range: 25:25            field_access: rw  
    field_name: de_inv                   field_range: 24:24            field_access: rw  
    field_name: de_deassertion_delay_en  field_range: 23:23            field_access: rw  
    field_name: de_assertion_early_en    field_range: 22:22            field_access: rw  
    field_name: rs485_en                 field_range: 21:21            field_access: rw  
    field_name: rx_dma_en                field_range: 20:20            field_access: rw  
    field_name: fraction_baud            field_range: 19:19            field_access: rw  
    field_name: sample_position          field_range: 18:17            field_access: rw  
    field_name: vote_majority_en         field_range: 16:16            field_access: rw  
    field_name: dma_en                   field_range: 15:15            field_access: rw  
    field_name: ctse                     field_range: 14:14            field_access: rw  
    field_name: rtse                     field_range: 13:13            field_access: rw  
    field_name: smartcard_en             field_range: 12:12            field_access: rw  
    field_name: hdsel                    field_range: 11:11            field_access: rw  
    field_name: lbcl                     field_range: 10:10            field_access: rw  
    field_name: cpha                     field_range: 9:9              field_access: rw  
    field_name: cpol                     field_range: 8:8              field_access: rw  
    field_name: clk_en                   field_range: 7:7              field_access: rw  
    field_name: bit_length               field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: separate_en              field_range: 2:2              field_access: rw  
    field_name: parity_sel               field_range: 1:1              field_access: rw  
    field_name: parity_en                field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1003301c,0xc0050030);

    /*testing for  SPI_SLAVE: addr=0x10033034*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: dis_nss                  field_range: 1:1              field_access: rw  
    field_name: slv_en                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033034,0x0);

    /*testing for EWR RX_IDLE: addr=0x10033044*/
    /*
    field_name: idle_tout_num            field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10033044,0xffff);

    /*testing for EWR RX_WM: addr=0x10033048*/
    /*
    field_name: wm_tout_num              field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10033048,0xffff);

    /*testing for RO RX_FIFO_LEFT_ENTRY: addr=0x1003304c*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1003304c,0x0);

    /*testing for RO TX_FIFO_LEFT_ENTRY: addr=0x10033050*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x10033050,0x0);

    /*testing for  TX_DATASIZE: addr=0x10033054*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: tx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033054,0x0);

    /*testing for  RX_DATASIZE: addr=0x10033058*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: rx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033058,0x0);

    /*testing for  SMARTCARD_SETUP: addr=0x10033060*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: tx_pue                   field_range: 4:4              field_access: rw  
    field_name: nack_en                  field_range: 3:3              field_access: rw  
    field_name: auto_retry_cnt           field_range: 2:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033060,0x0);

    /*testing for  SMARTCARD_TIMING: addr=0x10033064*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: psc                      field_range: 15:8             field_access: rw  
    field_name: guard_time               field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033064,0x0);

    /*testing for RO IP_VERSION: addr=0x10033068*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x10033068,0x30004);

    /*testing for  ADVANCED_SETUP: addr=0x1003307c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: abr_mod                  field_range: 21:20            field_access: rw  
    field_name: abr_en                   field_range: 19:19            field_access: rw  
    field_name: reserved                 field_range: 18:16            field_access: ro  
    field_name: rtu_en                   field_range: 15:15            field_access: rw  
    field_name: irdalp_en                field_range: 14:14            field_access: rw  
    field_name: irda_en                  field_range: 13:13            field_access: rw  
    field_name: lin_lbdl                 field_range: 12:12            field_access: rw  
    field_name: lin_en                   field_range: 11:11            field_access: rw  
    field_name: add                      field_range: 10:3             field_access: rw  
    field_name: add7                     field_range: 2:2              field_access: rw  
    field_name: wake_mod                 field_range: 1:1              field_access: rw  
    field_name: mm_en                    field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1003307c,0x0);

    /*********************TEST FOR CLOCK DIVISION 1*********************/

    #if defined(__CCM_PRESENT) && __CCM_PRESENT == 1 && __DCACHE_PRESENT == 1
        MFlushInvalDCache();
    #endif
    #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
        DisableICache();
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
        DisableDCache();
    #endif
    __RV_CSR_CLEAR(CSR_MMISC_CTL,MMISC_CTL_BPU);
    start_mcycle1=__RV_CSR_READ(CSR_MCYCLE);

    /*testing for  TXDATA: addr=0x10033000*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: tx_data                  field_range: 8:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033000, 0x0, (0x00000000 & 0x1ff | 0), 0x0, 0x1ff);

    REG_PWR_Double_Check(0x10033000, 0x0, (0xFFFFFFFF & 0x1ff | 0), 0x1ff, 0x1ff);

    REG_PWR_Double_Check(0x10033000, 0x0, (0x55555555 & 0x1ff | 0), 0x155, 0x1ff);

    REG_PWR_Double_Check(0x10033000, 0x0, (0xAAAAAAAA & 0x1ff | 0), 0xaa, 0x1ff);

    /*testing for RO RXDATA: addr=0x10033004*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: rx_data                  field_range: 8:0              field_access: ro  
    */
    REG_RO_Double_Check(0x10033004, 0x0, 0xFFFFFFFF);

    /*testing for  TXCTRL: addr=0x10033008*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: txcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: txen                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033008, 0x0, (0x00000000 & 0x1f0003 | 0), 0x0, 0x1f0003);

    REG_PWR_Double_Check(0x10033008, 0x0, (0xFFFFFFFF & 0x1f0003 | 0), 0x1f0003, 0x1f0003);

    REG_PWR_Double_Check(0x10033008, 0x0, (0x55555555 & 0x1f0003 | 0), 0x150001, 0x1f0003);

    REG_PWR_Double_Check(0x10033008, 0x0, (0xAAAAAAAA & 0x1f0003 | 0), 0xa0002, 0x1f0003);

    /*testing for  RXCTRL: addr=0x1003300c*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: rxcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: rxen                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1003300c, 0x0, (0x00000000 & 0x1f0003 | 0), 0x0, 0x1f0003);

    REG_PWR_Double_Check(0x1003300c, 0x0, (0xFFFFFFFF & 0x1f0003 | 0), 0x1f0003, 0x1f0003);

    REG_PWR_Double_Check(0x1003300c, 0x0, (0x55555555 & 0x1f0003 | 0), 0x150001, 0x1f0003);

    REG_PWR_Double_Check(0x1003300c, 0x0, (0xAAAAAAAA & 0x1f0003 | 0), 0xa0002, 0x1f0003);

    /*testing for  INT_EN: addr=0x10033010*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: modbus_ascii_match_irq_en field_range: 20:20            field_access: rw  
    field_name: modbus_rtu_tout_irq_en   field_range: 19:19            field_access: rw  
    field_name: lin_lbdie_irq_en         field_range: 18:18            field_access: rw  
    field_name: frame_err_irq_en         field_range: 17:17            field_access: rw  
    field_name: rx_fifo_full_irq_en      field_range: 16:16            field_access: rw  
    field_name: rx_fifo_empty_irq_en     field_range: 15:15            field_access: rw  
    field_name: tx_fifo_full_irq_en      field_range: 14:14            field_access: rw  
    field_name: tx_fifo_empty_irq_en     field_range: 13:13            field_access: rw  
    field_name: rx_eot_irq_en            field_range: 12:12            field_access: rw  
    field_name: tx_eot_irq_en            field_range: 11:11            field_access: rw  
    field_name: rx_wm_tout_irq_en        field_range: 10:10            field_access: rw  
    field_name: rx_idle_tout_irq_en      field_range: 9:9              field_access: rw  
    field_name: spi_slv_over_run_irq_en  field_range: 8:8              field_access: rw  
    field_name: spi_slv_under_run_irq_en field_range: 7:7              field_access: rw  
    field_name: stop_bit_irq_en          field_range: 6:6              field_access: rw  
    field_name: ctsie_fall_en            field_range: 5:5              field_access: rw  
    field_name: ctsie_rise_en            field_range: 4:4              field_access: rw  
    field_name: parity_irq_en            field_range: 3:3              field_access: rw  
    field_name: ow_irq_en                field_range: 2:2              field_access: rw  
    field_name: rxie                     field_range: 1:1              field_access: rw  
    field_name: txie                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033010, 0x0, (0x00000000 & 0x1fffff | 0), 0x0, 0x1fffff);

    REG_PWR_Double_Check(0x10033010, 0x0, (0xFFFFFFFF & 0x1fffff | 0), 0x1fffff, 0x1fffff);

    REG_PWR_Double_Check(0x10033010, 0x0, (0x55555555 & 0x1fffff | 0), 0x155555, 0x1fffff);

    REG_PWR_Double_Check(0x10033010, 0x0, (0xAAAAAAAA & 0x1fffff | 0), 0xaaaaa, 0x1fffff);

    /*testing for  STATUS: addr=0x10033014*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: ne_flag                  field_range: 24:24            field_access: w1c 
    field_name: frame_err_flag           field_range: 23:23            field_access: w1c 
    field_name: rx_fifo_full             field_range: 22:22            field_access: ro  
    field_name: tx_fifo_empty            field_range: 21:21            field_access: ro  
    field_name: cts_pin                  field_range: 20:20            field_access: ro  
    field_name: spi_slv_over_run         field_range: 19:19            field_access: w1c 
    field_name: spi_slv_under_run        field_range: 18:18            field_access: w1c 
    field_name: rx_eot                   field_range: 17:17            field_access: w1c 
    field_name: tx_eot                   field_range: 16:16            field_access: w1c 
    field_name: rx_fifo_empty            field_range: 15:15            field_access: ro  
    field_name: tx_fifo_full             field_range: 14:14            field_access: ro  
    field_name: rx_wm_tout_status        field_range: 13:13            field_access: w1c 
    field_name: rx_idle_tout_status      field_range: 12:12            field_access: w1c 
    field_name: reserved                 field_range: 11:9             field_access: ro  
    field_name: stop_bit_err_flag        field_range: 8:8              field_access: w1c 
    field_name: cts_fall_flag            field_range: 7:7              field_access: w1c 
    field_name: cts_rise_flag            field_range: 6:6              field_access: w1c 
    field_name: rx_error_parity          field_range: 5:5              field_access: w1c 
    field_name: rx_error_over_flow       field_range: 4:4              field_access: w1c 
    field_name: rx_busy                  field_range: 3:3              field_access: ro  
    field_name: tx_busy                  field_range: 2:2              field_access: ro  
    field_name: rxip                     field_range: 1:1              field_access: ro  
    field_name: txip                     field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x10033014, 0x208000, (0x00000000 & 0x0 | 2129920), 0x0, 0x0);

    REG_PWR_Double_Check(0x10033014, 0x208000, (0xFFFFFFFF & 0x0 | 2129920), 0x0, 0x0);

    REG_PWR_Double_Check(0x10033014, 0x208000, (0x55555555 & 0x0 | 2129920), 0x0, 0x0);

    REG_PWR_Double_Check(0x10033014, 0x208000, (0xAAAAAAAA & 0x0 | 2129920), 0x0, 0x0);

    /*testing for  DIV: addr=0x10033018*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: baud_div                 field_range: 20:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x10033018, 0x0, (0x00000000 & 0x1fffff | 0), 0x0, 0x1fffff);

    REG_PWR_Double_Check(0x10033018, 0x0, (0xFFFFFFFF & 0x1fffff | 0), 0x1fffff, 0x1fffff);

    REG_PWR_Double_Check(0x10033018, 0x0, (0x55555555 & 0x1fffff | 0), 0x155555, 0x1fffff);

    REG_PWR_Double_Check(0x10033018, 0x0, (0xAAAAAAAA & 0x1fffff | 0), 0xaaaaa, 0x1fffff);

    /*testing for  SETUP: addr=0x1003301c*/
    /*
    field_name: cfg_stop_bit             field_range: 31:30            field_access: rw  
    field_name: swap                     field_range: 29:29            field_access: rw  
    field_name: rxinv                    field_range: 28:28            field_access: rw  
    field_name: txinv                    field_range: 27:27            field_access: rw  
    field_name: datainv                  field_range: 26:26            field_access: rw  
    field_name: msbfirst                 field_range: 25:25            field_access: rw  
    field_name: de_inv                   field_range: 24:24            field_access: rw  
    field_name: de_deassertion_delay_en  field_range: 23:23            field_access: rw  
    field_name: de_assertion_early_en    field_range: 22:22            field_access: rw  
    field_name: rs485_en                 field_range: 21:21            field_access: rw  
    field_name: rx_dma_en                field_range: 20:20            field_access: rw  
    field_name: fraction_baud            field_range: 19:19            field_access: rw  
    field_name: sample_position          field_range: 18:17            field_access: rw  
    field_name: vote_majority_en         field_range: 16:16            field_access: rw  
    field_name: dma_en                   field_range: 15:15            field_access: rw  
    field_name: ctse                     field_range: 14:14            field_access: rw  
    field_name: rtse                     field_range: 13:13            field_access: rw  
    field_name: smartcard_en             field_range: 12:12            field_access: rw  
    field_name: hdsel                    field_range: 11:11            field_access: rw  
    field_name: lbcl                     field_range: 10:10            field_access: rw  
    field_name: cpha                     field_range: 9:9              field_access: rw  
    field_name: cpol                     field_range: 8:8              field_access: rw  
    field_name: clk_en                   field_range: 7:7              field_access: rw  
    field_name: bit_length               field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: separate_en              field_range: 2:2              field_access: rw  
    field_name: parity_sel               field_range: 1:1              field_access: rw  
    field_name: parity_en                field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0x00000000 & 0xffeffff7 | 0), 0x0, 0xffeffff7);

    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0xFFFFFFFF & 0xffeffff7 | 0), 0xffeffff7, 0xffeffff7);

    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0x55555555 & 0xffeffff7 | 0), 0x55455555, 0xffeffff7);

    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0xAAAAAAAA & 0xffeffff7 | 0), 0xaaaaaaa2, 0xffeffff7);

    /*testing for  SPI_SLAVE: addr=0x10033034*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: dis_nss                  field_range: 1:1              field_access: rw  
    field_name: slv_en                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033034, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x10033034, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x10033034, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x10033034, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for EWR RX_IDLE: addr=0x10033044*/
    /*
    field_name: idle_tout_num            field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10033044, 0xffff);

    /*testing for EWR RX_WM: addr=0x10033048*/
    /*
    field_name: wm_tout_num              field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10033048, 0xffff);

    /*testing for RO RX_FIFO_LEFT_ENTRY: addr=0x1003304c*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_RO_Double_Check(0x1003304c, 0x0, 0xFFFFFFFF);

    /*testing for RO TX_FIFO_LEFT_ENTRY: addr=0x10033050*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_RO_Double_Check(0x10033050, 0x0, 0xFFFFFFFF);

    /*testing for  TX_DATASIZE: addr=0x10033054*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: tx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033054, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x10033054, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x10033054, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x10033054, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for  RX_DATASIZE: addr=0x10033058*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: rx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033058, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x10033058, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x10033058, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x10033058, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for  SMARTCARD_SETUP: addr=0x10033060*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: tx_pue                   field_range: 4:4              field_access: rw  
    field_name: nack_en                  field_range: 3:3              field_access: rw  
    field_name: auto_retry_cnt           field_range: 2:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033060, 0x0, (0x00000000 & 0x1f | 0), 0x0, 0x1f);

    REG_PWR_Double_Check(0x10033060, 0x0, (0xFFFFFFFF & 0x1f | 0), 0x1f, 0x1f);

    REG_PWR_Double_Check(0x10033060, 0x0, (0x55555555 & 0x1f | 0), 0x15, 0x1f);

    REG_PWR_Double_Check(0x10033060, 0x0, (0xAAAAAAAA & 0x1f | 0), 0xa, 0x1f);

    /*testing for  SMARTCARD_TIMING: addr=0x10033064*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: psc                      field_range: 15:8             field_access: rw  
    field_name: guard_time               field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033064, 0x0, (0x00000000 & 0xffff | 0), 0x0, 0xffff);

    REG_PWR_Double_Check(0x10033064, 0x0, (0xFFFFFFFF & 0xffff | 0), 0xffff, 0xffff);

    REG_PWR_Double_Check(0x10033064, 0x0, (0x55555555 & 0xffff | 0), 0x5555, 0xffff);

    REG_PWR_Double_Check(0x10033064, 0x0, (0xAAAAAAAA & 0xffff | 0), 0xaaaa, 0xffff);

    /*testing for RO IP_VERSION: addr=0x10033068*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x10033068, 0x30004, 0xFFFFFFFF);

    /*testing for  ADVANCED_SETUP: addr=0x1003307c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: abr_mod                  field_range: 21:20            field_access: rw  
    field_name: abr_en                   field_range: 19:19            field_access: rw  
    field_name: reserved                 field_range: 18:16            field_access: ro  
    field_name: rtu_en                   field_range: 15:15            field_access: rw  
    field_name: irdalp_en                field_range: 14:14            field_access: rw  
    field_name: irda_en                  field_range: 13:13            field_access: rw  
    field_name: lin_lbdl                 field_range: 12:12            field_access: rw  
    field_name: lin_en                   field_range: 11:11            field_access: rw  
    field_name: add                      field_range: 10:3             field_access: rw  
    field_name: add7                     field_range: 2:2              field_access: rw  
    field_name: wake_mod                 field_range: 1:1              field_access: rw  
    field_name: mm_en                    field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1003307c, 0x0, (0x00000000 & 0x38ffff | 0), 0x0, 0x38ffff);

    REG_PWR_Double_Check(0x1003307c, 0x0, (0xFFFFFFFF & 0x38ffff | 0), 0x38ffff, 0x38ffff);

    REG_PWR_Double_Check(0x1003307c, 0x0, (0x55555555 & 0x38ffff | 0), 0x105555, 0x38ffff);

    REG_PWR_Double_Check(0x1003307c, 0x0, (0xAAAAAAAA & 0x38ffff | 0), 0x28aaaa, 0x38ffff);

    delta_mcycle1=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle1;

    /*********************TEST FOR CLOCK DIVISION 36*********************/

    #ifdef MISC_HAS_USART3_CLK_DIV

    usart3_clk_div(35);

    start_mcycle2=__RV_CSR_READ(CSR_MCYCLE);

    /*testing for  TXDATA: addr=0x10033000*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: tx_data                  field_range: 8:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033000, 0x0, (0x00000000 & 0x1ff | 0), 0x0, 0x1ff);

    REG_PWR_Double_Check(0x10033000, 0x0, (0xFFFFFFFF & 0x1ff | 0), 0x1ff, 0x1ff);

    REG_PWR_Double_Check(0x10033000, 0x0, (0x55555555 & 0x1ff | 0), 0x155, 0x1ff);

    REG_PWR_Double_Check(0x10033000, 0x0, (0xAAAAAAAA & 0x1ff | 0), 0xaa, 0x1ff);

    /*testing for RO RXDATA: addr=0x10033004*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: rx_data                  field_range: 8:0              field_access: ro  
    */
    REG_RO_Double_Check(0x10033004, 0x0, 0xFFFFFFFF);

    /*testing for  TXCTRL: addr=0x10033008*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: txcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: txen                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033008, 0x0, (0x00000000 & 0x1f0003 | 0), 0x0, 0x1f0003);

    REG_PWR_Double_Check(0x10033008, 0x0, (0xFFFFFFFF & 0x1f0003 | 0), 0x1f0003, 0x1f0003);

    REG_PWR_Double_Check(0x10033008, 0x0, (0x55555555 & 0x1f0003 | 0), 0x150001, 0x1f0003);

    REG_PWR_Double_Check(0x10033008, 0x0, (0xAAAAAAAA & 0x1f0003 | 0), 0xa0002, 0x1f0003);

    /*testing for  RXCTRL: addr=0x1003300c*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: rxcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: rxen                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1003300c, 0x0, (0x00000000 & 0x1f0003 | 0), 0x0, 0x1f0003);

    REG_PWR_Double_Check(0x1003300c, 0x0, (0xFFFFFFFF & 0x1f0003 | 0), 0x1f0003, 0x1f0003);

    REG_PWR_Double_Check(0x1003300c, 0x0, (0x55555555 & 0x1f0003 | 0), 0x150001, 0x1f0003);

    REG_PWR_Double_Check(0x1003300c, 0x0, (0xAAAAAAAA & 0x1f0003 | 0), 0xa0002, 0x1f0003);

    /*testing for  INT_EN: addr=0x10033010*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: modbus_ascii_match_irq_en field_range: 20:20            field_access: rw  
    field_name: modbus_rtu_tout_irq_en   field_range: 19:19            field_access: rw  
    field_name: lin_lbdie_irq_en         field_range: 18:18            field_access: rw  
    field_name: frame_err_irq_en         field_range: 17:17            field_access: rw  
    field_name: rx_fifo_full_irq_en      field_range: 16:16            field_access: rw  
    field_name: rx_fifo_empty_irq_en     field_range: 15:15            field_access: rw  
    field_name: tx_fifo_full_irq_en      field_range: 14:14            field_access: rw  
    field_name: tx_fifo_empty_irq_en     field_range: 13:13            field_access: rw  
    field_name: rx_eot_irq_en            field_range: 12:12            field_access: rw  
    field_name: tx_eot_irq_en            field_range: 11:11            field_access: rw  
    field_name: rx_wm_tout_irq_en        field_range: 10:10            field_access: rw  
    field_name: rx_idle_tout_irq_en      field_range: 9:9              field_access: rw  
    field_name: spi_slv_over_run_irq_en  field_range: 8:8              field_access: rw  
    field_name: spi_slv_under_run_irq_en field_range: 7:7              field_access: rw  
    field_name: stop_bit_irq_en          field_range: 6:6              field_access: rw  
    field_name: ctsie_fall_en            field_range: 5:5              field_access: rw  
    field_name: ctsie_rise_en            field_range: 4:4              field_access: rw  
    field_name: parity_irq_en            field_range: 3:3              field_access: rw  
    field_name: ow_irq_en                field_range: 2:2              field_access: rw  
    field_name: rxie                     field_range: 1:1              field_access: rw  
    field_name: txie                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033010, 0x0, (0x00000000 & 0x1fffff | 0), 0x0, 0x1fffff);

    REG_PWR_Double_Check(0x10033010, 0x0, (0xFFFFFFFF & 0x1fffff | 0), 0x1fffff, 0x1fffff);

    REG_PWR_Double_Check(0x10033010, 0x0, (0x55555555 & 0x1fffff | 0), 0x155555, 0x1fffff);

    REG_PWR_Double_Check(0x10033010, 0x0, (0xAAAAAAAA & 0x1fffff | 0), 0xaaaaa, 0x1fffff);

    /*testing for  STATUS: addr=0x10033014*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: ne_flag                  field_range: 24:24            field_access: w1c 
    field_name: frame_err_flag           field_range: 23:23            field_access: w1c 
    field_name: rx_fifo_full             field_range: 22:22            field_access: ro  
    field_name: tx_fifo_empty            field_range: 21:21            field_access: ro  
    field_name: cts_pin                  field_range: 20:20            field_access: ro  
    field_name: spi_slv_over_run         field_range: 19:19            field_access: w1c 
    field_name: spi_slv_under_run        field_range: 18:18            field_access: w1c 
    field_name: rx_eot                   field_range: 17:17            field_access: w1c 
    field_name: tx_eot                   field_range: 16:16            field_access: w1c 
    field_name: rx_fifo_empty            field_range: 15:15            field_access: ro  
    field_name: tx_fifo_full             field_range: 14:14            field_access: ro  
    field_name: rx_wm_tout_status        field_range: 13:13            field_access: w1c 
    field_name: rx_idle_tout_status      field_range: 12:12            field_access: w1c 
    field_name: reserved                 field_range: 11:9             field_access: ro  
    field_name: stop_bit_err_flag        field_range: 8:8              field_access: w1c 
    field_name: cts_fall_flag            field_range: 7:7              field_access: w1c 
    field_name: cts_rise_flag            field_range: 6:6              field_access: w1c 
    field_name: rx_error_parity          field_range: 5:5              field_access: w1c 
    field_name: rx_error_over_flow       field_range: 4:4              field_access: w1c 
    field_name: rx_busy                  field_range: 3:3              field_access: ro  
    field_name: tx_busy                  field_range: 2:2              field_access: ro  
    field_name: rxip                     field_range: 1:1              field_access: ro  
    field_name: txip                     field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x10033014, 0x208000, (0x00000000 & 0x0 | 2129920), 0x0, 0x0);

    REG_PWR_Double_Check(0x10033014, 0x208000, (0xFFFFFFFF & 0x0 | 2129920), 0x0, 0x0);

    REG_PWR_Double_Check(0x10033014, 0x208000, (0x55555555 & 0x0 | 2129920), 0x0, 0x0);

    REG_PWR_Double_Check(0x10033014, 0x208000, (0xAAAAAAAA & 0x0 | 2129920), 0x0, 0x0);

    /*testing for  DIV: addr=0x10033018*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: baud_div                 field_range: 20:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x10033018, 0x0, (0x00000000 & 0x1fffff | 0), 0x0, 0x1fffff);

    REG_PWR_Double_Check(0x10033018, 0x0, (0xFFFFFFFF & 0x1fffff | 0), 0x1fffff, 0x1fffff);

    REG_PWR_Double_Check(0x10033018, 0x0, (0x55555555 & 0x1fffff | 0), 0x155555, 0x1fffff);

    REG_PWR_Double_Check(0x10033018, 0x0, (0xAAAAAAAA & 0x1fffff | 0), 0xaaaaa, 0x1fffff);

    /*testing for  SETUP: addr=0x1003301c*/
    /*
    field_name: cfg_stop_bit             field_range: 31:30            field_access: rw  
    field_name: swap                     field_range: 29:29            field_access: rw  
    field_name: rxinv                    field_range: 28:28            field_access: rw  
    field_name: txinv                    field_range: 27:27            field_access: rw  
    field_name: datainv                  field_range: 26:26            field_access: rw  
    field_name: msbfirst                 field_range: 25:25            field_access: rw  
    field_name: de_inv                   field_range: 24:24            field_access: rw  
    field_name: de_deassertion_delay_en  field_range: 23:23            field_access: rw  
    field_name: de_assertion_early_en    field_range: 22:22            field_access: rw  
    field_name: rs485_en                 field_range: 21:21            field_access: rw  
    field_name: rx_dma_en                field_range: 20:20            field_access: rw  
    field_name: fraction_baud            field_range: 19:19            field_access: rw  
    field_name: sample_position          field_range: 18:17            field_access: rw  
    field_name: vote_majority_en         field_range: 16:16            field_access: rw  
    field_name: dma_en                   field_range: 15:15            field_access: rw  
    field_name: ctse                     field_range: 14:14            field_access: rw  
    field_name: rtse                     field_range: 13:13            field_access: rw  
    field_name: smartcard_en             field_range: 12:12            field_access: rw  
    field_name: hdsel                    field_range: 11:11            field_access: rw  
    field_name: lbcl                     field_range: 10:10            field_access: rw  
    field_name: cpha                     field_range: 9:9              field_access: rw  
    field_name: cpol                     field_range: 8:8              field_access: rw  
    field_name: clk_en                   field_range: 7:7              field_access: rw  
    field_name: bit_length               field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: separate_en              field_range: 2:2              field_access: rw  
    field_name: parity_sel               field_range: 1:1              field_access: rw  
    field_name: parity_en                field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0x00000000 & 0xffeffff7 | 0), 0x0, 0xffeffff7);

    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0xFFFFFFFF & 0xffeffff7 | 0), 0xffeffff7, 0xffeffff7);

    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0x55555555 & 0xffeffff7 | 0), 0x55455555, 0xffeffff7);

    REG_PWR_Double_Check(0x1003301c, 0xc0050030, (0xAAAAAAAA & 0xffeffff7 | 0), 0xaaaaaaa2, 0xffeffff7);

    /*testing for  SPI_SLAVE: addr=0x10033034*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: dis_nss                  field_range: 1:1              field_access: rw  
    field_name: slv_en                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033034, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x10033034, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x10033034, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x10033034, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for EWR RX_IDLE: addr=0x10033044*/
    /*
    field_name: idle_tout_num            field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10033044, 0xffff);

    /*testing for EWR RX_WM: addr=0x10033048*/
    /*
    field_name: wm_tout_num              field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10033048, 0xffff);

    /*testing for RO RX_FIFO_LEFT_ENTRY: addr=0x1003304c*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_RO_Double_Check(0x1003304c, 0x0, 0xFFFFFFFF);

    /*testing for RO TX_FIFO_LEFT_ENTRY: addr=0x10033050*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_RO_Double_Check(0x10033050, 0x0, 0xFFFFFFFF);

    /*testing for  TX_DATASIZE: addr=0x10033054*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: tx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033054, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x10033054, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x10033054, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x10033054, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for  RX_DATASIZE: addr=0x10033058*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: rx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033058, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x10033058, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x10033058, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x10033058, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for  SMARTCARD_SETUP: addr=0x10033060*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: tx_pue                   field_range: 4:4              field_access: rw  
    field_name: nack_en                  field_range: 3:3              field_access: rw  
    field_name: auto_retry_cnt           field_range: 2:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033060, 0x0, (0x00000000 & 0x1f | 0), 0x0, 0x1f);

    REG_PWR_Double_Check(0x10033060, 0x0, (0xFFFFFFFF & 0x1f | 0), 0x1f, 0x1f);

    REG_PWR_Double_Check(0x10033060, 0x0, (0x55555555 & 0x1f | 0), 0x15, 0x1f);

    REG_PWR_Double_Check(0x10033060, 0x0, (0xAAAAAAAA & 0x1f | 0), 0xa, 0x1f);

    /*testing for  SMARTCARD_TIMING: addr=0x10033064*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: psc                      field_range: 15:8             field_access: rw  
    field_name: guard_time               field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10033064, 0x0, (0x00000000 & 0xffff | 0), 0x0, 0xffff);

    REG_PWR_Double_Check(0x10033064, 0x0, (0xFFFFFFFF & 0xffff | 0), 0xffff, 0xffff);

    REG_PWR_Double_Check(0x10033064, 0x0, (0x55555555 & 0xffff | 0), 0x5555, 0xffff);

    REG_PWR_Double_Check(0x10033064, 0x0, (0xAAAAAAAA & 0xffff | 0), 0xaaaa, 0xffff);

    /*testing for RO IP_VERSION: addr=0x10033068*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x10033068, 0x30004, 0xFFFFFFFF);

    /*testing for  ADVANCED_SETUP: addr=0x1003307c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: abr_mod                  field_range: 21:20            field_access: rw  
    field_name: abr_en                   field_range: 19:19            field_access: rw  
    field_name: reserved                 field_range: 18:16            field_access: ro  
    field_name: rtu_en                   field_range: 15:15            field_access: rw  
    field_name: irdalp_en                field_range: 14:14            field_access: rw  
    field_name: irda_en                  field_range: 13:13            field_access: rw  
    field_name: lin_lbdl                 field_range: 12:12            field_access: rw  
    field_name: lin_en                   field_range: 11:11            field_access: rw  
    field_name: add                      field_range: 10:3             field_access: rw  
    field_name: add7                     field_range: 2:2              field_access: rw  
    field_name: wake_mod                 field_range: 1:1              field_access: rw  
    field_name: mm_en                    field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1003307c, 0x0, (0x00000000 & 0x38ffff | 0), 0x0, 0x38ffff);

    REG_PWR_Double_Check(0x1003307c, 0x0, (0xFFFFFFFF & 0x38ffff | 0), 0x38ffff, 0x38ffff);

    REG_PWR_Double_Check(0x1003307c, 0x0, (0x55555555 & 0x38ffff | 0), 0x105555, 0x38ffff);

    REG_PWR_Double_Check(0x1003307c, 0x0, (0xAAAAAAAA & 0x38ffff | 0), 0x28aaaa, 0x38ffff);

    delta_mcycle2=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle2;

    usart3_clk_div(0);

    #endif

    /*********************TEST FOR CLOCK DISABLE*********************/

    #ifndef __HAS_SCRATCHPAD_MODE 
        #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
            EnableICache();
        #endif
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
        EnableDCache();
    #endif
    __RV_CSR_SET(CSR_MMISC_CTL,MMISC_CTL_BPU);
    #ifdef MISC_HAS_USART3_HAS_CLK

    usart3_clk_en(DISABLE);

    /*testing for  TXDATA: addr=0x10033000*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: tx_data                  field_range: 8:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033000);

    /*testing for RO RXDATA: addr=0x10033004*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: rx_data                  field_range: 8:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033004);

    /*testing for  TXCTRL: addr=0x10033008*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: txcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: txen                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033008);

    /*testing for  RXCTRL: addr=0x1003300c*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: rxcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: rxen                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1003300c);

    /*testing for  INT_EN: addr=0x10033010*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: modbus_ascii_match_irq_en field_range: 20:20            field_access: rw  
    field_name: modbus_rtu_tout_irq_en   field_range: 19:19            field_access: rw  
    field_name: lin_lbdie_irq_en         field_range: 18:18            field_access: rw  
    field_name: frame_err_irq_en         field_range: 17:17            field_access: rw  
    field_name: rx_fifo_full_irq_en      field_range: 16:16            field_access: rw  
    field_name: rx_fifo_empty_irq_en     field_range: 15:15            field_access: rw  
    field_name: tx_fifo_full_irq_en      field_range: 14:14            field_access: rw  
    field_name: tx_fifo_empty_irq_en     field_range: 13:13            field_access: rw  
    field_name: rx_eot_irq_en            field_range: 12:12            field_access: rw  
    field_name: tx_eot_irq_en            field_range: 11:11            field_access: rw  
    field_name: rx_wm_tout_irq_en        field_range: 10:10            field_access: rw  
    field_name: rx_idle_tout_irq_en      field_range: 9:9              field_access: rw  
    field_name: spi_slv_over_run_irq_en  field_range: 8:8              field_access: rw  
    field_name: spi_slv_under_run_irq_en field_range: 7:7              field_access: rw  
    field_name: stop_bit_irq_en          field_range: 6:6              field_access: rw  
    field_name: ctsie_fall_en            field_range: 5:5              field_access: rw  
    field_name: ctsie_rise_en            field_range: 4:4              field_access: rw  
    field_name: parity_irq_en            field_range: 3:3              field_access: rw  
    field_name: ow_irq_en                field_range: 2:2              field_access: rw  
    field_name: rxie                     field_range: 1:1              field_access: rw  
    field_name: txie                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033010);

    /*testing for  STATUS: addr=0x10033014*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: ne_flag                  field_range: 24:24            field_access: w1c 
    field_name: frame_err_flag           field_range: 23:23            field_access: w1c 
    field_name: rx_fifo_full             field_range: 22:22            field_access: ro  
    field_name: tx_fifo_empty            field_range: 21:21            field_access: ro  
    field_name: cts_pin                  field_range: 20:20            field_access: ro  
    field_name: spi_slv_over_run         field_range: 19:19            field_access: w1c 
    field_name: spi_slv_under_run        field_range: 18:18            field_access: w1c 
    field_name: rx_eot                   field_range: 17:17            field_access: w1c 
    field_name: tx_eot                   field_range: 16:16            field_access: w1c 
    field_name: rx_fifo_empty            field_range: 15:15            field_access: ro  
    field_name: tx_fifo_full             field_range: 14:14            field_access: ro  
    field_name: rx_wm_tout_status        field_range: 13:13            field_access: w1c 
    field_name: rx_idle_tout_status      field_range: 12:12            field_access: w1c 
    field_name: reserved                 field_range: 11:9             field_access: ro  
    field_name: stop_bit_err_flag        field_range: 8:8              field_access: w1c 
    field_name: cts_fall_flag            field_range: 7:7              field_access: w1c 
    field_name: cts_rise_flag            field_range: 6:6              field_access: w1c 
    field_name: rx_error_parity          field_range: 5:5              field_access: w1c 
    field_name: rx_error_over_flow       field_range: 4:4              field_access: w1c 
    field_name: rx_busy                  field_range: 3:3              field_access: ro  
    field_name: tx_busy                  field_range: 2:2              field_access: ro  
    field_name: rxip                     field_range: 1:1              field_access: ro  
    field_name: txip                     field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033014);

    /*testing for  DIV: addr=0x10033018*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: baud_div                 field_range: 20:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033018);

    /*testing for  SETUP: addr=0x1003301c*/
    /*
    field_name: cfg_stop_bit             field_range: 31:30            field_access: rw  
    field_name: swap                     field_range: 29:29            field_access: rw  
    field_name: rxinv                    field_range: 28:28            field_access: rw  
    field_name: txinv                    field_range: 27:27            field_access: rw  
    field_name: datainv                  field_range: 26:26            field_access: rw  
    field_name: msbfirst                 field_range: 25:25            field_access: rw  
    field_name: de_inv                   field_range: 24:24            field_access: rw  
    field_name: de_deassertion_delay_en  field_range: 23:23            field_access: rw  
    field_name: de_assertion_early_en    field_range: 22:22            field_access: rw  
    field_name: rs485_en                 field_range: 21:21            field_access: rw  
    field_name: rx_dma_en                field_range: 20:20            field_access: rw  
    field_name: fraction_baud            field_range: 19:19            field_access: rw  
    field_name: sample_position          field_range: 18:17            field_access: rw  
    field_name: vote_majority_en         field_range: 16:16            field_access: rw  
    field_name: dma_en                   field_range: 15:15            field_access: rw  
    field_name: ctse                     field_range: 14:14            field_access: rw  
    field_name: rtse                     field_range: 13:13            field_access: rw  
    field_name: smartcard_en             field_range: 12:12            field_access: rw  
    field_name: hdsel                    field_range: 11:11            field_access: rw  
    field_name: lbcl                     field_range: 10:10            field_access: rw  
    field_name: cpha                     field_range: 9:9              field_access: rw  
    field_name: cpol                     field_range: 8:8              field_access: rw  
    field_name: clk_en                   field_range: 7:7              field_access: rw  
    field_name: bit_length               field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: separate_en              field_range: 2:2              field_access: rw  
    field_name: parity_sel               field_range: 1:1              field_access: rw  
    field_name: parity_en                field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1003301c);

    /*testing for  SPI_SLAVE: addr=0x10033034*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: dis_nss                  field_range: 1:1              field_access: rw  
    field_name: slv_en                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033034);

    /*testing for EWR RX_IDLE: addr=0x10033044*/
    /*
    field_name: idle_tout_num            field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033044);

    /*testing for EWR RX_WM: addr=0x10033048*/
    /*
    field_name: wm_tout_num              field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033048);

    /*testing for RO RX_FIFO_LEFT_ENTRY: addr=0x1003304c*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1003304c);

    /*testing for RO TX_FIFO_LEFT_ENTRY: addr=0x10033050*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033050);

    /*testing for  TX_DATASIZE: addr=0x10033054*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: tx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033054);

    /*testing for  RX_DATASIZE: addr=0x10033058*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: rx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033058);

    /*testing for  SMARTCARD_SETUP: addr=0x10033060*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: tx_pue                   field_range: 4:4              field_access: rw  
    field_name: nack_en                  field_range: 3:3              field_access: rw  
    field_name: auto_retry_cnt           field_range: 2:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033060);

    /*testing for  SMARTCARD_TIMING: addr=0x10033064*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: psc                      field_range: 15:8             field_access: rw  
    field_name: guard_time               field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033064);

    /*testing for RO IP_VERSION: addr=0x10033068*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033068);

    /*testing for  ADVANCED_SETUP: addr=0x1003307c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: abr_mod                  field_range: 21:20            field_access: rw  
    field_name: abr_en                   field_range: 19:19            field_access: rw  
    field_name: reserved                 field_range: 18:16            field_access: ro  
    field_name: rtu_en                   field_range: 15:15            field_access: rw  
    field_name: irdalp_en                field_range: 14:14            field_access: rw  
    field_name: irda_en                  field_range: 13:13            field_access: rw  
    field_name: lin_lbdl                 field_range: 12:12            field_access: rw  
    field_name: lin_en                   field_range: 11:11            field_access: rw  
    field_name: add                      field_range: 10:3             field_access: rw  
    field_name: add7                     field_range: 2:2              field_access: rw  
    field_name: wake_mod                 field_range: 1:1              field_access: rw  
    field_name: mm_en                    field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1003307c);

    usart3_clk_en(ENABLE);

    #endif

    /*********************TEST FOR CLOCK RESET*********************/

    #ifdef MISC_HAS_USART3_RST

    usart3_set_rst(DISABLE);

    /*testing for  TXDATA: addr=0x10033000*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: tx_data                  field_range: 8:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033000);

    /*testing for RO RXDATA: addr=0x10033004*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: rx_data                  field_range: 8:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033004);

    /*testing for  TXCTRL: addr=0x10033008*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: txcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: txen                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033008);

    /*testing for  RXCTRL: addr=0x1003300c*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: rxcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: rxen                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1003300c);

    /*testing for  INT_EN: addr=0x10033010*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: modbus_ascii_match_irq_en field_range: 20:20            field_access: rw  
    field_name: modbus_rtu_tout_irq_en   field_range: 19:19            field_access: rw  
    field_name: lin_lbdie_irq_en         field_range: 18:18            field_access: rw  
    field_name: frame_err_irq_en         field_range: 17:17            field_access: rw  
    field_name: rx_fifo_full_irq_en      field_range: 16:16            field_access: rw  
    field_name: rx_fifo_empty_irq_en     field_range: 15:15            field_access: rw  
    field_name: tx_fifo_full_irq_en      field_range: 14:14            field_access: rw  
    field_name: tx_fifo_empty_irq_en     field_range: 13:13            field_access: rw  
    field_name: rx_eot_irq_en            field_range: 12:12            field_access: rw  
    field_name: tx_eot_irq_en            field_range: 11:11            field_access: rw  
    field_name: rx_wm_tout_irq_en        field_range: 10:10            field_access: rw  
    field_name: rx_idle_tout_irq_en      field_range: 9:9              field_access: rw  
    field_name: spi_slv_over_run_irq_en  field_range: 8:8              field_access: rw  
    field_name: spi_slv_under_run_irq_en field_range: 7:7              field_access: rw  
    field_name: stop_bit_irq_en          field_range: 6:6              field_access: rw  
    field_name: ctsie_fall_en            field_range: 5:5              field_access: rw  
    field_name: ctsie_rise_en            field_range: 4:4              field_access: rw  
    field_name: parity_irq_en            field_range: 3:3              field_access: rw  
    field_name: ow_irq_en                field_range: 2:2              field_access: rw  
    field_name: rxie                     field_range: 1:1              field_access: rw  
    field_name: txie                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033010);

    /*testing for  STATUS: addr=0x10033014*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: ne_flag                  field_range: 24:24            field_access: w1c 
    field_name: frame_err_flag           field_range: 23:23            field_access: w1c 
    field_name: rx_fifo_full             field_range: 22:22            field_access: ro  
    field_name: tx_fifo_empty            field_range: 21:21            field_access: ro  
    field_name: cts_pin                  field_range: 20:20            field_access: ro  
    field_name: spi_slv_over_run         field_range: 19:19            field_access: w1c 
    field_name: spi_slv_under_run        field_range: 18:18            field_access: w1c 
    field_name: rx_eot                   field_range: 17:17            field_access: w1c 
    field_name: tx_eot                   field_range: 16:16            field_access: w1c 
    field_name: rx_fifo_empty            field_range: 15:15            field_access: ro  
    field_name: tx_fifo_full             field_range: 14:14            field_access: ro  
    field_name: rx_wm_tout_status        field_range: 13:13            field_access: w1c 
    field_name: rx_idle_tout_status      field_range: 12:12            field_access: w1c 
    field_name: reserved                 field_range: 11:9             field_access: ro  
    field_name: stop_bit_err_flag        field_range: 8:8              field_access: w1c 
    field_name: cts_fall_flag            field_range: 7:7              field_access: w1c 
    field_name: cts_rise_flag            field_range: 6:6              field_access: w1c 
    field_name: rx_error_parity          field_range: 5:5              field_access: w1c 
    field_name: rx_error_over_flow       field_range: 4:4              field_access: w1c 
    field_name: rx_busy                  field_range: 3:3              field_access: ro  
    field_name: tx_busy                  field_range: 2:2              field_access: ro  
    field_name: rxip                     field_range: 1:1              field_access: ro  
    field_name: txip                     field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033014);

    /*testing for  DIV: addr=0x10033018*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: baud_div                 field_range: 20:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033018);

    /*testing for  SETUP: addr=0x1003301c*/
    /*
    field_name: cfg_stop_bit             field_range: 31:30            field_access: rw  
    field_name: swap                     field_range: 29:29            field_access: rw  
    field_name: rxinv                    field_range: 28:28            field_access: rw  
    field_name: txinv                    field_range: 27:27            field_access: rw  
    field_name: datainv                  field_range: 26:26            field_access: rw  
    field_name: msbfirst                 field_range: 25:25            field_access: rw  
    field_name: de_inv                   field_range: 24:24            field_access: rw  
    field_name: de_deassertion_delay_en  field_range: 23:23            field_access: rw  
    field_name: de_assertion_early_en    field_range: 22:22            field_access: rw  
    field_name: rs485_en                 field_range: 21:21            field_access: rw  
    field_name: rx_dma_en                field_range: 20:20            field_access: rw  
    field_name: fraction_baud            field_range: 19:19            field_access: rw  
    field_name: sample_position          field_range: 18:17            field_access: rw  
    field_name: vote_majority_en         field_range: 16:16            field_access: rw  
    field_name: dma_en                   field_range: 15:15            field_access: rw  
    field_name: ctse                     field_range: 14:14            field_access: rw  
    field_name: rtse                     field_range: 13:13            field_access: rw  
    field_name: smartcard_en             field_range: 12:12            field_access: rw  
    field_name: hdsel                    field_range: 11:11            field_access: rw  
    field_name: lbcl                     field_range: 10:10            field_access: rw  
    field_name: cpha                     field_range: 9:9              field_access: rw  
    field_name: cpol                     field_range: 8:8              field_access: rw  
    field_name: clk_en                   field_range: 7:7              field_access: rw  
    field_name: bit_length               field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: separate_en              field_range: 2:2              field_access: rw  
    field_name: parity_sel               field_range: 1:1              field_access: rw  
    field_name: parity_en                field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1003301c);

    /*testing for  SPI_SLAVE: addr=0x10033034*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: dis_nss                  field_range: 1:1              field_access: rw  
    field_name: slv_en                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033034);

    /*testing for EWR RX_IDLE: addr=0x10033044*/
    /*
    field_name: idle_tout_num            field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033044);

    /*testing for EWR RX_WM: addr=0x10033048*/
    /*
    field_name: wm_tout_num              field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033048);

    /*testing for RO RX_FIFO_LEFT_ENTRY: addr=0x1003304c*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1003304c);

    /*testing for RO TX_FIFO_LEFT_ENTRY: addr=0x10033050*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033050);

    /*testing for  TX_DATASIZE: addr=0x10033054*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: tx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033054);

    /*testing for  RX_DATASIZE: addr=0x10033058*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: rx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033058);

    /*testing for  SMARTCARD_SETUP: addr=0x10033060*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: tx_pue                   field_range: 4:4              field_access: rw  
    field_name: nack_en                  field_range: 3:3              field_access: rw  
    field_name: auto_retry_cnt           field_range: 2:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033060);

    /*testing for  SMARTCARD_TIMING: addr=0x10033064*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: psc                      field_range: 15:8             field_access: rw  
    field_name: guard_time               field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10033064);

    /*testing for RO IP_VERSION: addr=0x10033068*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10033068);

    /*testing for  ADVANCED_SETUP: addr=0x1003307c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: abr_mod                  field_range: 21:20            field_access: rw  
    field_name: abr_en                   field_range: 19:19            field_access: rw  
    field_name: reserved                 field_range: 18:16            field_access: ro  
    field_name: rtu_en                   field_range: 15:15            field_access: rw  
    field_name: irdalp_en                field_range: 14:14            field_access: rw  
    field_name: irda_en                  field_range: 13:13            field_access: rw  
    field_name: lin_lbdl                 field_range: 12:12            field_access: rw  
    field_name: lin_en                   field_range: 11:11            field_access: rw  
    field_name: add                      field_range: 10:3             field_access: rw  
    field_name: add7                     field_range: 2:2              field_access: rw  
    field_name: wake_mod                 field_range: 1:1              field_access: rw  
    field_name: mm_en                    field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1003307c);

    usart3_set_rst(ENABLE);

    /*testing for  TXDATA: addr=0x10033000*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: tx_data                  field_range: 8:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033000,0x0);

    /*testing for RO RXDATA: addr=0x10033004*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: rx_data                  field_range: 8:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x10033004,0x0);

    /*testing for  TXCTRL: addr=0x10033008*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: txcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: txen                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033008,0x0);

    /*testing for  RXCTRL: addr=0x1003300c*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: rxcnt                    field_range: 20:16            field_access: rw  
    field_name: reserved                 field_range: 15:3             field_access: ro  
    field_name: clr                      field_range: 2:2              field_access: wo  
    field_name: continuous               field_range: 1:1              field_access: rw  
    field_name: rxen                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1003300c,0x0);

    /*testing for  INT_EN: addr=0x10033010*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: modbus_ascii_match_irq_en field_range: 20:20            field_access: rw  
    field_name: modbus_rtu_tout_irq_en   field_range: 19:19            field_access: rw  
    field_name: lin_lbdie_irq_en         field_range: 18:18            field_access: rw  
    field_name: frame_err_irq_en         field_range: 17:17            field_access: rw  
    field_name: rx_fifo_full_irq_en      field_range: 16:16            field_access: rw  
    field_name: rx_fifo_empty_irq_en     field_range: 15:15            field_access: rw  
    field_name: tx_fifo_full_irq_en      field_range: 14:14            field_access: rw  
    field_name: tx_fifo_empty_irq_en     field_range: 13:13            field_access: rw  
    field_name: rx_eot_irq_en            field_range: 12:12            field_access: rw  
    field_name: tx_eot_irq_en            field_range: 11:11            field_access: rw  
    field_name: rx_wm_tout_irq_en        field_range: 10:10            field_access: rw  
    field_name: rx_idle_tout_irq_en      field_range: 9:9              field_access: rw  
    field_name: spi_slv_over_run_irq_en  field_range: 8:8              field_access: rw  
    field_name: spi_slv_under_run_irq_en field_range: 7:7              field_access: rw  
    field_name: stop_bit_irq_en          field_range: 6:6              field_access: rw  
    field_name: ctsie_fall_en            field_range: 5:5              field_access: rw  
    field_name: ctsie_rise_en            field_range: 4:4              field_access: rw  
    field_name: parity_irq_en            field_range: 3:3              field_access: rw  
    field_name: ow_irq_en                field_range: 2:2              field_access: rw  
    field_name: rxie                     field_range: 1:1              field_access: rw  
    field_name: txie                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033010,0x0);

    /*testing for  STATUS: addr=0x10033014*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: ne_flag                  field_range: 24:24            field_access: w1c 
    field_name: frame_err_flag           field_range: 23:23            field_access: w1c 
    field_name: rx_fifo_full             field_range: 22:22            field_access: ro  
    field_name: tx_fifo_empty            field_range: 21:21            field_access: ro  
    field_name: cts_pin                  field_range: 20:20            field_access: ro  
    field_name: spi_slv_over_run         field_range: 19:19            field_access: w1c 
    field_name: spi_slv_under_run        field_range: 18:18            field_access: w1c 
    field_name: rx_eot                   field_range: 17:17            field_access: w1c 
    field_name: tx_eot                   field_range: 16:16            field_access: w1c 
    field_name: rx_fifo_empty            field_range: 15:15            field_access: ro  
    field_name: tx_fifo_full             field_range: 14:14            field_access: ro  
    field_name: rx_wm_tout_status        field_range: 13:13            field_access: w1c 
    field_name: rx_idle_tout_status      field_range: 12:12            field_access: w1c 
    field_name: reserved                 field_range: 11:9             field_access: ro  
    field_name: stop_bit_err_flag        field_range: 8:8              field_access: w1c 
    field_name: cts_fall_flag            field_range: 7:7              field_access: w1c 
    field_name: cts_rise_flag            field_range: 6:6              field_access: w1c 
    field_name: rx_error_parity          field_range: 5:5              field_access: w1c 
    field_name: rx_error_over_flow       field_range: 4:4              field_access: w1c 
    field_name: rx_busy                  field_range: 3:3              field_access: ro  
    field_name: tx_busy                  field_range: 2:2              field_access: ro  
    field_name: rxip                     field_range: 1:1              field_access: ro  
    field_name: txip                     field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x10033014,0x208000);

    /*testing for  DIV: addr=0x10033018*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: baud_div                 field_range: 20:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10033018,0x0);

    /*testing for  SETUP: addr=0x1003301c*/
    /*
    field_name: cfg_stop_bit             field_range: 31:30            field_access: rw  
    field_name: swap                     field_range: 29:29            field_access: rw  
    field_name: rxinv                    field_range: 28:28            field_access: rw  
    field_name: txinv                    field_range: 27:27            field_access: rw  
    field_name: datainv                  field_range: 26:26            field_access: rw  
    field_name: msbfirst                 field_range: 25:25            field_access: rw  
    field_name: de_inv                   field_range: 24:24            field_access: rw  
    field_name: de_deassertion_delay_en  field_range: 23:23            field_access: rw  
    field_name: de_assertion_early_en    field_range: 22:22            field_access: rw  
    field_name: rs485_en                 field_range: 21:21            field_access: rw  
    field_name: rx_dma_en                field_range: 20:20            field_access: rw  
    field_name: fraction_baud            field_range: 19:19            field_access: rw  
    field_name: sample_position          field_range: 18:17            field_access: rw  
    field_name: vote_majority_en         field_range: 16:16            field_access: rw  
    field_name: dma_en                   field_range: 15:15            field_access: rw  
    field_name: ctse                     field_range: 14:14            field_access: rw  
    field_name: rtse                     field_range: 13:13            field_access: rw  
    field_name: smartcard_en             field_range: 12:12            field_access: rw  
    field_name: hdsel                    field_range: 11:11            field_access: rw  
    field_name: lbcl                     field_range: 10:10            field_access: rw  
    field_name: cpha                     field_range: 9:9              field_access: rw  
    field_name: cpol                     field_range: 8:8              field_access: rw  
    field_name: clk_en                   field_range: 7:7              field_access: rw  
    field_name: bit_length               field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: separate_en              field_range: 2:2              field_access: rw  
    field_name: parity_sel               field_range: 1:1              field_access: rw  
    field_name: parity_en                field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1003301c,0xc0050030);

    /*testing for  SPI_SLAVE: addr=0x10033034*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: dis_nss                  field_range: 1:1              field_access: rw  
    field_name: slv_en                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033034,0x0);

    /*testing for EWR RX_IDLE: addr=0x10033044*/
    /*
    field_name: idle_tout_num            field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10033044,0xffff);

    /*testing for EWR RX_WM: addr=0x10033048*/
    /*
    field_name: wm_tout_num              field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10033048,0xffff);

    /*testing for RO RX_FIFO_LEFT_ENTRY: addr=0x1003304c*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1003304c,0x0);

    /*testing for RO TX_FIFO_LEFT_ENTRY: addr=0x10033050*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: left_entry               field_range: 4:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x10033050,0x0);

    /*testing for  TX_DATASIZE: addr=0x10033054*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: tx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033054,0x0);

    /*testing for  RX_DATASIZE: addr=0x10033058*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: rx_datasize_num          field_range: 1:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033058,0x0);

    /*testing for  SMARTCARD_SETUP: addr=0x10033060*/
    /*
    field_name: reserved                 field_range: 31:5             field_access: ro  
    field_name: tx_pue                   field_range: 4:4              field_access: rw  
    field_name: nack_en                  field_range: 3:3              field_access: rw  
    field_name: auto_retry_cnt           field_range: 2:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033060,0x0);

    /*testing for  SMARTCARD_TIMING: addr=0x10033064*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: psc                      field_range: 15:8             field_access: rw  
    field_name: guard_time               field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10033064,0x0);

    /*testing for RO IP_VERSION: addr=0x10033068*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x10033068,0x30004);

    /*testing for  ADVANCED_SETUP: addr=0x1003307c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: abr_mod                  field_range: 21:20            field_access: rw  
    field_name: abr_en                   field_range: 19:19            field_access: rw  
    field_name: reserved                 field_range: 18:16            field_access: ro  
    field_name: rtu_en                   field_range: 15:15            field_access: rw  
    field_name: irdalp_en                field_range: 14:14            field_access: rw  
    field_name: irda_en                  field_range: 13:13            field_access: rw  
    field_name: lin_lbdl                 field_range: 12:12            field_access: rw  
    field_name: lin_en                   field_range: 11:11            field_access: rw  
    field_name: add                      field_range: 10:3             field_access: rw  
    field_name: add7                     field_range: 2:2              field_access: rw  
    field_name: wake_mod                 field_range: 1:1              field_access: rw  
    field_name: mm_en                    field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1003307c,0x0);

    #endif

    #ifdef MISC_HAS_USART3_CLK_DIV

    #if defined(__ICACHE_PRESENT) && __ICACHE_PRESENT == 1
        DisableICache();
    #endif
    #if defined(__DCACHE_PRESENT) && __DCACHE_PRESENT == 1
        DisableDCache();
    #endif
    if(delta_mcycle1<delta_mcycle2)

        {state0=1;}

    if(Index > 0||state0==0){debug("reg test fail\r\n"); simulation_fail();} else{ debug("reg test pass\r\n");  simulation_pass();}

    #else

    if(Index > 0){debug("reg test fail\r\n"); simulation_fail();} else{ debug("reg test pass\r\n");  simulation_pass();}

    #endif

}
