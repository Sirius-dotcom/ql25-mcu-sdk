
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
    #ifdef MISC_HAS_LGPIO3_HAS_CLK

        lgpio3_clk_en(ENABLE);

    #endif

    /*********************TEST FOR POR DEVAL *********************/

    /*testing for  GPIO_GRP0_MODE0: addr=0x100a3008*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode0                    field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3008,0x0);

    /*testing for  GPIO_GRP0_MODE1: addr=0x100a300c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode1                    field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a300c,0x0);

    /*testing for  PAD_GRP0_OVAL: addr=0x100a3010*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: oval                     field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3010,0x0);

    /*testing for  PAD_GRP0_RISE_IE: addr=0x100a3014*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3014,0x0);

    /*testing for  PAD_GRP0_FALL_IE: addr=0x100a301c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a301c,0x0);

    /*testing for  PAD_GRP0_HIGH_IE: addr=0x100a3024*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3024,0x0);

    /*testing for  PAD_GRP0_LOW_IE: addr=0x100a302c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a302c,0x0);

    /*testing for  PAD_GRP0_POUT_MASK: addr=0x100a3044*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mask                     field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3044,0xff);

    /*testing for  PAD_GRP0_SW: addr=0x100a3074*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: keep                     field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3074,0x0);

    /*testing for  PAD_GRP0_EVENT_RISE: addr=0x100a3084*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: rise_en                  field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3084,0x0);

    /*testing for  PAD_GRP0_EVENT_FALL: addr=0x100a3088*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: fall_en                  field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3088,0x0);

    /*testing for  PAD_GRP0_DIGITAL_NOISE_FILTER: addr=0x100a308c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: en                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a308c,0x0);

    /*testing for  PAD_GRP0_EVENT_HIGH: addr=0x100a3098*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: high_en                  field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3098,0x0);

    /*testing for  PAD_GRP0_EVENT_LOW: addr=0x100a309c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: low_en                   field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a309c,0x0);

    /*testing for RO IP_VERSION: addr=0x100a3094*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x100a3094,0x10400);

    /*testing for  DIGITAL_NOISE_FILTER: addr=0x100a30a0*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: num                      field_range: 24:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100a30a0,0x0);

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

    /*testing for  GPIO_GRP0_MODE0: addr=0x100a3008*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode0                    field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3008, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3008, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3008, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3008, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  GPIO_GRP0_MODE1: addr=0x100a300c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode1                    field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a300c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a300c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a300c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a300c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_OVAL: addr=0x100a3010*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: oval                     field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3010, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3010, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3010, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3010, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_RISE_IE: addr=0x100a3014*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3014, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3014, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3014, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3014, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_FALL_IE: addr=0x100a301c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a301c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a301c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a301c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a301c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_HIGH_IE: addr=0x100a3024*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3024, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3024, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3024, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3024, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_LOW_IE: addr=0x100a302c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a302c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a302c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a302c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a302c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_POUT_MASK: addr=0x100a3044*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mask                     field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3044, 0xff, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3044, 0xff, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3044, 0xff, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3044, 0xff, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_SW: addr=0x100a3074*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: keep                     field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3074, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3074, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3074, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3074, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_RISE: addr=0x100a3084*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: rise_en                  field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3084, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3084, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3084, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3084, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_FALL: addr=0x100a3088*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: fall_en                  field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3088, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3088, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3088, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3088, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_DIGITAL_NOISE_FILTER: addr=0x100a308c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: en                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a308c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a308c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a308c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a308c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_HIGH: addr=0x100a3098*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: high_en                  field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3098, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3098, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3098, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3098, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_LOW: addr=0x100a309c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: low_en                   field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a309c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a309c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a309c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a309c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for RO IP_VERSION: addr=0x100a3094*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x100a3094, 0x10400, 0xFFFFFFFF);

    /*testing for  DIGITAL_NOISE_FILTER: addr=0x100a30a0*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: num                      field_range: 24:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x100a30a0, 0x0, (0x00000000 & 0x1ffffff | 0), 0x0, 0x1ffffff);

    REG_PWR_Double_Check(0x100a30a0, 0x0, (0xFFFFFFFF & 0x1ffffff | 0), 0x1ffffff, 0x1ffffff);

    REG_PWR_Double_Check(0x100a30a0, 0x0, (0x55555555 & 0x1ffffff | 0), 0x1555555, 0x1ffffff);

    REG_PWR_Double_Check(0x100a30a0, 0x0, (0xAAAAAAAA & 0x1ffffff | 0), 0xaaaaaa, 0x1ffffff);

    delta_mcycle1=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle1;

    /*********************TEST FOR CLOCK DIVISION 36*********************/

    #ifdef MISC_HAS_LGPIO3_CLK_DIV

    lgpio3_clk_div(35);

    start_mcycle2=__RV_CSR_READ(CSR_MCYCLE);

    /*testing for  GPIO_GRP0_MODE0: addr=0x100a3008*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode0                    field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3008, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3008, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3008, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3008, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  GPIO_GRP0_MODE1: addr=0x100a300c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode1                    field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a300c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a300c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a300c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a300c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_OVAL: addr=0x100a3010*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: oval                     field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3010, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3010, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3010, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3010, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_RISE_IE: addr=0x100a3014*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3014, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3014, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3014, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3014, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_FALL_IE: addr=0x100a301c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a301c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a301c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a301c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a301c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_HIGH_IE: addr=0x100a3024*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3024, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3024, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3024, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3024, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_LOW_IE: addr=0x100a302c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a302c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a302c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a302c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a302c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_POUT_MASK: addr=0x100a3044*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mask                     field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3044, 0xff, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3044, 0xff, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3044, 0xff, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3044, 0xff, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_SW: addr=0x100a3074*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: keep                     field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3074, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3074, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3074, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3074, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_RISE: addr=0x100a3084*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: rise_en                  field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3084, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3084, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3084, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3084, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_FALL: addr=0x100a3088*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: fall_en                  field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3088, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3088, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3088, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3088, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_DIGITAL_NOISE_FILTER: addr=0x100a308c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: en                       field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a308c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a308c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a308c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a308c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_HIGH: addr=0x100a3098*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: high_en                  field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a3098, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a3098, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a3098, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a3098, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for  PAD_GRP0_EVENT_LOW: addr=0x100a309c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: low_en                   field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x100a309c, 0x0, (0x00000000 & 0xff | 0), 0x0, 0xff);

    REG_PWR_Double_Check(0x100a309c, 0x0, (0xFFFFFFFF & 0xff | 0), 0xff, 0xff);

    REG_PWR_Double_Check(0x100a309c, 0x0, (0x55555555 & 0xff | 0), 0x55, 0xff);

    REG_PWR_Double_Check(0x100a309c, 0x0, (0xAAAAAAAA & 0xff | 0), 0xaa, 0xff);

    /*testing for RO IP_VERSION: addr=0x100a3094*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x100a3094, 0x10400, 0xFFFFFFFF);

    /*testing for  DIGITAL_NOISE_FILTER: addr=0x100a30a0*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: num                      field_range: 24:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x100a30a0, 0x0, (0x00000000 & 0x1ffffff | 0), 0x0, 0x1ffffff);

    REG_PWR_Double_Check(0x100a30a0, 0x0, (0xFFFFFFFF & 0x1ffffff | 0), 0x1ffffff, 0x1ffffff);

    REG_PWR_Double_Check(0x100a30a0, 0x0, (0x55555555 & 0x1ffffff | 0), 0x1555555, 0x1ffffff);

    REG_PWR_Double_Check(0x100a30a0, 0x0, (0xAAAAAAAA & 0x1ffffff | 0), 0xaaaaaa, 0x1ffffff);

    delta_mcycle2=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle2;

    lgpio3_clk_div(0);

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
    #ifdef MISC_HAS_LGPIO3_HAS_CLK

    lgpio3_clk_en(DISABLE);

    /*testing for  GPIO_GRP0_MODE0: addr=0x100a3008*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode0                    field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3008);

    /*testing for  GPIO_GRP0_MODE1: addr=0x100a300c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode1                    field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a300c);

    /*testing for  PAD_GRP0_OVAL: addr=0x100a3010*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: oval                     field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3010);

    /*testing for  PAD_GRP0_RISE_IE: addr=0x100a3014*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3014);

    /*testing for  PAD_GRP0_FALL_IE: addr=0x100a301c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a301c);

    /*testing for  PAD_GRP0_HIGH_IE: addr=0x100a3024*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3024);

    /*testing for  PAD_GRP0_LOW_IE: addr=0x100a302c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a302c);

    /*testing for  PAD_GRP0_POUT_MASK: addr=0x100a3044*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mask                     field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3044);

    /*testing for  PAD_GRP0_SW: addr=0x100a3074*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: keep                     field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3074);

    /*testing for  PAD_GRP0_EVENT_RISE: addr=0x100a3084*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: rise_en                  field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3084);

    /*testing for  PAD_GRP0_EVENT_FALL: addr=0x100a3088*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: fall_en                  field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3088);

    /*testing for  PAD_GRP0_DIGITAL_NOISE_FILTER: addr=0x100a308c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: en                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a308c);

    /*testing for  PAD_GRP0_EVENT_HIGH: addr=0x100a3098*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: high_en                  field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3098);

    /*testing for  PAD_GRP0_EVENT_LOW: addr=0x100a309c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: low_en                   field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a309c);

    /*testing for RO IP_VERSION: addr=0x100a3094*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x100a3094);

    /*testing for  DIGITAL_NOISE_FILTER: addr=0x100a30a0*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: num                      field_range: 24:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a30a0);

    lgpio3_clk_en(ENABLE);

    #endif

    /*********************TEST FOR CLOCK RESET*********************/

    #ifdef MISC_HAS_LGPIO3_RST

    lgpio3_set_rst(DISABLE);

    /*testing for  GPIO_GRP0_MODE0: addr=0x100a3008*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode0                    field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3008);

    /*testing for  GPIO_GRP0_MODE1: addr=0x100a300c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode1                    field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a300c);

    /*testing for  PAD_GRP0_OVAL: addr=0x100a3010*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: oval                     field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3010);

    /*testing for  PAD_GRP0_RISE_IE: addr=0x100a3014*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3014);

    /*testing for  PAD_GRP0_FALL_IE: addr=0x100a301c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a301c);

    /*testing for  PAD_GRP0_HIGH_IE: addr=0x100a3024*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3024);

    /*testing for  PAD_GRP0_LOW_IE: addr=0x100a302c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a302c);

    /*testing for  PAD_GRP0_POUT_MASK: addr=0x100a3044*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mask                     field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3044);

    /*testing for  PAD_GRP0_SW: addr=0x100a3074*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: keep                     field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3074);

    /*testing for  PAD_GRP0_EVENT_RISE: addr=0x100a3084*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: rise_en                  field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3084);

    /*testing for  PAD_GRP0_EVENT_FALL: addr=0x100a3088*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: fall_en                  field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3088);

    /*testing for  PAD_GRP0_DIGITAL_NOISE_FILTER: addr=0x100a308c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: en                       field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a308c);

    /*testing for  PAD_GRP0_EVENT_HIGH: addr=0x100a3098*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: high_en                  field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a3098);

    /*testing for  PAD_GRP0_EVENT_LOW: addr=0x100a309c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: low_en                   field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a309c);

    /*testing for RO IP_VERSION: addr=0x100a3094*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x100a3094);

    /*testing for  DIGITAL_NOISE_FILTER: addr=0x100a30a0*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: num                      field_range: 24:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x100a30a0);

    lgpio3_set_rst(ENABLE);

    /*testing for  GPIO_GRP0_MODE0: addr=0x100a3008*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode0                    field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3008,0x0);

    /*testing for  GPIO_GRP0_MODE1: addr=0x100a300c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mode1                    field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a300c,0x0);

    /*testing for  PAD_GRP0_OVAL: addr=0x100a3010*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: oval                     field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3010,0x0);

    /*testing for  PAD_GRP0_RISE_IE: addr=0x100a3014*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3014,0x0);

    /*testing for  PAD_GRP0_FALL_IE: addr=0x100a301c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a301c,0x0);

    /*testing for  PAD_GRP0_HIGH_IE: addr=0x100a3024*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3024,0x0);

    /*testing for  PAD_GRP0_LOW_IE: addr=0x100a302c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: ie                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a302c,0x0);

    /*testing for  PAD_GRP0_POUT_MASK: addr=0x100a3044*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: mask                     field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3044,0xff);

    /*testing for  PAD_GRP0_SW: addr=0x100a3074*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: keep                     field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3074,0x0);

    /*testing for  PAD_GRP0_EVENT_RISE: addr=0x100a3084*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: rise_en                  field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3084,0x0);

    /*testing for  PAD_GRP0_EVENT_FALL: addr=0x100a3088*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: fall_en                  field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3088,0x0);

    /*testing for  PAD_GRP0_DIGITAL_NOISE_FILTER: addr=0x100a308c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: en                       field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a308c,0x0);

    /*testing for  PAD_GRP0_EVENT_HIGH: addr=0x100a3098*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: high_en                  field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a3098,0x0);

    /*testing for  PAD_GRP0_EVENT_LOW: addr=0x100a309c*/
    /*
    field_name: reserved                 field_range: 31:8             field_access: ro  
    field_name: low_en                   field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x100a309c,0x0);

    /*testing for RO IP_VERSION: addr=0x100a3094*/
    /*
    field_name: version_num              field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x100a3094,0x10400);

    /*testing for  DIGITAL_NOISE_FILTER: addr=0x100a30a0*/
    /*
    field_name: reserved                 field_range: 31:25            field_access: ro  
    field_name: num                      field_range: 24:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x100a30a0,0x0);

    #endif

    #ifdef MISC_HAS_LGPIO3_CLK_DIV

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
