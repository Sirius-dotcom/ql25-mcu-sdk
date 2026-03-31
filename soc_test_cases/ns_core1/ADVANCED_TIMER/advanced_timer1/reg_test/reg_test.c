
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
    #ifdef MISC_HAS_ADVANCED_TIMER1_HAS_CLK

        advanced_timer1_clk_en(ENABLE);

    #endif

    /*********************TEST FOR POR DEVAL *********************/

    /*testing for  TIM_CR1: addr=0x10081000*/
    /*
    field_name: reserved                 field_range: 31:10            field_access: ro  
    field_name: ckd                      field_range: 9:8              field_access: rw  
    field_name: arpe                     field_range: 7:7              field_access: rw  
    field_name: cms                      field_range: 6:5              field_access: rw  
    field_name: dir                      field_range: 4:4              field_access: rw  
    field_name: opm                      field_range: 3:3              field_access: rw  
    field_name: urs                      field_range: 2:2              field_access: rw  
    field_name: udis                     field_range: 1:1              field_access: rw  
    field_name: cen                      field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081000,0x0);

    /*testing for  TIM_CR2: addr=0x10081004*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ois4                     field_range: 14:14            field_access: rw  
    field_name: ois3n                    field_range: 13:13            field_access: rw  
    field_name: ois3                     field_range: 12:12            field_access: rw  
    field_name: ois2n                    field_range: 11:11            field_access: rw  
    field_name: ois2                     field_range: 10:10            field_access: rw  
    field_name: ois1n                    field_range: 9:9              field_access: rw  
    field_name: ois1                     field_range: 8:8              field_access: rw  
    field_name: ti1s                     field_range: 7:7              field_access: rw  
    field_name: mms                      field_range: 6:4              field_access: rw  
    field_name: ccds                     field_range: 3:3              field_access: rw  
    field_name: ccus                     field_range: 2:2              field_access: rw  
    field_name: reserved                 field_range: 1:1              field_access: ro  
    field_name: ccpc                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081004,0x0);

    /*testing for  TIM_SMCR: addr=0x10081008*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etp                      field_range: 17:17            field_access: rw  
    field_name: ece                      field_range: 16:16            field_access: rw  
    field_name: etps                     field_range: 15:14            field_access: rw  
    field_name: etf                      field_range: 13:10            field_access: rw  
    field_name: msm                      field_range: 9:9              field_access: rw  
    field_name: ts                       field_range: 8:4              field_access: rw  
    field_name: sms                      field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081008,0x0);

    /*testing for  TIM_DIER: addr=0x1008100c*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: tde                      field_range: 14:14            field_access: rw  
    field_name: comde                    field_range: 13:13            field_access: rw  
    field_name: cc4de                    field_range: 12:12            field_access: rw  
    field_name: cc3de                    field_range: 11:11            field_access: rw  
    field_name: cc2de                    field_range: 10:10            field_access: rw  
    field_name: cc1de                    field_range: 9:9              field_access: rw  
    field_name: ude                      field_range: 8:8              field_access: rw  
    field_name: bie                      field_range: 7:7              field_access: rw  
    field_name: tie                      field_range: 6:6              field_access: rw  
    field_name: comie                    field_range: 5:5              field_access: rw  
    field_name: cc4ie                    field_range: 4:4              field_access: rw  
    field_name: cc3ie                    field_range: 3:3              field_access: rw  
    field_name: cc2ie                    field_range: 2:2              field_access: rw  
    field_name: cc1ie                    field_range: 1:1              field_access: rw  
    field_name: uie                      field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1008100c,0x0);

    /*testing for  TIM_SR: addr=0x10081010*/
    /*
    field_name: reserved                 field_range: 31:14            field_access: ro  
    field_name: sbif                     field_range: 13:13            field_access: w0c 
    field_name: cc4of                    field_range: 12:12            field_access: w0c 
    field_name: cc3of                    field_range: 11:11            field_access: w0c 
    field_name: cc2of                    field_range: 10:10            field_access: w0c 
    field_name: cc1of                    field_range: 9:9              field_access: w0c 
    field_name: b2if                     field_range: 8:8              field_access: w0c 
    field_name: bif                      field_range: 7:7              field_access: w0c 
    field_name: tif                      field_range: 6:6              field_access: w0c 
    field_name: comif                    field_range: 5:5              field_access: w0c 
    field_name: cc4if                    field_range: 4:4              field_access: w0c 
    field_name: cc3if                    field_range: 3:3              field_access: w0c 
    field_name: cc2if                    field_range: 2:2              field_access: w0c 
    field_name: cc1if                    field_range: 1:1              field_access: w0c 
    field_name: uif                      field_range: 0:0              field_access: rc  
    */
    REG_DefVal_CHECK(0x10081010,0x0);

    /*testing for  TIM_EGR: addr=0x10081014*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: b2g                      field_range: 8:8              field_access: wo  
    field_name: bg                       field_range: 7:7              field_access: wo  
    field_name: tg                       field_range: 6:6              field_access: wo  
    field_name: comg                     field_range: 5:5              field_access: wo  
    field_name: cc4g                     field_range: 4:4              field_access: wo  
    field_name: cc3g                     field_range: 3:3              field_access: wo  
    field_name: cc2g                     field_range: 2:2              field_access: wo  
    field_name: cc1g                     field_range: 1:1              field_access: wo  
    field_name: ug                       field_range: 0:0              field_access: wo  
    */
    REG_DefVal_CHECK(0x10081014,0x0);

    /*testing for  TIM_CCER: addr=0x10081020*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: co4e                     field_range: 17:17            field_access: ro  
    field_name: co3e                     field_range: 16:16            field_access: rw  
    field_name: co2e                     field_range: 15:15            field_access: rw  
    field_name: co1e                     field_range: 14:14            field_access: rw  
    field_name: cc4p                     field_range: 13:13            field_access: rw  
    field_name: cc4e                     field_range: 12:12            field_access: rw  
    field_name: cc3np                    field_range: 11:11            field_access: rw  
    field_name: cc3ne                    field_range: 10:10            field_access: rw  
    field_name: cc3p                     field_range: 9:9              field_access: rw  
    field_name: cc3e                     field_range: 8:8              field_access: rw  
    field_name: cc2np                    field_range: 7:7              field_access: rw  
    field_name: cc2ne                    field_range: 6:6              field_access: rw  
    field_name: cc2p                     field_range: 5:5              field_access: rw  
    field_name: cc2e                     field_range: 4:4              field_access: rw  
    field_name: cc1np                    field_range: 3:3              field_access: rw  
    field_name: cc1ne                    field_range: 2:2              field_access: rw  
    field_name: cc1p                     field_range: 1:1              field_access: rw  
    field_name: cc1e                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081020,0x1c000);

    /*testing for EWR TIM_CNT: addr=0x10081024*/
    /*
    field_name: cnt                      field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081024,0x0);

    /*testing for EWR TIM_PSC: addr=0x10081028*/
    /*
    field_name: psc                      field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081028,0x0);

    /*testing for EWR TIM_ARR: addr=0x1008102c*/
    /*
    field_name: arr                      field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x1008102c,0xffffffff);

    /*testing for  TIM_RCR: addr=0x10081030*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ic_psc_rep_en            field_range: 14:11            field_access: rw  
    field_name: gc4c                     field_range: 10:8             field_access: rw  
    field_name: rep                      field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081030,0x0);

    /*testing for EWR TIM_CCR1: addr=0x10081034*/
    /*
    field_name: ccr1                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081034,0x0);

    /*testing for EWR TIM_CCR2: addr=0x10081038*/
    /*
    field_name: ccr2                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081038,0x0);

    /*testing for EWR TIM_CCR3: addr=0x1008103c*/
    /*
    field_name: ccr3                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x1008103c,0x0);

    /*testing for EWR TIM_CCR4: addr=0x10081040*/
    /*
    field_name: ccr4                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081040,0x0);

    /*testing for  TIM_DCR: addr=0x10081048*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: map_idx                  field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: dbl                      field_range: 12:8             field_access: rw  
    field_name: reserved                 field_range: 7:5              field_access: ro  
    field_name: dba                      field_range: 4:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081048,0x0);

    /*testing for RO TIM_VERSION: addr=0x10081050*/
    /*
    field_name: git_ver                  field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x10081050,0x10010);

    /*testing for EWR TIM_IC_PSC_REP: addr=0x10081054*/
    /*
    field_name: ic4psc_rep               field_range: 31:24            field_access: rw  
    field_name: ic3psc_rep               field_range: 23:16            field_access: rw  
    field_name: ic2psc_rep               field_range: 15:8             field_access: rw  
    field_name: ic1psc_rep               field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081054,0x0);

    /*testing for  TIM_AF1: addr=0x10081058*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etrsel                   field_range: 17:14            field_access: rw  
    field_name: reserved                 field_range: 13:12            field_access: ro  
    field_name: bkcmp2p                  field_range: 11:11            field_access: rw  
    field_name: bkcmp1p                  field_range: 10:10            field_access: rw  
    field_name: bkinp                    field_range: 9:9              field_access: rw  
    field_name: bkdf1bk0e                field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bkcmp2e                  field_range: 2:2              field_access: rw  
    field_name: bkcmp1e                  field_range: 1:1              field_access: rw  
    field_name: bkine                    field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081058,0x0);

    /*testing for  TIM_AF2: addr=0x1008105c*/
    /*
    field_name: reserved                 field_range: 31:12            field_access: ro  
    field_name: bk2cmp2p                 field_range: 11:11            field_access: rw  
    field_name: bk2cmp1p                 field_range: 10:10            field_access: rw  
    field_name: bk2inp                   field_range: 9:9              field_access: rw  
    field_name: bk2df1bk1e               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bk2cmp2e                 field_range: 2:2              field_access: rw  
    field_name: bk2cmp1e                 field_range: 1:1              field_access: rw  
    field_name: bk2ine                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1008105c,0x0);

    /*testing for  TIM_TISEL: addr=0x10081060*/
    /*
    field_name: reserved                 field_range: 31:28            field_access: ro  
    field_name: ti4sel                   field_range: 27:24            field_access: rw  
    field_name: reserved                 field_range: 23:20            field_access: ro  
    field_name: ti3sel                   field_range: 19:16            field_access: rw  
    field_name: reserved                 field_range: 15:12            field_access: ro  
    field_name: ti2sel                   field_range: 11:8             field_access: rw  
    field_name: reserved                 field_range: 7:4              field_access: ro  
    field_name: ti1sel                   field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081060,0x0);

    /*testing for EWR TIM_OUTPUT_DELAY: addr=0x10081064*/
    /*
    field_name: oc4_delay                field_range: 31:24            field_access: rw  
    field_name: oc3_delay                field_range: 23:16            field_access: rw  
    field_name: oc2_delay                field_range: 15:8             field_access: rw  
    field_name: oc1_delay                field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081064,0x0);

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

    /*testing for  TIM_CR1: addr=0x10081000*/
    /*
    field_name: reserved                 field_range: 31:10            field_access: ro  
    field_name: ckd                      field_range: 9:8              field_access: rw  
    field_name: arpe                     field_range: 7:7              field_access: rw  
    field_name: cms                      field_range: 6:5              field_access: rw  
    field_name: dir                      field_range: 4:4              field_access: rw  
    field_name: opm                      field_range: 3:3              field_access: rw  
    field_name: urs                      field_range: 2:2              field_access: rw  
    field_name: udis                     field_range: 1:1              field_access: rw  
    field_name: cen                      field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081000, 0x0, (0x00000000 & 0x3ef | 0), 0x0, 0x3ef);

    REG_PWR_Double_Check(0x10081000, 0x0, (0xFFFFFFFF & 0x3ef | 0), 0x3ef, 0x3ef);

    REG_PWR_Double_Check(0x10081000, 0x0, (0x55555555 & 0x3ef | 0), 0x145, 0x3ef);

    REG_PWR_Double_Check(0x10081000, 0x0, (0xAAAAAAAA & 0x3ef | 0), 0x2aa, 0x3ef);

    /*testing for  TIM_CR2: addr=0x10081004*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ois4                     field_range: 14:14            field_access: rw  
    field_name: ois3n                    field_range: 13:13            field_access: rw  
    field_name: ois3                     field_range: 12:12            field_access: rw  
    field_name: ois2n                    field_range: 11:11            field_access: rw  
    field_name: ois2                     field_range: 10:10            field_access: rw  
    field_name: ois1n                    field_range: 9:9              field_access: rw  
    field_name: ois1                     field_range: 8:8              field_access: rw  
    field_name: ti1s                     field_range: 7:7              field_access: rw  
    field_name: mms                      field_range: 6:4              field_access: rw  
    field_name: ccds                     field_range: 3:3              field_access: rw  
    field_name: ccus                     field_range: 2:2              field_access: rw  
    field_name: reserved                 field_range: 1:1              field_access: ro  
    field_name: ccpc                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081004, 0x0, (0x00000000 & 0x7ffd | 0), 0x0, 0x7ffd);

    REG_PWR_Double_Check(0x10081004, 0x0, (0xFFFFFFFF & 0x7ffd | 0), 0x7ffd, 0x7ffd);

    REG_PWR_Double_Check(0x10081004, 0x0, (0x55555555 & 0x7ffd | 0), 0x5555, 0x7ffd);

    REG_PWR_Double_Check(0x10081004, 0x0, (0xAAAAAAAA & 0x7ffd | 0), 0x2aa8, 0x7ffd);

    /*testing for  TIM_SMCR: addr=0x10081008*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etp                      field_range: 17:17            field_access: rw  
    field_name: ece                      field_range: 16:16            field_access: rw  
    field_name: etps                     field_range: 15:14            field_access: rw  
    field_name: etf                      field_range: 13:10            field_access: rw  
    field_name: msm                      field_range: 9:9              field_access: rw  
    field_name: ts                       field_range: 8:4              field_access: rw  
    field_name: sms                      field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081008, 0x0, (0x00000000 & 0x3ffff | 0), 0x0, 0x3ffff);

    REG_PWR_Double_Check(0x10081008, 0x0, (0xFFFFFFFF & 0x3ffff | 0), 0x3ffff, 0x3ffff);

    REG_PWR_Double_Check(0x10081008, 0x0, (0x55555555 & 0x3ffff | 0), 0x15555, 0x3ffff);

    REG_PWR_Double_Check(0x10081008, 0x0, (0xAAAAAAAA & 0x3ffff | 0), 0x2aaaa, 0x3ffff);

    /*testing for  TIM_DIER: addr=0x1008100c*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: tde                      field_range: 14:14            field_access: rw  
    field_name: comde                    field_range: 13:13            field_access: rw  
    field_name: cc4de                    field_range: 12:12            field_access: rw  
    field_name: cc3de                    field_range: 11:11            field_access: rw  
    field_name: cc2de                    field_range: 10:10            field_access: rw  
    field_name: cc1de                    field_range: 9:9              field_access: rw  
    field_name: ude                      field_range: 8:8              field_access: rw  
    field_name: bie                      field_range: 7:7              field_access: rw  
    field_name: tie                      field_range: 6:6              field_access: rw  
    field_name: comie                    field_range: 5:5              field_access: rw  
    field_name: cc4ie                    field_range: 4:4              field_access: rw  
    field_name: cc3ie                    field_range: 3:3              field_access: rw  
    field_name: cc2ie                    field_range: 2:2              field_access: rw  
    field_name: cc1ie                    field_range: 1:1              field_access: rw  
    field_name: uie                      field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1008100c, 0x0, (0x00000000 & 0x7fff | 0), 0x0, 0x7fff);

    REG_PWR_Double_Check(0x1008100c, 0x0, (0xFFFFFFFF & 0x7fff | 0), 0x7fff, 0x7fff);

    REG_PWR_Double_Check(0x1008100c, 0x0, (0x55555555 & 0x7fff | 0), 0x5555, 0x7fff);

    REG_PWR_Double_Check(0x1008100c, 0x0, (0xAAAAAAAA & 0x7fff | 0), 0x2aaa, 0x7fff);

    /*testing for  TIM_SR: addr=0x10081010*/
    /*
    field_name: reserved                 field_range: 31:14            field_access: ro  
    field_name: sbif                     field_range: 13:13            field_access: w0c 
    field_name: cc4of                    field_range: 12:12            field_access: w0c 
    field_name: cc3of                    field_range: 11:11            field_access: w0c 
    field_name: cc2of                    field_range: 10:10            field_access: w0c 
    field_name: cc1of                    field_range: 9:9              field_access: w0c 
    field_name: b2if                     field_range: 8:8              field_access: w0c 
    field_name: bif                      field_range: 7:7              field_access: w0c 
    field_name: tif                      field_range: 6:6              field_access: w0c 
    field_name: comif                    field_range: 5:5              field_access: w0c 
    field_name: cc4if                    field_range: 4:4              field_access: w0c 
    field_name: cc3if                    field_range: 3:3              field_access: w0c 
    field_name: cc2if                    field_range: 2:2              field_access: w0c 
    field_name: cc1if                    field_range: 1:1              field_access: w0c 
    field_name: uif                      field_range: 0:0              field_access: rc  
    */
    REG_PWR_Double_Check(0x10081010, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081010, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081010, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081010, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  TIM_EGR: addr=0x10081014*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: b2g                      field_range: 8:8              field_access: wo  
    field_name: bg                       field_range: 7:7              field_access: wo  
    field_name: tg                       field_range: 6:6              field_access: wo  
    field_name: comg                     field_range: 5:5              field_access: wo  
    field_name: cc4g                     field_range: 4:4              field_access: wo  
    field_name: cc3g                     field_range: 3:3              field_access: wo  
    field_name: cc2g                     field_range: 2:2              field_access: wo  
    field_name: cc1g                     field_range: 1:1              field_access: wo  
    field_name: ug                       field_range: 0:0              field_access: wo  
    */
    REG_PWR_Double_Check(0x10081014, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081014, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081014, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081014, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  TIM_CCER: addr=0x10081020*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: co4e                     field_range: 17:17            field_access: ro  
    field_name: co3e                     field_range: 16:16            field_access: rw  
    field_name: co2e                     field_range: 15:15            field_access: rw  
    field_name: co1e                     field_range: 14:14            field_access: rw  
    field_name: cc4p                     field_range: 13:13            field_access: rw  
    field_name: cc4e                     field_range: 12:12            field_access: rw  
    field_name: cc3np                    field_range: 11:11            field_access: rw  
    field_name: cc3ne                    field_range: 10:10            field_access: rw  
    field_name: cc3p                     field_range: 9:9              field_access: rw  
    field_name: cc3e                     field_range: 8:8              field_access: rw  
    field_name: cc2np                    field_range: 7:7              field_access: rw  
    field_name: cc2ne                    field_range: 6:6              field_access: rw  
    field_name: cc2p                     field_range: 5:5              field_access: rw  
    field_name: cc2e                     field_range: 4:4              field_access: rw  
    field_name: cc1np                    field_range: 3:3              field_access: rw  
    field_name: cc1ne                    field_range: 2:2              field_access: rw  
    field_name: cc1p                     field_range: 1:1              field_access: rw  
    field_name: cc1e                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081020, 0x1c000, (0x00000000 & 0x1ffff | 0), 0x0, 0x1ffff);

    REG_PWR_Double_Check(0x10081020, 0x1c000, (0xFFFFFFFF & 0x1ffff | 0), 0x1ffff, 0x1ffff);

    REG_PWR_Double_Check(0x10081020, 0x1c000, (0x55555555 & 0x1ffff | 0), 0x15555, 0x1ffff);

    REG_PWR_Double_Check(0x10081020, 0x1c000, (0xAAAAAAAA & 0x1ffff | 0), 0xaaaa, 0x1ffff);

    /*testing for EWR TIM_CNT: addr=0x10081024*/
    /*
    field_name: cnt                      field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081024, 0x0);

    /*testing for EWR TIM_PSC: addr=0x10081028*/
    /*
    field_name: psc                      field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081028, 0x0);

    /*testing for EWR TIM_ARR: addr=0x1008102c*/
    /*
    field_name: arr                      field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x1008102c, 0xffffffff);

    /*testing for  TIM_RCR: addr=0x10081030*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ic_psc_rep_en            field_range: 14:11            field_access: rw  
    field_name: gc4c                     field_range: 10:8             field_access: rw  
    field_name: rep                      field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081030, 0x0, (0x00000000 & 0x7fff | 0), 0x0, 0x7fff);

    REG_PWR_Double_Check(0x10081030, 0x0, (0xFFFFFFFF & 0x7fff | 0), 0x7fff, 0x7fff);

    REG_PWR_Double_Check(0x10081030, 0x0, (0x55555555 & 0x7fff | 0), 0x5555, 0x7fff);

    REG_PWR_Double_Check(0x10081030, 0x0, (0xAAAAAAAA & 0x7fff | 0), 0x2aaa, 0x7fff);

    /*testing for EWR TIM_CCR1: addr=0x10081034*/
    /*
    field_name: ccr1                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081034, 0x0);

    /*testing for EWR TIM_CCR2: addr=0x10081038*/
    /*
    field_name: ccr2                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081038, 0x0);

    /*testing for EWR TIM_CCR3: addr=0x1008103c*/
    /*
    field_name: ccr3                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x1008103c, 0x0);

    /*testing for EWR TIM_CCR4: addr=0x10081040*/
    /*
    field_name: ccr4                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081040, 0x0);

    /*testing for  TIM_DCR: addr=0x10081048*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: map_idx                  field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: dbl                      field_range: 12:8             field_access: rw  
    field_name: reserved                 field_range: 7:5              field_access: ro  
    field_name: dba                      field_range: 4:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081048, 0x0, (0x00000000 & 0x1f1f | 0), 0x0, 0x1f1f);

    REG_PWR_Double_Check(0x10081048, 0x0, (0xFFFFFFFF & 0x1f1f | 0), 0x1f1f, 0x1f1f);

    REG_PWR_Double_Check(0x10081048, 0x0, (0x55555555 & 0x1f1f | 0), 0x1515, 0x1f1f);

    REG_PWR_Double_Check(0x10081048, 0x0, (0xAAAAAAAA & 0x1f1f | 0), 0xa0a, 0x1f1f);

    /*testing for RO TIM_VERSION: addr=0x10081050*/
    /*
    field_name: git_ver                  field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x10081050, 0x10010, 0xFFFFFFFF);

    /*testing for EWR TIM_IC_PSC_REP: addr=0x10081054*/
    /*
    field_name: ic4psc_rep               field_range: 31:24            field_access: rw  
    field_name: ic3psc_rep               field_range: 23:16            field_access: rw  
    field_name: ic2psc_rep               field_range: 15:8             field_access: rw  
    field_name: ic1psc_rep               field_range: 7:0              field_access: rw  
    */
    REG_EWR_Double_Check(0x10081054, 0x0);

    /*testing for  TIM_AF1: addr=0x10081058*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etrsel                   field_range: 17:14            field_access: rw  
    field_name: reserved                 field_range: 13:12            field_access: ro  
    field_name: bkcmp2p                  field_range: 11:11            field_access: rw  
    field_name: bkcmp1p                  field_range: 10:10            field_access: rw  
    field_name: bkinp                    field_range: 9:9              field_access: rw  
    field_name: bkdf1bk0e                field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bkcmp2e                  field_range: 2:2              field_access: rw  
    field_name: bkcmp1e                  field_range: 1:1              field_access: rw  
    field_name: bkine                    field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081058, 0x0, (0x00000000 & 0x3cf07 | 0), 0x0, 0x3cf07);

    REG_PWR_Double_Check(0x10081058, 0x0, (0xFFFFFFFF & 0x3cf07 | 0), 0x3cf07, 0x3cf07);

    REG_PWR_Double_Check(0x10081058, 0x0, (0x55555555 & 0x3cf07 | 0), 0x14505, 0x3cf07);

    REG_PWR_Double_Check(0x10081058, 0x0, (0xAAAAAAAA & 0x3cf07 | 0), 0x28a02, 0x3cf07);

    /*testing for  TIM_AF2: addr=0x1008105c*/
    /*
    field_name: reserved                 field_range: 31:12            field_access: ro  
    field_name: bk2cmp2p                 field_range: 11:11            field_access: rw  
    field_name: bk2cmp1p                 field_range: 10:10            field_access: rw  
    field_name: bk2inp                   field_range: 9:9              field_access: rw  
    field_name: bk2df1bk1e               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bk2cmp2e                 field_range: 2:2              field_access: rw  
    field_name: bk2cmp1e                 field_range: 1:1              field_access: rw  
    field_name: bk2ine                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1008105c, 0x0, (0x00000000 & 0xf07 | 0), 0x0, 0xf07);

    REG_PWR_Double_Check(0x1008105c, 0x0, (0xFFFFFFFF & 0xf07 | 0), 0xf07, 0xf07);

    REG_PWR_Double_Check(0x1008105c, 0x0, (0x55555555 & 0xf07 | 0), 0x505, 0xf07);

    REG_PWR_Double_Check(0x1008105c, 0x0, (0xAAAAAAAA & 0xf07 | 0), 0xa02, 0xf07);

    /*testing for  TIM_TISEL: addr=0x10081060*/
    /*
    field_name: reserved                 field_range: 31:28            field_access: ro  
    field_name: ti4sel                   field_range: 27:24            field_access: rw  
    field_name: reserved                 field_range: 23:20            field_access: ro  
    field_name: ti3sel                   field_range: 19:16            field_access: rw  
    field_name: reserved                 field_range: 15:12            field_access: ro  
    field_name: ti2sel                   field_range: 11:8             field_access: rw  
    field_name: reserved                 field_range: 7:4              field_access: ro  
    field_name: ti1sel                   field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081060, 0x0, (0x00000000 & 0xf0f0f0f | 0), 0x0, 0xf0f0f0f);

    REG_PWR_Double_Check(0x10081060, 0x0, (0xFFFFFFFF & 0xf0f0f0f | 0), 0xf0f0f0f, 0xf0f0f0f);

    REG_PWR_Double_Check(0x10081060, 0x0, (0x55555555 & 0xf0f0f0f | 0), 0x5050505, 0xf0f0f0f);

    REG_PWR_Double_Check(0x10081060, 0x0, (0xAAAAAAAA & 0xf0f0f0f | 0), 0xa0a0a0a, 0xf0f0f0f);

    /*testing for EWR TIM_OUTPUT_DELAY: addr=0x10081064*/
    /*
    field_name: oc4_delay                field_range: 31:24            field_access: rw  
    field_name: oc3_delay                field_range: 23:16            field_access: rw  
    field_name: oc2_delay                field_range: 15:8             field_access: rw  
    field_name: oc1_delay                field_range: 7:0              field_access: rw  
    */
    REG_EWR_Double_Check(0x10081064, 0x0);

    delta_mcycle1=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle1;

    /*********************TEST FOR CLOCK DIVISION 36*********************/

    #ifdef MISC_HAS_ADVANCED_TIMER1_CLK_DIV

    advanced_timer1_clk_div(35);

    start_mcycle2=__RV_CSR_READ(CSR_MCYCLE);

    /*testing for  TIM_CR1: addr=0x10081000*/
    /*
    field_name: reserved                 field_range: 31:10            field_access: ro  
    field_name: ckd                      field_range: 9:8              field_access: rw  
    field_name: arpe                     field_range: 7:7              field_access: rw  
    field_name: cms                      field_range: 6:5              field_access: rw  
    field_name: dir                      field_range: 4:4              field_access: rw  
    field_name: opm                      field_range: 3:3              field_access: rw  
    field_name: urs                      field_range: 2:2              field_access: rw  
    field_name: udis                     field_range: 1:1              field_access: rw  
    field_name: cen                      field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081000, 0x0, (0x00000000 & 0x3ef | 0), 0x0, 0x3ef);

    REG_PWR_Double_Check(0x10081000, 0x0, (0xFFFFFFFF & 0x3ef | 0), 0x3ef, 0x3ef);

    REG_PWR_Double_Check(0x10081000, 0x0, (0x55555555 & 0x3ef | 0), 0x145, 0x3ef);

    REG_PWR_Double_Check(0x10081000, 0x0, (0xAAAAAAAA & 0x3ef | 0), 0x2aa, 0x3ef);

    /*testing for  TIM_CR2: addr=0x10081004*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ois4                     field_range: 14:14            field_access: rw  
    field_name: ois3n                    field_range: 13:13            field_access: rw  
    field_name: ois3                     field_range: 12:12            field_access: rw  
    field_name: ois2n                    field_range: 11:11            field_access: rw  
    field_name: ois2                     field_range: 10:10            field_access: rw  
    field_name: ois1n                    field_range: 9:9              field_access: rw  
    field_name: ois1                     field_range: 8:8              field_access: rw  
    field_name: ti1s                     field_range: 7:7              field_access: rw  
    field_name: mms                      field_range: 6:4              field_access: rw  
    field_name: ccds                     field_range: 3:3              field_access: rw  
    field_name: ccus                     field_range: 2:2              field_access: rw  
    field_name: reserved                 field_range: 1:1              field_access: ro  
    field_name: ccpc                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081004, 0x0, (0x00000000 & 0x7ffd | 0), 0x0, 0x7ffd);

    REG_PWR_Double_Check(0x10081004, 0x0, (0xFFFFFFFF & 0x7ffd | 0), 0x7ffd, 0x7ffd);

    REG_PWR_Double_Check(0x10081004, 0x0, (0x55555555 & 0x7ffd | 0), 0x5555, 0x7ffd);

    REG_PWR_Double_Check(0x10081004, 0x0, (0xAAAAAAAA & 0x7ffd | 0), 0x2aa8, 0x7ffd);

    /*testing for  TIM_SMCR: addr=0x10081008*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etp                      field_range: 17:17            field_access: rw  
    field_name: ece                      field_range: 16:16            field_access: rw  
    field_name: etps                     field_range: 15:14            field_access: rw  
    field_name: etf                      field_range: 13:10            field_access: rw  
    field_name: msm                      field_range: 9:9              field_access: rw  
    field_name: ts                       field_range: 8:4              field_access: rw  
    field_name: sms                      field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081008, 0x0, (0x00000000 & 0x3ffff | 0), 0x0, 0x3ffff);

    REG_PWR_Double_Check(0x10081008, 0x0, (0xFFFFFFFF & 0x3ffff | 0), 0x3ffff, 0x3ffff);

    REG_PWR_Double_Check(0x10081008, 0x0, (0x55555555 & 0x3ffff | 0), 0x15555, 0x3ffff);

    REG_PWR_Double_Check(0x10081008, 0x0, (0xAAAAAAAA & 0x3ffff | 0), 0x2aaaa, 0x3ffff);

    /*testing for  TIM_DIER: addr=0x1008100c*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: tde                      field_range: 14:14            field_access: rw  
    field_name: comde                    field_range: 13:13            field_access: rw  
    field_name: cc4de                    field_range: 12:12            field_access: rw  
    field_name: cc3de                    field_range: 11:11            field_access: rw  
    field_name: cc2de                    field_range: 10:10            field_access: rw  
    field_name: cc1de                    field_range: 9:9              field_access: rw  
    field_name: ude                      field_range: 8:8              field_access: rw  
    field_name: bie                      field_range: 7:7              field_access: rw  
    field_name: tie                      field_range: 6:6              field_access: rw  
    field_name: comie                    field_range: 5:5              field_access: rw  
    field_name: cc4ie                    field_range: 4:4              field_access: rw  
    field_name: cc3ie                    field_range: 3:3              field_access: rw  
    field_name: cc2ie                    field_range: 2:2              field_access: rw  
    field_name: cc1ie                    field_range: 1:1              field_access: rw  
    field_name: uie                      field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1008100c, 0x0, (0x00000000 & 0x7fff | 0), 0x0, 0x7fff);

    REG_PWR_Double_Check(0x1008100c, 0x0, (0xFFFFFFFF & 0x7fff | 0), 0x7fff, 0x7fff);

    REG_PWR_Double_Check(0x1008100c, 0x0, (0x55555555 & 0x7fff | 0), 0x5555, 0x7fff);

    REG_PWR_Double_Check(0x1008100c, 0x0, (0xAAAAAAAA & 0x7fff | 0), 0x2aaa, 0x7fff);

    /*testing for  TIM_SR: addr=0x10081010*/
    /*
    field_name: reserved                 field_range: 31:14            field_access: ro  
    field_name: sbif                     field_range: 13:13            field_access: w0c 
    field_name: cc4of                    field_range: 12:12            field_access: w0c 
    field_name: cc3of                    field_range: 11:11            field_access: w0c 
    field_name: cc2of                    field_range: 10:10            field_access: w0c 
    field_name: cc1of                    field_range: 9:9              field_access: w0c 
    field_name: b2if                     field_range: 8:8              field_access: w0c 
    field_name: bif                      field_range: 7:7              field_access: w0c 
    field_name: tif                      field_range: 6:6              field_access: w0c 
    field_name: comif                    field_range: 5:5              field_access: w0c 
    field_name: cc4if                    field_range: 4:4              field_access: w0c 
    field_name: cc3if                    field_range: 3:3              field_access: w0c 
    field_name: cc2if                    field_range: 2:2              field_access: w0c 
    field_name: cc1if                    field_range: 1:1              field_access: w0c 
    field_name: uif                      field_range: 0:0              field_access: rc  
    */
    REG_PWR_Double_Check(0x10081010, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081010, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081010, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081010, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  TIM_EGR: addr=0x10081014*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: b2g                      field_range: 8:8              field_access: wo  
    field_name: bg                       field_range: 7:7              field_access: wo  
    field_name: tg                       field_range: 6:6              field_access: wo  
    field_name: comg                     field_range: 5:5              field_access: wo  
    field_name: cc4g                     field_range: 4:4              field_access: wo  
    field_name: cc3g                     field_range: 3:3              field_access: wo  
    field_name: cc2g                     field_range: 2:2              field_access: wo  
    field_name: cc1g                     field_range: 1:1              field_access: wo  
    field_name: ug                       field_range: 0:0              field_access: wo  
    */
    REG_PWR_Double_Check(0x10081014, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081014, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081014, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x10081014, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  TIM_CCER: addr=0x10081020*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: co4e                     field_range: 17:17            field_access: ro  
    field_name: co3e                     field_range: 16:16            field_access: rw  
    field_name: co2e                     field_range: 15:15            field_access: rw  
    field_name: co1e                     field_range: 14:14            field_access: rw  
    field_name: cc4p                     field_range: 13:13            field_access: rw  
    field_name: cc4e                     field_range: 12:12            field_access: rw  
    field_name: cc3np                    field_range: 11:11            field_access: rw  
    field_name: cc3ne                    field_range: 10:10            field_access: rw  
    field_name: cc3p                     field_range: 9:9              field_access: rw  
    field_name: cc3e                     field_range: 8:8              field_access: rw  
    field_name: cc2np                    field_range: 7:7              field_access: rw  
    field_name: cc2ne                    field_range: 6:6              field_access: rw  
    field_name: cc2p                     field_range: 5:5              field_access: rw  
    field_name: cc2e                     field_range: 4:4              field_access: rw  
    field_name: cc1np                    field_range: 3:3              field_access: rw  
    field_name: cc1ne                    field_range: 2:2              field_access: rw  
    field_name: cc1p                     field_range: 1:1              field_access: rw  
    field_name: cc1e                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081020, 0x1c000, (0x00000000 & 0x1ffff | 0), 0x0, 0x1ffff);

    REG_PWR_Double_Check(0x10081020, 0x1c000, (0xFFFFFFFF & 0x1ffff | 0), 0x1ffff, 0x1ffff);

    REG_PWR_Double_Check(0x10081020, 0x1c000, (0x55555555 & 0x1ffff | 0), 0x15555, 0x1ffff);

    REG_PWR_Double_Check(0x10081020, 0x1c000, (0xAAAAAAAA & 0x1ffff | 0), 0xaaaa, 0x1ffff);

    /*testing for EWR TIM_CNT: addr=0x10081024*/
    /*
    field_name: cnt                      field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081024, 0x0);

    /*testing for EWR TIM_PSC: addr=0x10081028*/
    /*
    field_name: psc                      field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081028, 0x0);

    /*testing for EWR TIM_ARR: addr=0x1008102c*/
    /*
    field_name: arr                      field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x1008102c, 0xffffffff);

    /*testing for  TIM_RCR: addr=0x10081030*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ic_psc_rep_en            field_range: 14:11            field_access: rw  
    field_name: gc4c                     field_range: 10:8             field_access: rw  
    field_name: rep                      field_range: 7:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081030, 0x0, (0x00000000 & 0x7fff | 0), 0x0, 0x7fff);

    REG_PWR_Double_Check(0x10081030, 0x0, (0xFFFFFFFF & 0x7fff | 0), 0x7fff, 0x7fff);

    REG_PWR_Double_Check(0x10081030, 0x0, (0x55555555 & 0x7fff | 0), 0x5555, 0x7fff);

    REG_PWR_Double_Check(0x10081030, 0x0, (0xAAAAAAAA & 0x7fff | 0), 0x2aaa, 0x7fff);

    /*testing for EWR TIM_CCR1: addr=0x10081034*/
    /*
    field_name: ccr1                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081034, 0x0);

    /*testing for EWR TIM_CCR2: addr=0x10081038*/
    /*
    field_name: ccr2                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081038, 0x0);

    /*testing for EWR TIM_CCR3: addr=0x1008103c*/
    /*
    field_name: ccr3                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x1008103c, 0x0);

    /*testing for EWR TIM_CCR4: addr=0x10081040*/
    /*
    field_name: ccr4                     field_range: 31:0             field_access: rw  
    */
    REG_EWR_Double_Check(0x10081040, 0x0);

    /*testing for  TIM_DCR: addr=0x10081048*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: map_idx                  field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: dbl                      field_range: 12:8             field_access: rw  
    field_name: reserved                 field_range: 7:5              field_access: ro  
    field_name: dba                      field_range: 4:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081048, 0x0, (0x00000000 & 0x1f1f | 0), 0x0, 0x1f1f);

    REG_PWR_Double_Check(0x10081048, 0x0, (0xFFFFFFFF & 0x1f1f | 0), 0x1f1f, 0x1f1f);

    REG_PWR_Double_Check(0x10081048, 0x0, (0x55555555 & 0x1f1f | 0), 0x1515, 0x1f1f);

    REG_PWR_Double_Check(0x10081048, 0x0, (0xAAAAAAAA & 0x1f1f | 0), 0xa0a, 0x1f1f);

    /*testing for RO TIM_VERSION: addr=0x10081050*/
    /*
    field_name: git_ver                  field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x10081050, 0x10010, 0xFFFFFFFF);

    /*testing for EWR TIM_IC_PSC_REP: addr=0x10081054*/
    /*
    field_name: ic4psc_rep               field_range: 31:24            field_access: rw  
    field_name: ic3psc_rep               field_range: 23:16            field_access: rw  
    field_name: ic2psc_rep               field_range: 15:8             field_access: rw  
    field_name: ic1psc_rep               field_range: 7:0              field_access: rw  
    */
    REG_EWR_Double_Check(0x10081054, 0x0);

    /*testing for  TIM_AF1: addr=0x10081058*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etrsel                   field_range: 17:14            field_access: rw  
    field_name: reserved                 field_range: 13:12            field_access: ro  
    field_name: bkcmp2p                  field_range: 11:11            field_access: rw  
    field_name: bkcmp1p                  field_range: 10:10            field_access: rw  
    field_name: bkinp                    field_range: 9:9              field_access: rw  
    field_name: bkdf1bk0e                field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bkcmp2e                  field_range: 2:2              field_access: rw  
    field_name: bkcmp1e                  field_range: 1:1              field_access: rw  
    field_name: bkine                    field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081058, 0x0, (0x00000000 & 0x3cf07 | 0), 0x0, 0x3cf07);

    REG_PWR_Double_Check(0x10081058, 0x0, (0xFFFFFFFF & 0x3cf07 | 0), 0x3cf07, 0x3cf07);

    REG_PWR_Double_Check(0x10081058, 0x0, (0x55555555 & 0x3cf07 | 0), 0x14505, 0x3cf07);

    REG_PWR_Double_Check(0x10081058, 0x0, (0xAAAAAAAA & 0x3cf07 | 0), 0x28a02, 0x3cf07);

    /*testing for  TIM_AF2: addr=0x1008105c*/
    /*
    field_name: reserved                 field_range: 31:12            field_access: ro  
    field_name: bk2cmp2p                 field_range: 11:11            field_access: rw  
    field_name: bk2cmp1p                 field_range: 10:10            field_access: rw  
    field_name: bk2inp                   field_range: 9:9              field_access: rw  
    field_name: bk2df1bk1e               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bk2cmp2e                 field_range: 2:2              field_access: rw  
    field_name: bk2cmp1e                 field_range: 1:1              field_access: rw  
    field_name: bk2ine                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1008105c, 0x0, (0x00000000 & 0xf07 | 0), 0x0, 0xf07);

    REG_PWR_Double_Check(0x1008105c, 0x0, (0xFFFFFFFF & 0xf07 | 0), 0xf07, 0xf07);

    REG_PWR_Double_Check(0x1008105c, 0x0, (0x55555555 & 0xf07 | 0), 0x505, 0xf07);

    REG_PWR_Double_Check(0x1008105c, 0x0, (0xAAAAAAAA & 0xf07 | 0), 0xa02, 0xf07);

    /*testing for  TIM_TISEL: addr=0x10081060*/
    /*
    field_name: reserved                 field_range: 31:28            field_access: ro  
    field_name: ti4sel                   field_range: 27:24            field_access: rw  
    field_name: reserved                 field_range: 23:20            field_access: ro  
    field_name: ti3sel                   field_range: 19:16            field_access: rw  
    field_name: reserved                 field_range: 15:12            field_access: ro  
    field_name: ti2sel                   field_range: 11:8             field_access: rw  
    field_name: reserved                 field_range: 7:4              field_access: ro  
    field_name: ti1sel                   field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x10081060, 0x0, (0x00000000 & 0xf0f0f0f | 0), 0x0, 0xf0f0f0f);

    REG_PWR_Double_Check(0x10081060, 0x0, (0xFFFFFFFF & 0xf0f0f0f | 0), 0xf0f0f0f, 0xf0f0f0f);

    REG_PWR_Double_Check(0x10081060, 0x0, (0x55555555 & 0xf0f0f0f | 0), 0x5050505, 0xf0f0f0f);

    REG_PWR_Double_Check(0x10081060, 0x0, (0xAAAAAAAA & 0xf0f0f0f | 0), 0xa0a0a0a, 0xf0f0f0f);

    /*testing for EWR TIM_OUTPUT_DELAY: addr=0x10081064*/
    /*
    field_name: oc4_delay                field_range: 31:24            field_access: rw  
    field_name: oc3_delay                field_range: 23:16            field_access: rw  
    field_name: oc2_delay                field_range: 15:8             field_access: rw  
    field_name: oc1_delay                field_range: 7:0              field_access: rw  
    */
    REG_EWR_Double_Check(0x10081064, 0x0);

    delta_mcycle2=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle2;

    advanced_timer1_clk_div(0);

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
    #ifdef MISC_HAS_ADVANCED_TIMER1_HAS_CLK

    advanced_timer1_clk_en(DISABLE);

    /*testing for  TIM_CR1: addr=0x10081000*/
    /*
    field_name: reserved                 field_range: 31:10            field_access: ro  
    field_name: ckd                      field_range: 9:8              field_access: rw  
    field_name: arpe                     field_range: 7:7              field_access: rw  
    field_name: cms                      field_range: 6:5              field_access: rw  
    field_name: dir                      field_range: 4:4              field_access: rw  
    field_name: opm                      field_range: 3:3              field_access: rw  
    field_name: urs                      field_range: 2:2              field_access: rw  
    field_name: udis                     field_range: 1:1              field_access: rw  
    field_name: cen                      field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081000);

    /*testing for  TIM_CR2: addr=0x10081004*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ois4                     field_range: 14:14            field_access: rw  
    field_name: ois3n                    field_range: 13:13            field_access: rw  
    field_name: ois3                     field_range: 12:12            field_access: rw  
    field_name: ois2n                    field_range: 11:11            field_access: rw  
    field_name: ois2                     field_range: 10:10            field_access: rw  
    field_name: ois1n                    field_range: 9:9              field_access: rw  
    field_name: ois1                     field_range: 8:8              field_access: rw  
    field_name: ti1s                     field_range: 7:7              field_access: rw  
    field_name: mms                      field_range: 6:4              field_access: rw  
    field_name: ccds                     field_range: 3:3              field_access: rw  
    field_name: ccus                     field_range: 2:2              field_access: rw  
    field_name: reserved                 field_range: 1:1              field_access: ro  
    field_name: ccpc                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081004);

    /*testing for  TIM_SMCR: addr=0x10081008*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etp                      field_range: 17:17            field_access: rw  
    field_name: ece                      field_range: 16:16            field_access: rw  
    field_name: etps                     field_range: 15:14            field_access: rw  
    field_name: etf                      field_range: 13:10            field_access: rw  
    field_name: msm                      field_range: 9:9              field_access: rw  
    field_name: ts                       field_range: 8:4              field_access: rw  
    field_name: sms                      field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081008);

    /*testing for  TIM_DIER: addr=0x1008100c*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: tde                      field_range: 14:14            field_access: rw  
    field_name: comde                    field_range: 13:13            field_access: rw  
    field_name: cc4de                    field_range: 12:12            field_access: rw  
    field_name: cc3de                    field_range: 11:11            field_access: rw  
    field_name: cc2de                    field_range: 10:10            field_access: rw  
    field_name: cc1de                    field_range: 9:9              field_access: rw  
    field_name: ude                      field_range: 8:8              field_access: rw  
    field_name: bie                      field_range: 7:7              field_access: rw  
    field_name: tie                      field_range: 6:6              field_access: rw  
    field_name: comie                    field_range: 5:5              field_access: rw  
    field_name: cc4ie                    field_range: 4:4              field_access: rw  
    field_name: cc3ie                    field_range: 3:3              field_access: rw  
    field_name: cc2ie                    field_range: 2:2              field_access: rw  
    field_name: cc1ie                    field_range: 1:1              field_access: rw  
    field_name: uie                      field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008100c);

    /*testing for  TIM_SR: addr=0x10081010*/
    /*
    field_name: reserved                 field_range: 31:14            field_access: ro  
    field_name: sbif                     field_range: 13:13            field_access: w0c 
    field_name: cc4of                    field_range: 12:12            field_access: w0c 
    field_name: cc3of                    field_range: 11:11            field_access: w0c 
    field_name: cc2of                    field_range: 10:10            field_access: w0c 
    field_name: cc1of                    field_range: 9:9              field_access: w0c 
    field_name: b2if                     field_range: 8:8              field_access: w0c 
    field_name: bif                      field_range: 7:7              field_access: w0c 
    field_name: tif                      field_range: 6:6              field_access: w0c 
    field_name: comif                    field_range: 5:5              field_access: w0c 
    field_name: cc4if                    field_range: 4:4              field_access: w0c 
    field_name: cc3if                    field_range: 3:3              field_access: w0c 
    field_name: cc2if                    field_range: 2:2              field_access: w0c 
    field_name: cc1if                    field_range: 1:1              field_access: w0c 
    field_name: uif                      field_range: 0:0              field_access: rc  
    */
    REG_CLK_DIS_CHECK(0x10081010);

    /*testing for  TIM_EGR: addr=0x10081014*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: b2g                      field_range: 8:8              field_access: wo  
    field_name: bg                       field_range: 7:7              field_access: wo  
    field_name: tg                       field_range: 6:6              field_access: wo  
    field_name: comg                     field_range: 5:5              field_access: wo  
    field_name: cc4g                     field_range: 4:4              field_access: wo  
    field_name: cc3g                     field_range: 3:3              field_access: wo  
    field_name: cc2g                     field_range: 2:2              field_access: wo  
    field_name: cc1g                     field_range: 1:1              field_access: wo  
    field_name: ug                       field_range: 0:0              field_access: wo  
    */
    REG_CLK_DIS_CHECK(0x10081014);

    /*testing for  TIM_CCER: addr=0x10081020*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: co4e                     field_range: 17:17            field_access: ro  
    field_name: co3e                     field_range: 16:16            field_access: rw  
    field_name: co2e                     field_range: 15:15            field_access: rw  
    field_name: co1e                     field_range: 14:14            field_access: rw  
    field_name: cc4p                     field_range: 13:13            field_access: rw  
    field_name: cc4e                     field_range: 12:12            field_access: rw  
    field_name: cc3np                    field_range: 11:11            field_access: rw  
    field_name: cc3ne                    field_range: 10:10            field_access: rw  
    field_name: cc3p                     field_range: 9:9              field_access: rw  
    field_name: cc3e                     field_range: 8:8              field_access: rw  
    field_name: cc2np                    field_range: 7:7              field_access: rw  
    field_name: cc2ne                    field_range: 6:6              field_access: rw  
    field_name: cc2p                     field_range: 5:5              field_access: rw  
    field_name: cc2e                     field_range: 4:4              field_access: rw  
    field_name: cc1np                    field_range: 3:3              field_access: rw  
    field_name: cc1ne                    field_range: 2:2              field_access: rw  
    field_name: cc1p                     field_range: 1:1              field_access: rw  
    field_name: cc1e                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081020);

    /*testing for EWR TIM_CNT: addr=0x10081024*/
    /*
    field_name: cnt                      field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081024);

    /*testing for EWR TIM_PSC: addr=0x10081028*/
    /*
    field_name: psc                      field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081028);

    /*testing for EWR TIM_ARR: addr=0x1008102c*/
    /*
    field_name: arr                      field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008102c);

    /*testing for  TIM_RCR: addr=0x10081030*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ic_psc_rep_en            field_range: 14:11            field_access: rw  
    field_name: gc4c                     field_range: 10:8             field_access: rw  
    field_name: rep                      field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081030);

    /*testing for EWR TIM_CCR1: addr=0x10081034*/
    /*
    field_name: ccr1                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081034);

    /*testing for EWR TIM_CCR2: addr=0x10081038*/
    /*
    field_name: ccr2                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081038);

    /*testing for EWR TIM_CCR3: addr=0x1008103c*/
    /*
    field_name: ccr3                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008103c);

    /*testing for EWR TIM_CCR4: addr=0x10081040*/
    /*
    field_name: ccr4                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081040);

    /*testing for  TIM_DCR: addr=0x10081048*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: map_idx                  field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: dbl                      field_range: 12:8             field_access: rw  
    field_name: reserved                 field_range: 7:5              field_access: ro  
    field_name: dba                      field_range: 4:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081048);

    /*testing for RO TIM_VERSION: addr=0x10081050*/
    /*
    field_name: git_ver                  field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10081050);

    /*testing for EWR TIM_IC_PSC_REP: addr=0x10081054*/
    /*
    field_name: ic4psc_rep               field_range: 31:24            field_access: rw  
    field_name: ic3psc_rep               field_range: 23:16            field_access: rw  
    field_name: ic2psc_rep               field_range: 15:8             field_access: rw  
    field_name: ic1psc_rep               field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081054);

    /*testing for  TIM_AF1: addr=0x10081058*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etrsel                   field_range: 17:14            field_access: rw  
    field_name: reserved                 field_range: 13:12            field_access: ro  
    field_name: bkcmp2p                  field_range: 11:11            field_access: rw  
    field_name: bkcmp1p                  field_range: 10:10            field_access: rw  
    field_name: bkinp                    field_range: 9:9              field_access: rw  
    field_name: bkdf1bk0e                field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bkcmp2e                  field_range: 2:2              field_access: rw  
    field_name: bkcmp1e                  field_range: 1:1              field_access: rw  
    field_name: bkine                    field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081058);

    /*testing for  TIM_AF2: addr=0x1008105c*/
    /*
    field_name: reserved                 field_range: 31:12            field_access: ro  
    field_name: bk2cmp2p                 field_range: 11:11            field_access: rw  
    field_name: bk2cmp1p                 field_range: 10:10            field_access: rw  
    field_name: bk2inp                   field_range: 9:9              field_access: rw  
    field_name: bk2df1bk1e               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bk2cmp2e                 field_range: 2:2              field_access: rw  
    field_name: bk2cmp1e                 field_range: 1:1              field_access: rw  
    field_name: bk2ine                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008105c);

    /*testing for  TIM_TISEL: addr=0x10081060*/
    /*
    field_name: reserved                 field_range: 31:28            field_access: ro  
    field_name: ti4sel                   field_range: 27:24            field_access: rw  
    field_name: reserved                 field_range: 23:20            field_access: ro  
    field_name: ti3sel                   field_range: 19:16            field_access: rw  
    field_name: reserved                 field_range: 15:12            field_access: ro  
    field_name: ti2sel                   field_range: 11:8             field_access: rw  
    field_name: reserved                 field_range: 7:4              field_access: ro  
    field_name: ti1sel                   field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081060);

    /*testing for EWR TIM_OUTPUT_DELAY: addr=0x10081064*/
    /*
    field_name: oc4_delay                field_range: 31:24            field_access: rw  
    field_name: oc3_delay                field_range: 23:16            field_access: rw  
    field_name: oc2_delay                field_range: 15:8             field_access: rw  
    field_name: oc1_delay                field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081064);

    advanced_timer1_clk_en(ENABLE);

    #endif

    /*********************TEST FOR CLOCK RESET*********************/

    #ifdef MISC_HAS_ADVANCED_TIMER1_RST

    advanced_timer1_set_rst(DISABLE);

    /*testing for  TIM_CR1: addr=0x10081000*/
    /*
    field_name: reserved                 field_range: 31:10            field_access: ro  
    field_name: ckd                      field_range: 9:8              field_access: rw  
    field_name: arpe                     field_range: 7:7              field_access: rw  
    field_name: cms                      field_range: 6:5              field_access: rw  
    field_name: dir                      field_range: 4:4              field_access: rw  
    field_name: opm                      field_range: 3:3              field_access: rw  
    field_name: urs                      field_range: 2:2              field_access: rw  
    field_name: udis                     field_range: 1:1              field_access: rw  
    field_name: cen                      field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081000);

    /*testing for  TIM_CR2: addr=0x10081004*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ois4                     field_range: 14:14            field_access: rw  
    field_name: ois3n                    field_range: 13:13            field_access: rw  
    field_name: ois3                     field_range: 12:12            field_access: rw  
    field_name: ois2n                    field_range: 11:11            field_access: rw  
    field_name: ois2                     field_range: 10:10            field_access: rw  
    field_name: ois1n                    field_range: 9:9              field_access: rw  
    field_name: ois1                     field_range: 8:8              field_access: rw  
    field_name: ti1s                     field_range: 7:7              field_access: rw  
    field_name: mms                      field_range: 6:4              field_access: rw  
    field_name: ccds                     field_range: 3:3              field_access: rw  
    field_name: ccus                     field_range: 2:2              field_access: rw  
    field_name: reserved                 field_range: 1:1              field_access: ro  
    field_name: ccpc                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081004);

    /*testing for  TIM_SMCR: addr=0x10081008*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etp                      field_range: 17:17            field_access: rw  
    field_name: ece                      field_range: 16:16            field_access: rw  
    field_name: etps                     field_range: 15:14            field_access: rw  
    field_name: etf                      field_range: 13:10            field_access: rw  
    field_name: msm                      field_range: 9:9              field_access: rw  
    field_name: ts                       field_range: 8:4              field_access: rw  
    field_name: sms                      field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081008);

    /*testing for  TIM_DIER: addr=0x1008100c*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: tde                      field_range: 14:14            field_access: rw  
    field_name: comde                    field_range: 13:13            field_access: rw  
    field_name: cc4de                    field_range: 12:12            field_access: rw  
    field_name: cc3de                    field_range: 11:11            field_access: rw  
    field_name: cc2de                    field_range: 10:10            field_access: rw  
    field_name: cc1de                    field_range: 9:9              field_access: rw  
    field_name: ude                      field_range: 8:8              field_access: rw  
    field_name: bie                      field_range: 7:7              field_access: rw  
    field_name: tie                      field_range: 6:6              field_access: rw  
    field_name: comie                    field_range: 5:5              field_access: rw  
    field_name: cc4ie                    field_range: 4:4              field_access: rw  
    field_name: cc3ie                    field_range: 3:3              field_access: rw  
    field_name: cc2ie                    field_range: 2:2              field_access: rw  
    field_name: cc1ie                    field_range: 1:1              field_access: rw  
    field_name: uie                      field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008100c);

    /*testing for  TIM_SR: addr=0x10081010*/
    /*
    field_name: reserved                 field_range: 31:14            field_access: ro  
    field_name: sbif                     field_range: 13:13            field_access: w0c 
    field_name: cc4of                    field_range: 12:12            field_access: w0c 
    field_name: cc3of                    field_range: 11:11            field_access: w0c 
    field_name: cc2of                    field_range: 10:10            field_access: w0c 
    field_name: cc1of                    field_range: 9:9              field_access: w0c 
    field_name: b2if                     field_range: 8:8              field_access: w0c 
    field_name: bif                      field_range: 7:7              field_access: w0c 
    field_name: tif                      field_range: 6:6              field_access: w0c 
    field_name: comif                    field_range: 5:5              field_access: w0c 
    field_name: cc4if                    field_range: 4:4              field_access: w0c 
    field_name: cc3if                    field_range: 3:3              field_access: w0c 
    field_name: cc2if                    field_range: 2:2              field_access: w0c 
    field_name: cc1if                    field_range: 1:1              field_access: w0c 
    field_name: uif                      field_range: 0:0              field_access: rc  
    */
    REG_CLK_DIS_CHECK(0x10081010);

    /*testing for  TIM_EGR: addr=0x10081014*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: b2g                      field_range: 8:8              field_access: wo  
    field_name: bg                       field_range: 7:7              field_access: wo  
    field_name: tg                       field_range: 6:6              field_access: wo  
    field_name: comg                     field_range: 5:5              field_access: wo  
    field_name: cc4g                     field_range: 4:4              field_access: wo  
    field_name: cc3g                     field_range: 3:3              field_access: wo  
    field_name: cc2g                     field_range: 2:2              field_access: wo  
    field_name: cc1g                     field_range: 1:1              field_access: wo  
    field_name: ug                       field_range: 0:0              field_access: wo  
    */
    REG_CLK_DIS_CHECK(0x10081014);

    /*testing for  TIM_CCER: addr=0x10081020*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: co4e                     field_range: 17:17            field_access: ro  
    field_name: co3e                     field_range: 16:16            field_access: rw  
    field_name: co2e                     field_range: 15:15            field_access: rw  
    field_name: co1e                     field_range: 14:14            field_access: rw  
    field_name: cc4p                     field_range: 13:13            field_access: rw  
    field_name: cc4e                     field_range: 12:12            field_access: rw  
    field_name: cc3np                    field_range: 11:11            field_access: rw  
    field_name: cc3ne                    field_range: 10:10            field_access: rw  
    field_name: cc3p                     field_range: 9:9              field_access: rw  
    field_name: cc3e                     field_range: 8:8              field_access: rw  
    field_name: cc2np                    field_range: 7:7              field_access: rw  
    field_name: cc2ne                    field_range: 6:6              field_access: rw  
    field_name: cc2p                     field_range: 5:5              field_access: rw  
    field_name: cc2e                     field_range: 4:4              field_access: rw  
    field_name: cc1np                    field_range: 3:3              field_access: rw  
    field_name: cc1ne                    field_range: 2:2              field_access: rw  
    field_name: cc1p                     field_range: 1:1              field_access: rw  
    field_name: cc1e                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081020);

    /*testing for EWR TIM_CNT: addr=0x10081024*/
    /*
    field_name: cnt                      field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081024);

    /*testing for EWR TIM_PSC: addr=0x10081028*/
    /*
    field_name: psc                      field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081028);

    /*testing for EWR TIM_ARR: addr=0x1008102c*/
    /*
    field_name: arr                      field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008102c);

    /*testing for  TIM_RCR: addr=0x10081030*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ic_psc_rep_en            field_range: 14:11            field_access: rw  
    field_name: gc4c                     field_range: 10:8             field_access: rw  
    field_name: rep                      field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081030);

    /*testing for EWR TIM_CCR1: addr=0x10081034*/
    /*
    field_name: ccr1                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081034);

    /*testing for EWR TIM_CCR2: addr=0x10081038*/
    /*
    field_name: ccr2                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081038);

    /*testing for EWR TIM_CCR3: addr=0x1008103c*/
    /*
    field_name: ccr3                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008103c);

    /*testing for EWR TIM_CCR4: addr=0x10081040*/
    /*
    field_name: ccr4                     field_range: 31:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081040);

    /*testing for  TIM_DCR: addr=0x10081048*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: map_idx                  field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: dbl                      field_range: 12:8             field_access: rw  
    field_name: reserved                 field_range: 7:5              field_access: ro  
    field_name: dba                      field_range: 4:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081048);

    /*testing for RO TIM_VERSION: addr=0x10081050*/
    /*
    field_name: git_ver                  field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x10081050);

    /*testing for EWR TIM_IC_PSC_REP: addr=0x10081054*/
    /*
    field_name: ic4psc_rep               field_range: 31:24            field_access: rw  
    field_name: ic3psc_rep               field_range: 23:16            field_access: rw  
    field_name: ic2psc_rep               field_range: 15:8             field_access: rw  
    field_name: ic1psc_rep               field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081054);

    /*testing for  TIM_AF1: addr=0x10081058*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etrsel                   field_range: 17:14            field_access: rw  
    field_name: reserved                 field_range: 13:12            field_access: ro  
    field_name: bkcmp2p                  field_range: 11:11            field_access: rw  
    field_name: bkcmp1p                  field_range: 10:10            field_access: rw  
    field_name: bkinp                    field_range: 9:9              field_access: rw  
    field_name: bkdf1bk0e                field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bkcmp2e                  field_range: 2:2              field_access: rw  
    field_name: bkcmp1e                  field_range: 1:1              field_access: rw  
    field_name: bkine                    field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081058);

    /*testing for  TIM_AF2: addr=0x1008105c*/
    /*
    field_name: reserved                 field_range: 31:12            field_access: ro  
    field_name: bk2cmp2p                 field_range: 11:11            field_access: rw  
    field_name: bk2cmp1p                 field_range: 10:10            field_access: rw  
    field_name: bk2inp                   field_range: 9:9              field_access: rw  
    field_name: bk2df1bk1e               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bk2cmp2e                 field_range: 2:2              field_access: rw  
    field_name: bk2cmp1e                 field_range: 1:1              field_access: rw  
    field_name: bk2ine                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1008105c);

    /*testing for  TIM_TISEL: addr=0x10081060*/
    /*
    field_name: reserved                 field_range: 31:28            field_access: ro  
    field_name: ti4sel                   field_range: 27:24            field_access: rw  
    field_name: reserved                 field_range: 23:20            field_access: ro  
    field_name: ti3sel                   field_range: 19:16            field_access: rw  
    field_name: reserved                 field_range: 15:12            field_access: ro  
    field_name: ti2sel                   field_range: 11:8             field_access: rw  
    field_name: reserved                 field_range: 7:4              field_access: ro  
    field_name: ti1sel                   field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081060);

    /*testing for EWR TIM_OUTPUT_DELAY: addr=0x10081064*/
    /*
    field_name: oc4_delay                field_range: 31:24            field_access: rw  
    field_name: oc3_delay                field_range: 23:16            field_access: rw  
    field_name: oc2_delay                field_range: 15:8             field_access: rw  
    field_name: oc1_delay                field_range: 7:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x10081064);

    advanced_timer1_set_rst(ENABLE);

    /*testing for  TIM_CR1: addr=0x10081000*/
    /*
    field_name: reserved                 field_range: 31:10            field_access: ro  
    field_name: ckd                      field_range: 9:8              field_access: rw  
    field_name: arpe                     field_range: 7:7              field_access: rw  
    field_name: cms                      field_range: 6:5              field_access: rw  
    field_name: dir                      field_range: 4:4              field_access: rw  
    field_name: opm                      field_range: 3:3              field_access: rw  
    field_name: urs                      field_range: 2:2              field_access: rw  
    field_name: udis                     field_range: 1:1              field_access: rw  
    field_name: cen                      field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081000,0x0);

    /*testing for  TIM_CR2: addr=0x10081004*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ois4                     field_range: 14:14            field_access: rw  
    field_name: ois3n                    field_range: 13:13            field_access: rw  
    field_name: ois3                     field_range: 12:12            field_access: rw  
    field_name: ois2n                    field_range: 11:11            field_access: rw  
    field_name: ois2                     field_range: 10:10            field_access: rw  
    field_name: ois1n                    field_range: 9:9              field_access: rw  
    field_name: ois1                     field_range: 8:8              field_access: rw  
    field_name: ti1s                     field_range: 7:7              field_access: rw  
    field_name: mms                      field_range: 6:4              field_access: rw  
    field_name: ccds                     field_range: 3:3              field_access: rw  
    field_name: ccus                     field_range: 2:2              field_access: rw  
    field_name: reserved                 field_range: 1:1              field_access: ro  
    field_name: ccpc                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081004,0x0);

    /*testing for  TIM_SMCR: addr=0x10081008*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etp                      field_range: 17:17            field_access: rw  
    field_name: ece                      field_range: 16:16            field_access: rw  
    field_name: etps                     field_range: 15:14            field_access: rw  
    field_name: etf                      field_range: 13:10            field_access: rw  
    field_name: msm                      field_range: 9:9              field_access: rw  
    field_name: ts                       field_range: 8:4              field_access: rw  
    field_name: sms                      field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081008,0x0);

    /*testing for  TIM_DIER: addr=0x1008100c*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: tde                      field_range: 14:14            field_access: rw  
    field_name: comde                    field_range: 13:13            field_access: rw  
    field_name: cc4de                    field_range: 12:12            field_access: rw  
    field_name: cc3de                    field_range: 11:11            field_access: rw  
    field_name: cc2de                    field_range: 10:10            field_access: rw  
    field_name: cc1de                    field_range: 9:9              field_access: rw  
    field_name: ude                      field_range: 8:8              field_access: rw  
    field_name: bie                      field_range: 7:7              field_access: rw  
    field_name: tie                      field_range: 6:6              field_access: rw  
    field_name: comie                    field_range: 5:5              field_access: rw  
    field_name: cc4ie                    field_range: 4:4              field_access: rw  
    field_name: cc3ie                    field_range: 3:3              field_access: rw  
    field_name: cc2ie                    field_range: 2:2              field_access: rw  
    field_name: cc1ie                    field_range: 1:1              field_access: rw  
    field_name: uie                      field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1008100c,0x0);

    /*testing for  TIM_SR: addr=0x10081010*/
    /*
    field_name: reserved                 field_range: 31:14            field_access: ro  
    field_name: sbif                     field_range: 13:13            field_access: w0c 
    field_name: cc4of                    field_range: 12:12            field_access: w0c 
    field_name: cc3of                    field_range: 11:11            field_access: w0c 
    field_name: cc2of                    field_range: 10:10            field_access: w0c 
    field_name: cc1of                    field_range: 9:9              field_access: w0c 
    field_name: b2if                     field_range: 8:8              field_access: w0c 
    field_name: bif                      field_range: 7:7              field_access: w0c 
    field_name: tif                      field_range: 6:6              field_access: w0c 
    field_name: comif                    field_range: 5:5              field_access: w0c 
    field_name: cc4if                    field_range: 4:4              field_access: w0c 
    field_name: cc3if                    field_range: 3:3              field_access: w0c 
    field_name: cc2if                    field_range: 2:2              field_access: w0c 
    field_name: cc1if                    field_range: 1:1              field_access: w0c 
    field_name: uif                      field_range: 0:0              field_access: rc  
    */
    REG_DefVal_CHECK(0x10081010,0x0);

    /*testing for  TIM_EGR: addr=0x10081014*/
    /*
    field_name: reserved                 field_range: 31:9             field_access: ro  
    field_name: b2g                      field_range: 8:8              field_access: wo  
    field_name: bg                       field_range: 7:7              field_access: wo  
    field_name: tg                       field_range: 6:6              field_access: wo  
    field_name: comg                     field_range: 5:5              field_access: wo  
    field_name: cc4g                     field_range: 4:4              field_access: wo  
    field_name: cc3g                     field_range: 3:3              field_access: wo  
    field_name: cc2g                     field_range: 2:2              field_access: wo  
    field_name: cc1g                     field_range: 1:1              field_access: wo  
    field_name: ug                       field_range: 0:0              field_access: wo  
    */
    REG_DefVal_CHECK(0x10081014,0x0);

    /*testing for  TIM_CCER: addr=0x10081020*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: co4e                     field_range: 17:17            field_access: ro  
    field_name: co3e                     field_range: 16:16            field_access: rw  
    field_name: co2e                     field_range: 15:15            field_access: rw  
    field_name: co1e                     field_range: 14:14            field_access: rw  
    field_name: cc4p                     field_range: 13:13            field_access: rw  
    field_name: cc4e                     field_range: 12:12            field_access: rw  
    field_name: cc3np                    field_range: 11:11            field_access: rw  
    field_name: cc3ne                    field_range: 10:10            field_access: rw  
    field_name: cc3p                     field_range: 9:9              field_access: rw  
    field_name: cc3e                     field_range: 8:8              field_access: rw  
    field_name: cc2np                    field_range: 7:7              field_access: rw  
    field_name: cc2ne                    field_range: 6:6              field_access: rw  
    field_name: cc2p                     field_range: 5:5              field_access: rw  
    field_name: cc2e                     field_range: 4:4              field_access: rw  
    field_name: cc1np                    field_range: 3:3              field_access: rw  
    field_name: cc1ne                    field_range: 2:2              field_access: rw  
    field_name: cc1p                     field_range: 1:1              field_access: rw  
    field_name: cc1e                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081020,0x1c000);

    /*testing for EWR TIM_CNT: addr=0x10081024*/
    /*
    field_name: cnt                      field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081024,0x0);

    /*testing for EWR TIM_PSC: addr=0x10081028*/
    /*
    field_name: psc                      field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081028,0x0);

    /*testing for EWR TIM_ARR: addr=0x1008102c*/
    /*
    field_name: arr                      field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x1008102c,0xffffffff);

    /*testing for  TIM_RCR: addr=0x10081030*/
    /*
    field_name: reserved                 field_range: 31:15            field_access: ro  
    field_name: ic_psc_rep_en            field_range: 14:11            field_access: rw  
    field_name: gc4c                     field_range: 10:8             field_access: rw  
    field_name: rep                      field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081030,0x0);

    /*testing for EWR TIM_CCR1: addr=0x10081034*/
    /*
    field_name: ccr1                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081034,0x0);

    /*testing for EWR TIM_CCR2: addr=0x10081038*/
    /*
    field_name: ccr2                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081038,0x0);

    /*testing for EWR TIM_CCR3: addr=0x1008103c*/
    /*
    field_name: ccr3                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x1008103c,0x0);

    /*testing for EWR TIM_CCR4: addr=0x10081040*/
    /*
    field_name: ccr4                     field_range: 31:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x10081040,0x0);

    /*testing for  TIM_DCR: addr=0x10081048*/
    /*
    field_name: reserved                 field_range: 31:21            field_access: ro  
    field_name: map_idx                  field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: dbl                      field_range: 12:8             field_access: rw  
    field_name: reserved                 field_range: 7:5              field_access: ro  
    field_name: dba                      field_range: 4:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081048,0x0);

    /*testing for RO TIM_VERSION: addr=0x10081050*/
    /*
    field_name: git_ver                  field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x10081050,0x10010);

    /*testing for EWR TIM_IC_PSC_REP: addr=0x10081054*/
    /*
    field_name: ic4psc_rep               field_range: 31:24            field_access: rw  
    field_name: ic3psc_rep               field_range: 23:16            field_access: rw  
    field_name: ic2psc_rep               field_range: 15:8             field_access: rw  
    field_name: ic1psc_rep               field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081054,0x0);

    /*testing for  TIM_AF1: addr=0x10081058*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: etrsel                   field_range: 17:14            field_access: rw  
    field_name: reserved                 field_range: 13:12            field_access: ro  
    field_name: bkcmp2p                  field_range: 11:11            field_access: rw  
    field_name: bkcmp1p                  field_range: 10:10            field_access: rw  
    field_name: bkinp                    field_range: 9:9              field_access: rw  
    field_name: bkdf1bk0e                field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bkcmp2e                  field_range: 2:2              field_access: rw  
    field_name: bkcmp1e                  field_range: 1:1              field_access: rw  
    field_name: bkine                    field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081058,0x0);

    /*testing for  TIM_AF2: addr=0x1008105c*/
    /*
    field_name: reserved                 field_range: 31:12            field_access: ro  
    field_name: bk2cmp2p                 field_range: 11:11            field_access: rw  
    field_name: bk2cmp1p                 field_range: 10:10            field_access: rw  
    field_name: bk2inp                   field_range: 9:9              field_access: rw  
    field_name: bk2df1bk1e               field_range: 8:8              field_access: rw  
    field_name: reserved                 field_range: 7:3              field_access: ro  
    field_name: bk2cmp2e                 field_range: 2:2              field_access: rw  
    field_name: bk2cmp1e                 field_range: 1:1              field_access: rw  
    field_name: bk2ine                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1008105c,0x0);

    /*testing for  TIM_TISEL: addr=0x10081060*/
    /*
    field_name: reserved                 field_range: 31:28            field_access: ro  
    field_name: ti4sel                   field_range: 27:24            field_access: rw  
    field_name: reserved                 field_range: 23:20            field_access: ro  
    field_name: ti3sel                   field_range: 19:16            field_access: rw  
    field_name: reserved                 field_range: 15:12            field_access: ro  
    field_name: ti2sel                   field_range: 11:8             field_access: rw  
    field_name: reserved                 field_range: 7:4              field_access: ro  
    field_name: ti1sel                   field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081060,0x0);

    /*testing for EWR TIM_OUTPUT_DELAY: addr=0x10081064*/
    /*
    field_name: oc4_delay                field_range: 31:24            field_access: rw  
    field_name: oc3_delay                field_range: 23:16            field_access: rw  
    field_name: oc2_delay                field_range: 15:8             field_access: rw  
    field_name: oc1_delay                field_range: 7:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x10081064,0x0);

    #endif

    #ifdef MISC_HAS_ADVANCED_TIMER1_CLK_DIV

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
