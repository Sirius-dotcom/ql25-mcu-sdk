
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
    #ifdef MISC_HAS_I2C1_HAS_CLK

        i2c1_clk_en(ENABLE);

    #endif

    /*********************TEST FOR POR DEVAL *********************/

    /*testing for EWR TIMING: addr=0x10041028*/
    /*
    field_name: presc                    field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10041028,0xa);

    /*testing for  TXDATA: addr=0x10041030*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: txdata                   field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041030,0x0);

    /*testing for  INT_IE: addr=0x10041038*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: pec_byte_tc_ine_ie       field_range: 10:10            field_access: rw  
    field_name: sbc_tcr_ine_ie           field_range: 9:9              field_access: rw  
    field_name: err_ine_ie               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: nack_reception_int_ie    field_range: 6:6              field_access: rw  
    field_name: address_matched_int_ie   field_range: 5:5              field_access: rw  
    field_name: time_out_enable          field_range: 4:4              field_access: rw  
    field_name: rxfifo_wm_int_ie         field_range: 3:3              field_access: rw  
    field_name: txfifo_wm_int_ie         field_range: 2:2              field_access: rw  
    field_name: arbitration_lost_int_ie  field_range: 1:1              field_access: rw  
    field_name: eot_int_ie               field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041038,0x0);

    /*testing for  SLAVE_ADDRESS1: addr=0x1004103c*/
    /*
    field_name: slave_address1_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address1_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address1           field_range: 9:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1004103c,0x800ffc00);

    /*testing for  SLAVE_ADDRESS2: addr=0x10041044*/
    /*
    field_name: slave_address2_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address2_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address2           field_range: 9:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041044,0xffc00);

    /*testing for  TIME_OUT: addr=0x10041048*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: num                      field_range: 15:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10041048,0xffff);

    /*testing for  TXFIFO_WM: addr=0x1004107c*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1004107c,0x0);

    /*testing for  RXFIFO_WM: addr=0x10041080*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041080,0x0);

    /*testing for RO DEBUG: addr=0x10041098*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: status                   field_range: 14:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x10041098,0x3);

    /*testing for RO IP_VERSION: addr=0x1004109c*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x1004109c,0x30003);

    /*testing for EWR START_SETUP_TIME: addr=0x100410a0*/
    /*
    field_name: start_setup_time         field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410a0,0x32);

    /*testing for EWR START_HOLD_TIME: addr=0x100410a4*/
    /*
    field_name: start_hold_time          field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410a4,0x32);

    /*testing for EWR SDA_SETUP_TIME: addr=0x100410a8*/
    /*
    field_name: sda_setup_time           field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410a8,0x5);

    /*testing for EWR SDA_HOLD_TIME: addr=0x100410ac*/
    /*
    field_name: sda_hold_time            field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410ac,0x4);

    /*testing for EWR SCL_HIGH_PERIOD: addr=0x100410b0*/
    /*
    field_name: scl_high_period          field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410b0,0x32);

    /*testing for EWR SCL_LOW_PERIOD: addr=0x100410b4*/
    /*
    field_name: scl_low_period           field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410b4,0x32);

    /*testing for EWR STOP_SETUP_TIME: addr=0x100410b8*/
    /*
    field_name: stop_setup_time          field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410b8,0x32);

    /*testing for EWR BUS_FREE_TIME: addr=0x100410bc*/
    /*
    field_name: bus_free_time            field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410bc,0x32);

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

    /*testing for EWR TIMING: addr=0x10041028*/
    /*
    field_name: presc                    field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10041028, 0xa);

    /*testing for  TXDATA: addr=0x10041030*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: txdata                   field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041030, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x10041030, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x10041030, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x10041030, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  INT_IE: addr=0x10041038*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: pec_byte_tc_ine_ie       field_range: 10:10            field_access: rw  
    field_name: sbc_tcr_ine_ie           field_range: 9:9              field_access: rw  
    field_name: err_ine_ie               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: nack_reception_int_ie    field_range: 6:6              field_access: rw  
    field_name: address_matched_int_ie   field_range: 5:5              field_access: rw  
    field_name: time_out_enable          field_range: 4:4              field_access: rw  
    field_name: rxfifo_wm_int_ie         field_range: 3:3              field_access: rw  
    field_name: txfifo_wm_int_ie         field_range: 2:2              field_access: rw  
    field_name: arbitration_lost_int_ie  field_range: 1:1              field_access: rw  
    field_name: eot_int_ie               field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041038, 0x0, (0x00000000 & 0x77f | 0), 0x0, 0x77f);

    REG_PWR_Double_Check(0x10041038, 0x0, (0xFFFFFFFF & 0x77f | 0), 0x77f, 0x77f);

    REG_PWR_Double_Check(0x10041038, 0x0, (0x55555555 & 0x77f | 0), 0x555, 0x77f);

    REG_PWR_Double_Check(0x10041038, 0x0, (0xAAAAAAAA & 0x77f | 0), 0x22a, 0x77f);

    /*testing for  SLAVE_ADDRESS1: addr=0x1004103c*/
    /*
    field_name: slave_address1_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address1_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address1           field_range: 9:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0x00000000 & 0x800fffff | 0), 0x0, 0x800fffff);

    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0xFFFFFFFF & 0x800fffff | 0), 0x800fffff, 0x800fffff);

    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0x55555555 & 0x800fffff | 0), 0x55555, 0x800fffff);

    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0xAAAAAAAA & 0x800fffff | 0), 0x800aaaaa, 0x800fffff);

    /*testing for  SLAVE_ADDRESS2: addr=0x10041044*/
    /*
    field_name: slave_address2_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address2_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address2           field_range: 9:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041044, 0xffc00, (0x00000000 & 0x800fffff | 0), 0x0, 0x800fffff);

    REG_PWR_Double_Check(0x10041044, 0xffc00, (0xFFFFFFFF & 0x800fffff | 0), 0x800fffff, 0x800fffff);

    REG_PWR_Double_Check(0x10041044, 0xffc00, (0x55555555 & 0x800fffff | 0), 0x55555, 0x800fffff);

    REG_PWR_Double_Check(0x10041044, 0xffc00, (0xAAAAAAAA & 0x800fffff | 0), 0x800aaaaa, 0x800fffff);

    /*testing for  TIME_OUT: addr=0x10041048*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: num                      field_range: 15:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x10041048, 0xffff, (0x00000000 & 0xffff | 0), 0x0, 0xffff);

    REG_PWR_Double_Check(0x10041048, 0xffff, (0xFFFFFFFF & 0xffff | 0), 0xffff, 0xffff);

    REG_PWR_Double_Check(0x10041048, 0xffff, (0x55555555 & 0xffff | 0), 0x5555, 0xffff);

    REG_PWR_Double_Check(0x10041048, 0xffff, (0xAAAAAAAA & 0xffff | 0), 0xaaaa, 0xffff);

    /*testing for  TXFIFO_WM: addr=0x1004107c*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1004107c, 0x0, (0x00000000 & 0xf | 0), 0x0, 0xf);

    REG_PWR_Double_Check(0x1004107c, 0x0, (0xFFFFFFFF & 0xf | 0), 0xf, 0xf);

    REG_PWR_Double_Check(0x1004107c, 0x0, (0x55555555 & 0xf | 0), 0x5, 0xf);

    REG_PWR_Double_Check(0x1004107c, 0x0, (0xAAAAAAAA & 0xf | 0), 0xa, 0xf);

    /*testing for  RXFIFO_WM: addr=0x10041080*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041080, 0x0, (0x00000000 & 0xf | 0), 0x0, 0xf);

    REG_PWR_Double_Check(0x10041080, 0x0, (0xFFFFFFFF & 0xf | 0), 0xf, 0xf);

    REG_PWR_Double_Check(0x10041080, 0x0, (0x55555555 & 0xf | 0), 0x5, 0xf);

    REG_PWR_Double_Check(0x10041080, 0x0, (0xAAAAAAAA & 0xf | 0), 0xa, 0xf);

    /*testing for RO DEBUG: addr=0x10041098*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: status                   field_range: 14:0             field_access: ro  
    */
    REG_RO_Double_Check(0x10041098, 0x3, 0xFFFFFFFF);

    /*testing for RO IP_VERSION: addr=0x1004109c*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x1004109c, 0x30003, 0xFFFFFFFF);

    /*testing for EWR START_SETUP_TIME: addr=0x100410a0*/
    /*
    field_name: start_setup_time         field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410a0, 0x32);

    /*testing for EWR START_HOLD_TIME: addr=0x100410a4*/
    /*
    field_name: start_hold_time          field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410a4, 0x32);

    /*testing for EWR SDA_SETUP_TIME: addr=0x100410a8*/
    /*
    field_name: sda_setup_time           field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410a8, 0x5);

    /*testing for EWR SDA_HOLD_TIME: addr=0x100410ac*/
    /*
    field_name: sda_hold_time            field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410ac, 0x4);

    /*testing for EWR SCL_HIGH_PERIOD: addr=0x100410b0*/
    /*
    field_name: scl_high_period          field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410b0, 0x32);

    /*testing for EWR SCL_LOW_PERIOD: addr=0x100410b4*/
    /*
    field_name: scl_low_period           field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410b4, 0x32);

    /*testing for EWR STOP_SETUP_TIME: addr=0x100410b8*/
    /*
    field_name: stop_setup_time          field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410b8, 0x32);

    /*testing for EWR BUS_FREE_TIME: addr=0x100410bc*/
    /*
    field_name: bus_free_time            field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410bc, 0x32);

    delta_mcycle1=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle1;

    /*********************TEST FOR CLOCK DIVISION 36*********************/

    #ifdef MISC_HAS_I2C1_CLK_DIV

    i2c1_clk_div(35);

    start_mcycle2=__RV_CSR_READ(CSR_MCYCLE);

    /*testing for EWR TIMING: addr=0x10041028*/
    /*
    field_name: presc                    field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10041028, 0xa);

    /*testing for  TXDATA: addr=0x10041030*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: txdata                   field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041030, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x10041030, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x10041030, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x10041030, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  INT_IE: addr=0x10041038*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: pec_byte_tc_ine_ie       field_range: 10:10            field_access: rw  
    field_name: sbc_tcr_ine_ie           field_range: 9:9              field_access: rw  
    field_name: err_ine_ie               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: nack_reception_int_ie    field_range: 6:6              field_access: rw  
    field_name: address_matched_int_ie   field_range: 5:5              field_access: rw  
    field_name: time_out_enable          field_range: 4:4              field_access: rw  
    field_name: rxfifo_wm_int_ie         field_range: 3:3              field_access: rw  
    field_name: txfifo_wm_int_ie         field_range: 2:2              field_access: rw  
    field_name: arbitration_lost_int_ie  field_range: 1:1              field_access: rw  
    field_name: eot_int_ie               field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041038, 0x0, (0x00000000 & 0x77f | 0), 0x0, 0x77f);

    REG_PWR_Double_Check(0x10041038, 0x0, (0xFFFFFFFF & 0x77f | 0), 0x77f, 0x77f);

    REG_PWR_Double_Check(0x10041038, 0x0, (0x55555555 & 0x77f | 0), 0x555, 0x77f);

    REG_PWR_Double_Check(0x10041038, 0x0, (0xAAAAAAAA & 0x77f | 0), 0x22a, 0x77f);

    /*testing for  SLAVE_ADDRESS1: addr=0x1004103c*/
    /*
    field_name: slave_address1_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address1_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address1           field_range: 9:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0x00000000 & 0x800fffff | 0), 0x0, 0x800fffff);

    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0xFFFFFFFF & 0x800fffff | 0), 0x800fffff, 0x800fffff);

    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0x55555555 & 0x800fffff | 0), 0x55555, 0x800fffff);

    REG_PWR_Double_Check(0x1004103c, 0x800ffc00, (0xAAAAAAAA & 0x800fffff | 0), 0x800aaaaa, 0x800fffff);

    /*testing for  SLAVE_ADDRESS2: addr=0x10041044*/
    /*
    field_name: slave_address2_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address2_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address2           field_range: 9:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041044, 0xffc00, (0x00000000 & 0x800fffff | 0), 0x0, 0x800fffff);

    REG_PWR_Double_Check(0x10041044, 0xffc00, (0xFFFFFFFF & 0x800fffff | 0), 0x800fffff, 0x800fffff);

    REG_PWR_Double_Check(0x10041044, 0xffc00, (0x55555555 & 0x800fffff | 0), 0x55555, 0x800fffff);

    REG_PWR_Double_Check(0x10041044, 0xffc00, (0xAAAAAAAA & 0x800fffff | 0), 0x800aaaaa, 0x800fffff);

    /*testing for  TIME_OUT: addr=0x10041048*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: num                      field_range: 15:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x10041048, 0xffff, (0x00000000 & 0xffff | 0), 0x0, 0xffff);

    REG_PWR_Double_Check(0x10041048, 0xffff, (0xFFFFFFFF & 0xffff | 0), 0xffff, 0xffff);

    REG_PWR_Double_Check(0x10041048, 0xffff, (0x55555555 & 0xffff | 0), 0x5555, 0xffff);

    REG_PWR_Double_Check(0x10041048, 0xffff, (0xAAAAAAAA & 0xffff | 0), 0xaaaa, 0xffff);

    /*testing for  TXFIFO_WM: addr=0x1004107c*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1004107c, 0x0, (0x00000000 & 0xf | 0), 0x0, 0xf);

    REG_PWR_Double_Check(0x1004107c, 0x0, (0xFFFFFFFF & 0xf | 0), 0xf, 0xf);

    REG_PWR_Double_Check(0x1004107c, 0x0, (0x55555555 & 0xf | 0), 0x5, 0xf);

    REG_PWR_Double_Check(0x1004107c, 0x0, (0xAAAAAAAA & 0xf | 0), 0xa, 0xf);

    /*testing for  RXFIFO_WM: addr=0x10041080*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10041080, 0x0, (0x00000000 & 0xf | 0), 0x0, 0xf);

    REG_PWR_Double_Check(0x10041080, 0x0, (0xFFFFFFFF & 0xf | 0), 0xf, 0xf);

    REG_PWR_Double_Check(0x10041080, 0x0, (0x55555555 & 0xf | 0), 0x5, 0xf);

    REG_PWR_Double_Check(0x10041080, 0x0, (0xAAAAAAAA & 0xf | 0), 0xa, 0xf);

    /*testing for RO DEBUG: addr=0x10041098*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: status                   field_range: 14:0             field_access: ro  
    */
    REG_RO_Double_Check(0x10041098, 0x3, 0xFFFFFFFF);

    /*testing for RO IP_VERSION: addr=0x1004109c*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x1004109c, 0x30003, 0xFFFFFFFF);

    /*testing for EWR START_SETUP_TIME: addr=0x100410a0*/
    /*
    field_name: start_setup_time         field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410a0, 0x32);

    /*testing for EWR START_HOLD_TIME: addr=0x100410a4*/
    /*
    field_name: start_hold_time          field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410a4, 0x32);

    /*testing for EWR SDA_SETUP_TIME: addr=0x100410a8*/
    /*
    field_name: sda_setup_time           field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410a8, 0x5);

    /*testing for EWR SDA_HOLD_TIME: addr=0x100410ac*/
    /*
    field_name: sda_hold_time            field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410ac, 0x4);

    /*testing for EWR SCL_HIGH_PERIOD: addr=0x100410b0*/
    /*
    field_name: scl_high_period          field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410b0, 0x32);

    /*testing for EWR SCL_LOW_PERIOD: addr=0x100410b4*/
    /*
    field_name: scl_low_period           field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410b4, 0x32);

    /*testing for EWR STOP_SETUP_TIME: addr=0x100410b8*/
    /*
    field_name: stop_setup_time          field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410b8, 0x32);

    /*testing for EWR BUS_FREE_TIME: addr=0x100410bc*/
    /*
    field_name: bus_free_time            field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x100410bc, 0x32);

    delta_mcycle2=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle2;

    i2c1_clk_div(0);

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
    #ifdef MISC_HAS_I2C1_HAS_CLK

    i2c1_clk_en(DISABLE);

    /*testing for EWR TIMING: addr=0x10041028*/
    /*
    field_name: presc                    field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041028);

    /*testing for  TXDATA: addr=0x10041030*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: txdata                   field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041030);

    /*testing for  INT_IE: addr=0x10041038*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: pec_byte_tc_ine_ie       field_range: 10:10            field_access: rw  
    field_name: sbc_tcr_ine_ie           field_range: 9:9              field_access: rw  
    field_name: err_ine_ie               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: nack_reception_int_ie    field_range: 6:6              field_access: rw  
    field_name: address_matched_int_ie   field_range: 5:5              field_access: rw  
    field_name: time_out_enable          field_range: 4:4              field_access: rw  
    field_name: rxfifo_wm_int_ie         field_range: 3:3              field_access: rw  
    field_name: txfifo_wm_int_ie         field_range: 2:2              field_access: rw  
    field_name: arbitration_lost_int_ie  field_range: 1:1              field_access: rw  
    field_name: eot_int_ie               field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041038);

    /*testing for  SLAVE_ADDRESS1: addr=0x1004103c*/
    /*
    field_name: slave_address1_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address1_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address1           field_range: 9:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1004103c);

    /*testing for  SLAVE_ADDRESS2: addr=0x10041044*/
    /*
    field_name: slave_address2_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address2_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address2           field_range: 9:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041044);

    /*testing for  TIME_OUT: addr=0x10041048*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: num                      field_range: 15:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041048);

    /*testing for  TXFIFO_WM: addr=0x1004107c*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1004107c);

    /*testing for  RXFIFO_WM: addr=0x10041080*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041080);

    /*testing for RO DEBUG: addr=0x10041098*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: status                   field_range: 14:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10041098);

    /*testing for RO IP_VERSION: addr=0x1004109c*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1004109c);

    /*testing for EWR START_SETUP_TIME: addr=0x100410a0*/
    /*
    field_name: start_setup_time         field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410a0);

    /*testing for EWR START_HOLD_TIME: addr=0x100410a4*/
    /*
    field_name: start_hold_time          field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410a4);

    /*testing for EWR SDA_SETUP_TIME: addr=0x100410a8*/
    /*
    field_name: sda_setup_time           field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410a8);

    /*testing for EWR SDA_HOLD_TIME: addr=0x100410ac*/
    /*
    field_name: sda_hold_time            field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410ac);

    /*testing for EWR SCL_HIGH_PERIOD: addr=0x100410b0*/
    /*
    field_name: scl_high_period          field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410b0);

    /*testing for EWR SCL_LOW_PERIOD: addr=0x100410b4*/
    /*
    field_name: scl_low_period           field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410b4);

    /*testing for EWR STOP_SETUP_TIME: addr=0x100410b8*/
    /*
    field_name: stop_setup_time          field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410b8);

    /*testing for EWR BUS_FREE_TIME: addr=0x100410bc*/
    /*
    field_name: bus_free_time            field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410bc);

    i2c1_clk_en(ENABLE);

    #endif

    /*********************TEST FOR CLOCK RESET*********************/

    #ifdef MISC_HAS_I2C1_RST

    i2c1_set_rst(DISABLE);

    /*testing for EWR TIMING: addr=0x10041028*/
    /*
    field_name: presc                    field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041028);

    /*testing for  TXDATA: addr=0x10041030*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: txdata                   field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041030);

    /*testing for  INT_IE: addr=0x10041038*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: pec_byte_tc_ine_ie       field_range: 10:10            field_access: rw  
    field_name: sbc_tcr_ine_ie           field_range: 9:9              field_access: rw  
    field_name: err_ine_ie               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: nack_reception_int_ie    field_range: 6:6              field_access: rw  
    field_name: address_matched_int_ie   field_range: 5:5              field_access: rw  
    field_name: time_out_enable          field_range: 4:4              field_access: rw  
    field_name: rxfifo_wm_int_ie         field_range: 3:3              field_access: rw  
    field_name: txfifo_wm_int_ie         field_range: 2:2              field_access: rw  
    field_name: arbitration_lost_int_ie  field_range: 1:1              field_access: rw  
    field_name: eot_int_ie               field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041038);

    /*testing for  SLAVE_ADDRESS1: addr=0x1004103c*/
    /*
    field_name: slave_address1_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address1_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address1           field_range: 9:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1004103c);

    /*testing for  SLAVE_ADDRESS2: addr=0x10041044*/
    /*
    field_name: slave_address2_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address2_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address2           field_range: 9:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041044);

    /*testing for  TIME_OUT: addr=0x10041048*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: num                      field_range: 15:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041048);

    /*testing for  TXFIFO_WM: addr=0x1004107c*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1004107c);

    /*testing for  RXFIFO_WM: addr=0x10041080*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10041080);

    /*testing for RO DEBUG: addr=0x10041098*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: status                   field_range: 14:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10041098);

    /*testing for RO IP_VERSION: addr=0x1004109c*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1004109c);

    /*testing for EWR START_SETUP_TIME: addr=0x100410a0*/
    /*
    field_name: start_setup_time         field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410a0);

    /*testing for EWR START_HOLD_TIME: addr=0x100410a4*/
    /*
    field_name: start_hold_time          field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410a4);

    /*testing for EWR SDA_SETUP_TIME: addr=0x100410a8*/
    /*
    field_name: sda_setup_time           field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410a8);

    /*testing for EWR SDA_HOLD_TIME: addr=0x100410ac*/
    /*
    field_name: sda_hold_time            field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410ac);

    /*testing for EWR SCL_HIGH_PERIOD: addr=0x100410b0*/
    /*
    field_name: scl_high_period          field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410b0);

    /*testing for EWR SCL_LOW_PERIOD: addr=0x100410b4*/
    /*
    field_name: scl_low_period           field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410b4);

    /*testing for EWR STOP_SETUP_TIME: addr=0x100410b8*/
    /*
    field_name: stop_setup_time          field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410b8);

    /*testing for EWR BUS_FREE_TIME: addr=0x100410bc*/
    /*
    field_name: bus_free_time            field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100410bc);

    i2c1_set_rst(ENABLE);

    /*testing for EWR TIMING: addr=0x10041028*/
    /*
    field_name: presc                    field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10041028,0xa);

    /*testing for  TXDATA: addr=0x10041030*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: txdata                   field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041030,0x0);

    /*testing for  INT_IE: addr=0x10041038*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: pec_byte_tc_ine_ie       field_range: 10:10            field_access: rw  
    field_name: sbc_tcr_ine_ie           field_range: 9:9              field_access: rw  
    field_name: err_ine_ie               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: nack_reception_int_ie    field_range: 6:6              field_access: rw  
    field_name: address_matched_int_ie   field_range: 5:5              field_access: rw  
    field_name: time_out_enable          field_range: 4:4              field_access: rw  
    field_name: rxfifo_wm_int_ie         field_range: 3:3              field_access: rw  
    field_name: txfifo_wm_int_ie         field_range: 2:2              field_access: rw  
    field_name: arbitration_lost_int_ie  field_range: 1:1              field_access: rw  
    field_name: eot_int_ie               field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041038,0x0);

    /*testing for  SLAVE_ADDRESS1: addr=0x1004103c*/
    /*
    field_name: slave_address1_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address1_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address1           field_range: 9:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1004103c,0x800ffc00);

    /*testing for  SLAVE_ADDRESS2: addr=0x10041044*/
    /*
    field_name: slave_address2_en        field_range: 31:31            field_access: rw  
    field_name: reserved                 field_range: 30:20            field_access: ro  
    field_name: slave_address2_mask      field_range: 19:10            field_access: rw  
    field_name: slave_address2           field_range: 9:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041044,0xffc00);

    /*testing for  TIME_OUT: addr=0x10041048*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: num                      field_range: 15:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10041048,0xffff);

    /*testing for  TXFIFO_WM: addr=0x1004107c*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1004107c,0x0);

    /*testing for  RXFIFO_WM: addr=0x10041080*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: wm_cnt                   field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10041080,0x0);

    /*testing for RO DEBUG: addr=0x10041098*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: status                   field_range: 14:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x10041098,0x3);

    /*testing for RO IP_VERSION: addr=0x1004109c*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x1004109c,0x30003);

    /*testing for EWR START_SETUP_TIME: addr=0x100410a0*/
    /*
    field_name: start_setup_time         field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410a0,0x32);

    /*testing for EWR START_HOLD_TIME: addr=0x100410a4*/
    /*
    field_name: start_hold_time          field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410a4,0x32);

    /*testing for EWR SDA_SETUP_TIME: addr=0x100410a8*/
    /*
    field_name: sda_setup_time           field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410a8,0x5);

    /*testing for EWR SDA_HOLD_TIME: addr=0x100410ac*/
    /*
    field_name: sda_hold_time            field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410ac,0x4);

    /*testing for EWR SCL_HIGH_PERIOD: addr=0x100410b0*/
    /*
    field_name: scl_high_period          field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410b0,0x32);

    /*testing for EWR SCL_LOW_PERIOD: addr=0x100410b4*/
    /*
    field_name: scl_low_period           field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410b4,0x32);

    /*testing for EWR STOP_SETUP_TIME: addr=0x100410b8*/
    /*
    field_name: stop_setup_time          field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410b8,0x32);

    /*testing for EWR BUS_FREE_TIME: addr=0x100410bc*/
    /*
    field_name: bus_free_time            field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100410bc,0x32);

    #endif

    #ifdef MISC_HAS_I2C1_CLK_DIV

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
