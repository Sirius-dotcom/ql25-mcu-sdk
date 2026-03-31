
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
    #ifdef MISC_HAS_XKAN1_HAS_CLK

        xkan1_clk_en(ENABLE);

    #endif

    /*********************TEST FOR POR DEVAL *********************/

    /*testing for RO RELEASE: addr=0x11001000*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x11001000,0x20201);

    /*testing for  CFG_MCR: addr=0x11001004*/
    /*
    field_name: reserved                 field_range: 31:19            field_access: ro  
    field_name: protocol_exception_handling field_range: 18:17            field_access: rw  
    field_name: be_receiver_in_loss      field_range: 16:16            field_access: rw  
    field_name: restricted_operation     field_range: 15:15            field_access: rw  
    field_name: rx_pin_pull_up           field_range: 14:14            field_access: rw  
    field_name: tx_pin_pull_up           field_range: 13:13            field_access: rw  
    field_name: clk_stop_ack             field_range: 12:12            field_access: ro  
    field_name: clk_stop_req             field_range: 11:11            field_access: rw  
    field_name: reserved                 field_range: 10:10            field_access: ro  
    field_name: txpause                  field_range: 9:9              field_access: rw  
    field_name: brs_en                   field_range: 8:8              field_access: rw  
    field_name: fd_en                    field_range: 7:7              field_access: rw  
    field_name: timestamp_en             field_range: 6:6              field_access: rw  
    field_name: reserved                 field_range: 5:5              field_access: ro  
    field_name: auto_retrans_en          field_range: 4:4              field_access: rw  
    field_name: tdc_en                   field_range: 3:3              field_access: rw  
    field_name: sleep_req                field_range: 2:2              field_access: rw  
    field_name: initial_req              field_range: 1:1              field_access: rw  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001004,0x36004);

    /*testing for  CFG_TEST: addr=0x11001008*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: silm                     field_range: 1:1              field_access: rw  
    field_name: lbkm                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001008,0x0);

    /*testing for  CFG_PSR: addr=0x1100100c*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: trans_delay_comp         field_range: 22:16            field_access: ro  
    field_name: reserved                 field_range: 15:5             field_access: ro  
    field_name: pxe                      field_range: 4:4              field_access: w1c 
    field_name: esi                      field_range: 3:3              field_access: w1c 
    field_name: brs                      field_range: 2:2              field_access: w1c 
    field_name: edl                      field_range: 1:1              field_access: w1c 
    field_name: ide                      field_range: 0:0              field_access: w1c 
    */
    REG_DefVal_CHECK(0x1100100c,0x0);

    /*testing for  CFG_GLOBAL_FILTER: addr=0x11001014*/
    /*
    field_name: reserved                 field_range: 31:6             field_access: ro  
    field_name: accept_miss_st_frame     field_range: 5:4              field_access: rw  
    field_name: accept_miss_ex_frame     field_range: 3:2              field_access: rw  
    field_name: reject_st_remote         field_range: 1:1              field_access: rw  
    field_name: reject_ex_remote         field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001014,0x3c);

    /*testing for  CFG_ST_FILTER: addr=0x11001018*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: stflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: stflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001018,0x0);

    /*testing for  CFG_EX_FILTER: addr=0x1100101c*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: exflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: exflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x1100101c,0x0);

    /*testing for  CFG_STIDM: addr=0x11001020*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: sidm                     field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001020,0x7ff);

    /*testing for  CFG_EXIDM: addr=0x11001024*/
    /*
    field_name: reserved                 field_range: 31:29            field_access: ro  
    field_name: eidm                     field_range: 28:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001024,0x1fffffff);

    /*testing for  CFG_NBTR: addr=0x11001028*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: nsjw                     field_range: 30:24            field_access: rw  
    field_name: ntseg2                   field_range: 23:17            field_access: rw  
    field_name: ntseg1                   field_range: 16:9             field_access: rw  
    field_name: nbrp                     field_range: 8:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001028,0x1040600);

    /*testing for  CFG_DBTR: addr=0x1100102c*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: dsjw                     field_range: 17:14            field_access: rw  
    field_name: dtseg2                   field_range: 13:10            field_access: rw  
    field_name: dtseg1                   field_range: 9:5              field_access: rw  
    field_name: dbrp                     field_range: 4:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1100102c,0x0);

    /*testing for  CFG_ISO_MODE: addr=0x11001030*/
    /*
    field_name: reserved                 field_range: 31:1             field_access: ro  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001030,0x1);

    /*testing for  CFG_RXDATL_RSP: addr=0x11001040*/
    /*
    field_name: low                      field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001040,0x0);

    /*testing for  CFG_RXDATH_RSP: addr=0x11001044*/
    /*
    field_name: high                     field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001044,0x0);

    /*testing for  CFG_RXFIFO0: addr=0x11001048*/
    /*
    field_name: rxff0_opm                field_range: 31:31            field_access: rw  
    field_name: rxff0_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff0_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff0_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff0_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001048,0x0);

    /*testing for  CFG_RXFIFO0_STATUS: addr=0x1100104c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff0_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff0_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff0_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff0_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff0_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1100104c,0x0);

    /*testing for  CFG_RXFIFO0_ACK: addr=0x11001050*/
    /*
    field_name: rxf0_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x11001050,0x0);

    /*testing for  CFG_RXBUF: addr=0x11001054*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rxbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001054,0x0);

    /*testing for  CFG_RXFIFO1: addr=0x11001058*/
    /*
    field_name: rxff1_opm                field_range: 31:31            field_access: rw  
    field_name: rxff1_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff1_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff1_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff1_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001058,0x0);

    /*testing for  CFG_RXFIFO1_STATUS: addr=0x1100105c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff1_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff1_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff1_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff1_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff1_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1100105c,0x0);

    /*testing for  CFG_RXFIFO1_ACK: addr=0x11001060*/
    /*
    field_name: rxf1_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x11001060,0x0);

    /*testing for  CFG_RX_DSIZE: addr=0x11001064*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rb_dsize                 field_range: 10:8             field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: f1_dsize                 field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: f0_dsize                 field_range: 2:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001064,0x0);

    /*testing for  CFG_TXBUF: addr=0x11001070*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: txfifo_clr               field_range: 30:30            field_access: wo  
    field_name: txfifo_num               field_range: 29:24            field_access: rw  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: txbuf_num                field_range: 21:16            field_access: rw  
    field_name: reserved                 field_range: 15:11            field_access: ro  
    field_name: txbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001070,0x0);

    /*testing for RO CFG_TXFIFO_STATUS: addr=0x11001074*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: txff_full                field_range: 21:21            field_access: ro  
    field_name: txff_wptr                field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: txff_rptr                field_range: 12:8             field_access: ro  
    field_name: reserved                 field_range: 7:6              field_access: ro  
    field_name: txff_entry_num           field_range: 5:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x11001074,0x0);

    /*testing for  CFG_TX_DSIZE: addr=0x11001078*/
    /*
    field_name: reserved                 field_range: 31:3             field_access: ro  
    field_name: txb_dsize                field_range: 2:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001078,0x0);

    /*testing for  CFG_TXABORT_FINISH: addr=0x11001084*/
    /*
    field_name: finish                   field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001084,0x0);

    /*testing for  CFG_TXREQ_PENDING: addr=0x11001088*/
    /*
    field_name: req_pending              field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001088,0x0);

    /*testing for  CFG_TXB_TRANS: addr=0x1100108c*/
    /*
    field_name: tx_complete              field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x1100108c,0x0);

    /*testing for  CFG_TIME_STAMP: addr=0x11001090*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: value                    field_range: 15:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001090,0x0);

    /*testing for  CFG_TIME_DIV: addr=0x11001094*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: div_num                  field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001094,0x0);

    /*testing for  CFG_TRANS_DELAY_COMP: addr=0x11001098*/
    /*
    field_name: reserved                 field_range: 31:7             field_access: ro  
    field_name: offset_value             field_range: 6:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001098,0x0);

    /*testing for  CFG_ESR: addr=0x1100109c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: lec                      field_range: 21:19            field_access: w1c 
    field_name: bus_off                  field_range: 18:18            field_access: ro  
    field_name: err_pass_flag            field_range: 17:17            field_access: ro  
    field_name: err_active_flag          field_range: 16:16            field_access: ro  
    field_name: rec                      field_range: 15:8             field_access: ro  
    field_name: tec                      field_range: 7:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1100109c,0x10000);

    /*testing for  CFG_TRANS_ERR: addr=0x110010a0*/
    /*
    field_name: tx_num                   field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x110010a0,0x0);

    /*testing for  CFG_INT_EN: addr=0x110010b4*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_en             field_range: 22:22            field_access: rw  
    field_name: rxf1_full_en             field_range: 21:21            field_access: rw  
    field_name: rxf1_wm_en               field_range: 20:20            field_access: rw  
    field_name: rxf1_fill_en             field_range: 19:19            field_access: rw  
    field_name: rxf0_lost_en             field_range: 18:18            field_access: rw  
    field_name: rxf0_full_en             field_range: 17:17            field_access: rw  
    field_name: rxf0_wm_en               field_range: 16:16            field_access: rw  
    field_name: rxf0_fill_en             field_range: 15:15            field_access: rw  
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_en     field_range: 7:7              field_access: rw  
    field_name: arbitration_lost_int_en  field_range: 6:6              field_access: rw  
    field_name: bus_off_int_en           field_range: 5:5              field_access: rw  
    field_name: error_det_int_en         field_range: 4:4              field_access: rw  
    field_name: time_stamp_wrap_en       field_range: 3:3              field_access: rw  
    field_name: txff_uf_en               field_range: 2:2              field_access: rw  
    field_name: rxff_of_en               field_range: 1:1              field_access: rw  
    field_name: rxb_upd_en               field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x110010b4,0x0);

    /*testing for  CFG_INT_STATUS: addr=0x110010b8*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_int            field_range: 22:22            field_access: ro  
    field_name: rxf1_full_int            field_range: 21:21            field_access: ro  
    field_name: rxf1_wm_int              field_range: 20:20            field_access: ro  
    field_name: rxf1_fill_int            field_range: 19:19            field_access: w1c 
    field_name: rxf0_lost_int            field_range: 18:18            field_access: ro  
    field_name: rxf0_full_int            field_range: 17:17            field_access: ro  
    field_name: rxf0_wm_int              field_range: 16:16            field_access: ro  
    field_name: rxf0_fill_int            field_range: 15:15            field_access: w1c 
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_int    field_range: 7:7              field_access: ro  
    field_name: arbitration_lost_int     field_range: 6:6              field_access: w1c 
    field_name: bus_off_int              field_range: 5:5              field_access: w1c 
    field_name: error_det_int            field_range: 4:4              field_access: w1c 
    field_name: time_stamp_wrap_int      field_range: 3:3              field_access: w1c 
    field_name: txff_uf_int              field_range: 2:2              field_access: w1c 
    field_name: rxff_of_int              field_range: 1:1              field_access: w1c 
    field_name: rxb_upd_int              field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x110010b8,0x0);

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

    /*testing for RO RELEASE: addr=0x11001000*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x11001000, 0x20201, 0xFFFFFFFF);

    /*testing for  CFG_MCR: addr=0x11001004*/
    /*
    field_name: reserved                 field_range: 31:19            field_access: ro  
    field_name: protocol_exception_handling field_range: 18:17            field_access: rw  
    field_name: be_receiver_in_loss      field_range: 16:16            field_access: rw  
    field_name: restricted_operation     field_range: 15:15            field_access: rw  
    field_name: rx_pin_pull_up           field_range: 14:14            field_access: rw  
    field_name: tx_pin_pull_up           field_range: 13:13            field_access: rw  
    field_name: clk_stop_ack             field_range: 12:12            field_access: ro  
    field_name: clk_stop_req             field_range: 11:11            field_access: rw  
    field_name: reserved                 field_range: 10:10            field_access: ro  
    field_name: txpause                  field_range: 9:9              field_access: rw  
    field_name: brs_en                   field_range: 8:8              field_access: rw  
    field_name: fd_en                    field_range: 7:7              field_access: rw  
    field_name: timestamp_en             field_range: 6:6              field_access: rw  
    field_name: reserved                 field_range: 5:5              field_access: ro  
    field_name: auto_retrans_en          field_range: 4:4              field_access: rw  
    field_name: tdc_en                   field_range: 3:3              field_access: rw  
    field_name: sleep_req                field_range: 2:2              field_access: rw  
    field_name: initial_req              field_range: 1:1              field_access: rw  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001004, 0x36004, (0x00000000 & 0x7ebdb | 4), 0x0, 0x7ebdb);

    REG_PWR_Double_Check(0x11001004, 0x36004, (0xFFFFFFFF & 0x7ebdb | 4), 0x7ebdb, 0x7ebdb);

    REG_PWR_Double_Check(0x11001004, 0x36004, (0x55555555 & 0x7ebdb | 4), 0x54151, 0x7ebdb);

    REG_PWR_Double_Check(0x11001004, 0x36004, (0xAAAAAAAA & 0x7ebdb | 4), 0x2aa8a, 0x7ebdb);

    /*testing for  CFG_TEST: addr=0x11001008*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: silm                     field_range: 1:1              field_access: rw  
    field_name: lbkm                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001008, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x11001008, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x11001008, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x11001008, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for  CFG_PSR: addr=0x1100100c*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: trans_delay_comp         field_range: 22:16            field_access: ro  
    field_name: reserved                 field_range: 15:5             field_access: ro  
    field_name: pxe                      field_range: 4:4              field_access: w1c 
    field_name: esi                      field_range: 3:3              field_access: w1c 
    field_name: brs                      field_range: 2:2              field_access: w1c 
    field_name: edl                      field_range: 1:1              field_access: w1c 
    field_name: ide                      field_range: 0:0              field_access: w1c 
    */
    REG_PWR_Double_Check(0x1100100c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100100c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100100c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100100c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_GLOBAL_FILTER: addr=0x11001014*/
    /*
    field_name: reserved                 field_range: 31:6             field_access: ro  
    field_name: accept_miss_st_frame     field_range: 5:4              field_access: rw  
    field_name: accept_miss_ex_frame     field_range: 3:2              field_access: rw  
    field_name: reject_st_remote         field_range: 1:1              field_access: rw  
    field_name: reject_ex_remote         field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001014, 0x3c, (0x00000000 & 0x3f | 0), 0x0, 0x3f);

    REG_PWR_Double_Check(0x11001014, 0x3c, (0xFFFFFFFF & 0x3f | 0), 0x3f, 0x3f);

    REG_PWR_Double_Check(0x11001014, 0x3c, (0x55555555 & 0x3f | 0), 0x15, 0x3f);

    REG_PWR_Double_Check(0x11001014, 0x3c, (0xAAAAAAAA & 0x3f | 0), 0x2a, 0x3f);

    /*testing for  CFG_ST_FILTER: addr=0x11001018*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: stflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: stflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001018, 0x0, (0x00000000 & 0xff7ff | 0), 0x0, 0xff7ff);

    REG_PWR_Double_Check(0x11001018, 0x0, (0xFFFFFFFF & 0xff7ff | 0), 0xff7ff, 0xff7ff);

    REG_PWR_Double_Check(0x11001018, 0x0, (0x55555555 & 0xff7ff | 0), 0x55555, 0xff7ff);

    REG_PWR_Double_Check(0x11001018, 0x0, (0xAAAAAAAA & 0xff7ff | 0), 0xaa2aa, 0xff7ff);

    /*testing for  CFG_EX_FILTER: addr=0x1100101c*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: exflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: exflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x1100101c, 0x0, (0x00000000 & 0xff7ff | 0), 0x0, 0xff7ff);

    REG_PWR_Double_Check(0x1100101c, 0x0, (0xFFFFFFFF & 0xff7ff | 0), 0xff7ff, 0xff7ff);

    REG_PWR_Double_Check(0x1100101c, 0x0, (0x55555555 & 0xff7ff | 0), 0x55555, 0xff7ff);

    REG_PWR_Double_Check(0x1100101c, 0x0, (0xAAAAAAAA & 0xff7ff | 0), 0xaa2aa, 0xff7ff);

    /*testing for  CFG_STIDM: addr=0x11001020*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: sidm                     field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001020, 0x7ff, (0x00000000 & 0x7ff | 0), 0x0, 0x7ff);

    REG_PWR_Double_Check(0x11001020, 0x7ff, (0xFFFFFFFF & 0x7ff | 0), 0x7ff, 0x7ff);

    REG_PWR_Double_Check(0x11001020, 0x7ff, (0x55555555 & 0x7ff | 0), 0x555, 0x7ff);

    REG_PWR_Double_Check(0x11001020, 0x7ff, (0xAAAAAAAA & 0x7ff | 0), 0x2aa, 0x7ff);

    /*testing for  CFG_EXIDM: addr=0x11001024*/
    /*
    field_name: reserved                 field_range: 31:29            field_access: ro  
    field_name: eidm                     field_range: 28:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0x00000000 & 0x1fffffff | 0), 0x0, 0x1fffffff);

    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0xFFFFFFFF & 0x1fffffff | 0), 0x1fffffff, 0x1fffffff);

    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0x55555555 & 0x1fffffff | 0), 0x15555555, 0x1fffffff);

    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0xAAAAAAAA & 0x1fffffff | 0), 0xaaaaaaa, 0x1fffffff);

    /*testing for  CFG_NBTR: addr=0x11001028*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: nsjw                     field_range: 30:24            field_access: rw  
    field_name: ntseg2                   field_range: 23:17            field_access: rw  
    field_name: ntseg1                   field_range: 16:9             field_access: rw  
    field_name: nbrp                     field_range: 8:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001028, 0x1040600, (0x00000000 & 0x7fffffff | 0), 0x0, 0x7fffffff);

    REG_PWR_Double_Check(0x11001028, 0x1040600, (0xFFFFFFFF & 0x7fffffff | 0), 0x7fffffff, 0x7fffffff);

    REG_PWR_Double_Check(0x11001028, 0x1040600, (0x55555555 & 0x7fffffff | 0), 0x55555555, 0x7fffffff);

    REG_PWR_Double_Check(0x11001028, 0x1040600, (0xAAAAAAAA & 0x7fffffff | 0), 0x2aaaaaaa, 0x7fffffff);

    /*testing for  CFG_DBTR: addr=0x1100102c*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: dsjw                     field_range: 17:14            field_access: rw  
    field_name: dtseg2                   field_range: 13:10            field_access: rw  
    field_name: dtseg1                   field_range: 9:5              field_access: rw  
    field_name: dbrp                     field_range: 4:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1100102c, 0x0, (0x00000000 & 0x3ffff | 0), 0x0, 0x3ffff);

    REG_PWR_Double_Check(0x1100102c, 0x0, (0xFFFFFFFF & 0x3ffff | 0), 0x3ffff, 0x3ffff);

    REG_PWR_Double_Check(0x1100102c, 0x0, (0x55555555 & 0x3ffff | 0), 0x15555, 0x3ffff);

    REG_PWR_Double_Check(0x1100102c, 0x0, (0xAAAAAAAA & 0x3ffff | 0), 0x2aaaa, 0x3ffff);

    /*testing for  CFG_ISO_MODE: addr=0x11001030*/
    /*
    field_name: reserved                 field_range: 31:1             field_access: ro  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001030, 0x1, (0x00000000 & 0x1 | 0), 0x0, 0x1);

    REG_PWR_Double_Check(0x11001030, 0x1, (0xFFFFFFFF & 0x1 | 0), 0x1, 0x1);

    REG_PWR_Double_Check(0x11001030, 0x1, (0x55555555 & 0x1 | 0), 0x1, 0x1);

    REG_PWR_Double_Check(0x11001030, 0x1, (0xAAAAAAAA & 0x1 | 0), 0x0, 0x1);

    /*testing for  CFG_RXDATL_RSP: addr=0x11001040*/
    /*
    field_name: low                      field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001040, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001040, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001040, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001040, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXDATH_RSP: addr=0x11001044*/
    /*
    field_name: high                     field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001044, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001044, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001044, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001044, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXFIFO0: addr=0x11001048*/
    /*
    field_name: rxff0_opm                field_range: 31:31            field_access: rw  
    field_name: rxff0_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff0_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff0_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff0_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001048, 0x0, (0x00000000 & 0x83fff7ff | 0), 0x0, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001048, 0x0, (0xFFFFFFFF & 0x83fff7ff | 0), 0x83fff7ff, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001048, 0x0, (0x55555555 & 0x83fff7ff | 0), 0x1555555, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001048, 0x0, (0xAAAAAAAA & 0x83fff7ff | 0), 0x82aaa2aa, 0x83fff7ff);

    /*testing for  CFG_RXFIFO0_STATUS: addr=0x1100104c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff0_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff0_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff0_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff0_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff0_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x1100104c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100104c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100104c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100104c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXFIFO0_ACK: addr=0x11001050*/
    /*
    field_name: rxf0_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_PWR_Double_Check(0x11001050, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001050, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001050, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001050, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXBUF: addr=0x11001054*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rxbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001054, 0x0, (0x00000000 & 0x7ff | 0), 0x0, 0x7ff);

    REG_PWR_Double_Check(0x11001054, 0x0, (0xFFFFFFFF & 0x7ff | 0), 0x7ff, 0x7ff);

    REG_PWR_Double_Check(0x11001054, 0x0, (0x55555555 & 0x7ff | 0), 0x555, 0x7ff);

    REG_PWR_Double_Check(0x11001054, 0x0, (0xAAAAAAAA & 0x7ff | 0), 0x2aa, 0x7ff);

    /*testing for  CFG_RXFIFO1: addr=0x11001058*/
    /*
    field_name: rxff1_opm                field_range: 31:31            field_access: rw  
    field_name: rxff1_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff1_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff1_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff1_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001058, 0x0, (0x00000000 & 0x83fff7ff | 0), 0x0, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001058, 0x0, (0xFFFFFFFF & 0x83fff7ff | 0), 0x83fff7ff, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001058, 0x0, (0x55555555 & 0x83fff7ff | 0), 0x1555555, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001058, 0x0, (0xAAAAAAAA & 0x83fff7ff | 0), 0x82aaa2aa, 0x83fff7ff);

    /*testing for  CFG_RXFIFO1_STATUS: addr=0x1100105c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff1_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff1_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff1_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff1_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff1_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x1100105c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100105c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100105c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100105c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXFIFO1_ACK: addr=0x11001060*/
    /*
    field_name: rxf1_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_PWR_Double_Check(0x11001060, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001060, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001060, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001060, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RX_DSIZE: addr=0x11001064*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rb_dsize                 field_range: 10:8             field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: f1_dsize                 field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: f0_dsize                 field_range: 2:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001064, 0x0, (0x00000000 & 0x777 | 0), 0x0, 0x777);

    REG_PWR_Double_Check(0x11001064, 0x0, (0xFFFFFFFF & 0x777 | 0), 0x777, 0x777);

    REG_PWR_Double_Check(0x11001064, 0x0, (0x55555555 & 0x777 | 0), 0x555, 0x777);

    REG_PWR_Double_Check(0x11001064, 0x0, (0xAAAAAAAA & 0x777 | 0), 0x222, 0x777);

    /*testing for  CFG_TXBUF: addr=0x11001070*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: txfifo_clr               field_range: 30:30            field_access: wo  
    field_name: txfifo_num               field_range: 29:24            field_access: rw  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: txbuf_num                field_range: 21:16            field_access: rw  
    field_name: reserved                 field_range: 15:11            field_access: ro  
    field_name: txbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001070, 0x0, (0x00000000 & 0x3f3f07ff | 0), 0x0, 0x3f3f07ff);

    REG_PWR_Double_Check(0x11001070, 0x0, (0xFFFFFFFF & 0x3f3f07ff | 0), 0x3f3f07ff, 0x3f3f07ff);

    REG_PWR_Double_Check(0x11001070, 0x0, (0x55555555 & 0x3f3f07ff | 0), 0x15150555, 0x3f3f07ff);

    REG_PWR_Double_Check(0x11001070, 0x0, (0xAAAAAAAA & 0x3f3f07ff | 0), 0x2a2a02aa, 0x3f3f07ff);

    /*testing for RO CFG_TXFIFO_STATUS: addr=0x11001074*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: txff_full                field_range: 21:21            field_access: ro  
    field_name: txff_wptr                field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: txff_rptr                field_range: 12:8             field_access: ro  
    field_name: reserved                 field_range: 7:6              field_access: ro  
    field_name: txff_entry_num           field_range: 5:0              field_access: ro  
    */
    REG_RO_Double_Check(0x11001074, 0x0, 0xFFFFFFFF);

    /*testing for  CFG_TX_DSIZE: addr=0x11001078*/
    /*
    field_name: reserved                 field_range: 31:3             field_access: ro  
    field_name: txb_dsize                field_range: 2:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001078, 0x0, (0x00000000 & 0x7 | 0), 0x0, 0x7);

    REG_PWR_Double_Check(0x11001078, 0x0, (0xFFFFFFFF & 0x7 | 0), 0x7, 0x7);

    REG_PWR_Double_Check(0x11001078, 0x0, (0x55555555 & 0x7 | 0), 0x5, 0x7);

    REG_PWR_Double_Check(0x11001078, 0x0, (0xAAAAAAAA & 0x7 | 0), 0x2, 0x7);

    /*testing for  CFG_TXABORT_FINISH: addr=0x11001084*/
    /*
    field_name: finish                   field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001084, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001084, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001084, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001084, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TXREQ_PENDING: addr=0x11001088*/
    /*
    field_name: req_pending              field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001088, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001088, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001088, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001088, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TXB_TRANS: addr=0x1100108c*/
    /*
    field_name: tx_complete              field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x1100108c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100108c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100108c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100108c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TIME_STAMP: addr=0x11001090*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: value                    field_range: 15:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001090, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001090, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001090, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001090, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TIME_DIV: addr=0x11001094*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: div_num                  field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001094, 0x0, (0x00000000 & 0xf | 0), 0x0, 0xf);

    REG_PWR_Double_Check(0x11001094, 0x0, (0xFFFFFFFF & 0xf | 0), 0xf, 0xf);

    REG_PWR_Double_Check(0x11001094, 0x0, (0x55555555 & 0xf | 0), 0x5, 0xf);

    REG_PWR_Double_Check(0x11001094, 0x0, (0xAAAAAAAA & 0xf | 0), 0xa, 0xf);

    /*testing for  CFG_TRANS_DELAY_COMP: addr=0x11001098*/
    /*
    field_name: reserved                 field_range: 31:7             field_access: ro  
    field_name: offset_value             field_range: 6:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001098, 0x0, (0x00000000 & 0x7f | 0), 0x0, 0x7f);

    REG_PWR_Double_Check(0x11001098, 0x0, (0xFFFFFFFF & 0x7f | 0), 0x7f, 0x7f);

    REG_PWR_Double_Check(0x11001098, 0x0, (0x55555555 & 0x7f | 0), 0x55, 0x7f);

    REG_PWR_Double_Check(0x11001098, 0x0, (0xAAAAAAAA & 0x7f | 0), 0x2a, 0x7f);

    /*testing for  CFG_ESR: addr=0x1100109c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: lec                      field_range: 21:19            field_access: w1c 
    field_name: bus_off                  field_range: 18:18            field_access: ro  
    field_name: err_pass_flag            field_range: 17:17            field_access: ro  
    field_name: err_active_flag          field_range: 16:16            field_access: ro  
    field_name: rec                      field_range: 15:8             field_access: ro  
    field_name: tec                      field_range: 7:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x1100109c, 0x10000, (0x00000000 & 0x0 | 65536), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100109c, 0x10000, (0xFFFFFFFF & 0x0 | 65536), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100109c, 0x10000, (0x55555555 & 0x0 | 65536), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100109c, 0x10000, (0xAAAAAAAA & 0x0 | 65536), 0x0, 0x0);

    /*testing for  CFG_TRANS_ERR: addr=0x110010a0*/
    /*
    field_name: tx_num                   field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x110010a0, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010a0, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010a0, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010a0, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_INT_EN: addr=0x110010b4*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_en             field_range: 22:22            field_access: rw  
    field_name: rxf1_full_en             field_range: 21:21            field_access: rw  
    field_name: rxf1_wm_en               field_range: 20:20            field_access: rw  
    field_name: rxf1_fill_en             field_range: 19:19            field_access: rw  
    field_name: rxf0_lost_en             field_range: 18:18            field_access: rw  
    field_name: rxf0_full_en             field_range: 17:17            field_access: rw  
    field_name: rxf0_wm_en               field_range: 16:16            field_access: rw  
    field_name: rxf0_fill_en             field_range: 15:15            field_access: rw  
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_en     field_range: 7:7              field_access: rw  
    field_name: arbitration_lost_int_en  field_range: 6:6              field_access: rw  
    field_name: bus_off_int_en           field_range: 5:5              field_access: rw  
    field_name: error_det_int_en         field_range: 4:4              field_access: rw  
    field_name: time_stamp_wrap_en       field_range: 3:3              field_access: rw  
    field_name: txff_uf_en               field_range: 2:2              field_access: rw  
    field_name: rxff_of_en               field_range: 1:1              field_access: rw  
    field_name: rxb_upd_en               field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x110010b4, 0x0, (0x00000000 & 0x7f80ff | 0), 0x0, 0x7f80ff);

    REG_PWR_Double_Check(0x110010b4, 0x0, (0xFFFFFFFF & 0x7f80ff | 0), 0x7f80ff, 0x7f80ff);

    REG_PWR_Double_Check(0x110010b4, 0x0, (0x55555555 & 0x7f80ff | 0), 0x550055, 0x7f80ff);

    REG_PWR_Double_Check(0x110010b4, 0x0, (0xAAAAAAAA & 0x7f80ff | 0), 0x2a80aa, 0x7f80ff);

    /*testing for  CFG_INT_STATUS: addr=0x110010b8*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_int            field_range: 22:22            field_access: ro  
    field_name: rxf1_full_int            field_range: 21:21            field_access: ro  
    field_name: rxf1_wm_int              field_range: 20:20            field_access: ro  
    field_name: rxf1_fill_int            field_range: 19:19            field_access: w1c 
    field_name: rxf0_lost_int            field_range: 18:18            field_access: ro  
    field_name: rxf0_full_int            field_range: 17:17            field_access: ro  
    field_name: rxf0_wm_int              field_range: 16:16            field_access: ro  
    field_name: rxf0_fill_int            field_range: 15:15            field_access: w1c 
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_int    field_range: 7:7              field_access: ro  
    field_name: arbitration_lost_int     field_range: 6:6              field_access: w1c 
    field_name: bus_off_int              field_range: 5:5              field_access: w1c 
    field_name: error_det_int            field_range: 4:4              field_access: w1c 
    field_name: time_stamp_wrap_int      field_range: 3:3              field_access: w1c 
    field_name: txff_uf_int              field_range: 2:2              field_access: w1c 
    field_name: rxff_of_int              field_range: 1:1              field_access: w1c 
    field_name: rxb_upd_int              field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x110010b8, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010b8, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010b8, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010b8, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    delta_mcycle1=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle1;

    /*********************TEST FOR CLOCK DIVISION 36*********************/

    #ifdef MISC_HAS_XKAN1_CLK_DIV

    xkan1_clk_div(35);

    start_mcycle2=__RV_CSR_READ(CSR_MCYCLE);

    /*testing for RO RELEASE: addr=0x11001000*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_RO_Double_Check(0x11001000, 0x20201, 0xFFFFFFFF);

    /*testing for  CFG_MCR: addr=0x11001004*/
    /*
    field_name: reserved                 field_range: 31:19            field_access: ro  
    field_name: protocol_exception_handling field_range: 18:17            field_access: rw  
    field_name: be_receiver_in_loss      field_range: 16:16            field_access: rw  
    field_name: restricted_operation     field_range: 15:15            field_access: rw  
    field_name: rx_pin_pull_up           field_range: 14:14            field_access: rw  
    field_name: tx_pin_pull_up           field_range: 13:13            field_access: rw  
    field_name: clk_stop_ack             field_range: 12:12            field_access: ro  
    field_name: clk_stop_req             field_range: 11:11            field_access: rw  
    field_name: reserved                 field_range: 10:10            field_access: ro  
    field_name: txpause                  field_range: 9:9              field_access: rw  
    field_name: brs_en                   field_range: 8:8              field_access: rw  
    field_name: fd_en                    field_range: 7:7              field_access: rw  
    field_name: timestamp_en             field_range: 6:6              field_access: rw  
    field_name: reserved                 field_range: 5:5              field_access: ro  
    field_name: auto_retrans_en          field_range: 4:4              field_access: rw  
    field_name: tdc_en                   field_range: 3:3              field_access: rw  
    field_name: sleep_req                field_range: 2:2              field_access: rw  
    field_name: initial_req              field_range: 1:1              field_access: rw  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001004, 0x36004, (0x00000000 & 0x7ebdb | 4), 0x0, 0x7ebdb);

    REG_PWR_Double_Check(0x11001004, 0x36004, (0xFFFFFFFF & 0x7ebdb | 4), 0x7ebdb, 0x7ebdb);

    REG_PWR_Double_Check(0x11001004, 0x36004, (0x55555555 & 0x7ebdb | 4), 0x54151, 0x7ebdb);

    REG_PWR_Double_Check(0x11001004, 0x36004, (0xAAAAAAAA & 0x7ebdb | 4), 0x2aa8a, 0x7ebdb);

    /*testing for  CFG_TEST: addr=0x11001008*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: silm                     field_range: 1:1              field_access: rw  
    field_name: lbkm                     field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001008, 0x0, (0x00000000 & 0x3 | 0), 0x0, 0x3);

    REG_PWR_Double_Check(0x11001008, 0x0, (0xFFFFFFFF & 0x3 | 0), 0x3, 0x3);

    REG_PWR_Double_Check(0x11001008, 0x0, (0x55555555 & 0x3 | 0), 0x1, 0x3);

    REG_PWR_Double_Check(0x11001008, 0x0, (0xAAAAAAAA & 0x3 | 0), 0x2, 0x3);

    /*testing for  CFG_PSR: addr=0x1100100c*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: trans_delay_comp         field_range: 22:16            field_access: ro  
    field_name: reserved                 field_range: 15:5             field_access: ro  
    field_name: pxe                      field_range: 4:4              field_access: w1c 
    field_name: esi                      field_range: 3:3              field_access: w1c 
    field_name: brs                      field_range: 2:2              field_access: w1c 
    field_name: edl                      field_range: 1:1              field_access: w1c 
    field_name: ide                      field_range: 0:0              field_access: w1c 
    */
    REG_PWR_Double_Check(0x1100100c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100100c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100100c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100100c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_GLOBAL_FILTER: addr=0x11001014*/
    /*
    field_name: reserved                 field_range: 31:6             field_access: ro  
    field_name: accept_miss_st_frame     field_range: 5:4              field_access: rw  
    field_name: accept_miss_ex_frame     field_range: 3:2              field_access: rw  
    field_name: reject_st_remote         field_range: 1:1              field_access: rw  
    field_name: reject_ex_remote         field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001014, 0x3c, (0x00000000 & 0x3f | 0), 0x0, 0x3f);

    REG_PWR_Double_Check(0x11001014, 0x3c, (0xFFFFFFFF & 0x3f | 0), 0x3f, 0x3f);

    REG_PWR_Double_Check(0x11001014, 0x3c, (0x55555555 & 0x3f | 0), 0x15, 0x3f);

    REG_PWR_Double_Check(0x11001014, 0x3c, (0xAAAAAAAA & 0x3f | 0), 0x2a, 0x3f);

    /*testing for  CFG_ST_FILTER: addr=0x11001018*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: stflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: stflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001018, 0x0, (0x00000000 & 0xff7ff | 0), 0x0, 0xff7ff);

    REG_PWR_Double_Check(0x11001018, 0x0, (0xFFFFFFFF & 0xff7ff | 0), 0xff7ff, 0xff7ff);

    REG_PWR_Double_Check(0x11001018, 0x0, (0x55555555 & 0xff7ff | 0), 0x55555, 0xff7ff);

    REG_PWR_Double_Check(0x11001018, 0x0, (0xAAAAAAAA & 0xff7ff | 0), 0xaa2aa, 0xff7ff);

    /*testing for  CFG_EX_FILTER: addr=0x1100101c*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: exflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: exflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x1100101c, 0x0, (0x00000000 & 0xff7ff | 0), 0x0, 0xff7ff);

    REG_PWR_Double_Check(0x1100101c, 0x0, (0xFFFFFFFF & 0xff7ff | 0), 0xff7ff, 0xff7ff);

    REG_PWR_Double_Check(0x1100101c, 0x0, (0x55555555 & 0xff7ff | 0), 0x55555, 0xff7ff);

    REG_PWR_Double_Check(0x1100101c, 0x0, (0xAAAAAAAA & 0xff7ff | 0), 0xaa2aa, 0xff7ff);

    /*testing for  CFG_STIDM: addr=0x11001020*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: sidm                     field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001020, 0x7ff, (0x00000000 & 0x7ff | 0), 0x0, 0x7ff);

    REG_PWR_Double_Check(0x11001020, 0x7ff, (0xFFFFFFFF & 0x7ff | 0), 0x7ff, 0x7ff);

    REG_PWR_Double_Check(0x11001020, 0x7ff, (0x55555555 & 0x7ff | 0), 0x555, 0x7ff);

    REG_PWR_Double_Check(0x11001020, 0x7ff, (0xAAAAAAAA & 0x7ff | 0), 0x2aa, 0x7ff);

    /*testing for  CFG_EXIDM: addr=0x11001024*/
    /*
    field_name: reserved                 field_range: 31:29            field_access: ro  
    field_name: eidm                     field_range: 28:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0x00000000 & 0x1fffffff | 0), 0x0, 0x1fffffff);

    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0xFFFFFFFF & 0x1fffffff | 0), 0x1fffffff, 0x1fffffff);

    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0x55555555 & 0x1fffffff | 0), 0x15555555, 0x1fffffff);

    REG_PWR_Double_Check(0x11001024, 0x1fffffff, (0xAAAAAAAA & 0x1fffffff | 0), 0xaaaaaaa, 0x1fffffff);

    /*testing for  CFG_NBTR: addr=0x11001028*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: nsjw                     field_range: 30:24            field_access: rw  
    field_name: ntseg2                   field_range: 23:17            field_access: rw  
    field_name: ntseg1                   field_range: 16:9             field_access: rw  
    field_name: nbrp                     field_range: 8:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001028, 0x1040600, (0x00000000 & 0x7fffffff | 0), 0x0, 0x7fffffff);

    REG_PWR_Double_Check(0x11001028, 0x1040600, (0xFFFFFFFF & 0x7fffffff | 0), 0x7fffffff, 0x7fffffff);

    REG_PWR_Double_Check(0x11001028, 0x1040600, (0x55555555 & 0x7fffffff | 0), 0x55555555, 0x7fffffff);

    REG_PWR_Double_Check(0x11001028, 0x1040600, (0xAAAAAAAA & 0x7fffffff | 0), 0x2aaaaaaa, 0x7fffffff);

    /*testing for  CFG_DBTR: addr=0x1100102c*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: dsjw                     field_range: 17:14            field_access: rw  
    field_name: dtseg2                   field_range: 13:10            field_access: rw  
    field_name: dtseg1                   field_range: 9:5              field_access: rw  
    field_name: dbrp                     field_range: 4:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x1100102c, 0x0, (0x00000000 & 0x3ffff | 0), 0x0, 0x3ffff);

    REG_PWR_Double_Check(0x1100102c, 0x0, (0xFFFFFFFF & 0x3ffff | 0), 0x3ffff, 0x3ffff);

    REG_PWR_Double_Check(0x1100102c, 0x0, (0x55555555 & 0x3ffff | 0), 0x15555, 0x3ffff);

    REG_PWR_Double_Check(0x1100102c, 0x0, (0xAAAAAAAA & 0x3ffff | 0), 0x2aaaa, 0x3ffff);

    /*testing for  CFG_ISO_MODE: addr=0x11001030*/
    /*
    field_name: reserved                 field_range: 31:1             field_access: ro  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001030, 0x1, (0x00000000 & 0x1 | 0), 0x0, 0x1);

    REG_PWR_Double_Check(0x11001030, 0x1, (0xFFFFFFFF & 0x1 | 0), 0x1, 0x1);

    REG_PWR_Double_Check(0x11001030, 0x1, (0x55555555 & 0x1 | 0), 0x1, 0x1);

    REG_PWR_Double_Check(0x11001030, 0x1, (0xAAAAAAAA & 0x1 | 0), 0x0, 0x1);

    /*testing for  CFG_RXDATL_RSP: addr=0x11001040*/
    /*
    field_name: low                      field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001040, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001040, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001040, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001040, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXDATH_RSP: addr=0x11001044*/
    /*
    field_name: high                     field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001044, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001044, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001044, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001044, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXFIFO0: addr=0x11001048*/
    /*
    field_name: rxff0_opm                field_range: 31:31            field_access: rw  
    field_name: rxff0_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff0_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff0_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff0_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001048, 0x0, (0x00000000 & 0x83fff7ff | 0), 0x0, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001048, 0x0, (0xFFFFFFFF & 0x83fff7ff | 0), 0x83fff7ff, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001048, 0x0, (0x55555555 & 0x83fff7ff | 0), 0x1555555, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001048, 0x0, (0xAAAAAAAA & 0x83fff7ff | 0), 0x82aaa2aa, 0x83fff7ff);

    /*testing for  CFG_RXFIFO0_STATUS: addr=0x1100104c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff0_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff0_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff0_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff0_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff0_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x1100104c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100104c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100104c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100104c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXFIFO0_ACK: addr=0x11001050*/
    /*
    field_name: rxf0_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_PWR_Double_Check(0x11001050, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001050, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001050, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001050, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXBUF: addr=0x11001054*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rxbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001054, 0x0, (0x00000000 & 0x7ff | 0), 0x0, 0x7ff);

    REG_PWR_Double_Check(0x11001054, 0x0, (0xFFFFFFFF & 0x7ff | 0), 0x7ff, 0x7ff);

    REG_PWR_Double_Check(0x11001054, 0x0, (0x55555555 & 0x7ff | 0), 0x555, 0x7ff);

    REG_PWR_Double_Check(0x11001054, 0x0, (0xAAAAAAAA & 0x7ff | 0), 0x2aa, 0x7ff);

    /*testing for  CFG_RXFIFO1: addr=0x11001058*/
    /*
    field_name: rxff1_opm                field_range: 31:31            field_access: rw  
    field_name: rxff1_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff1_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff1_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff1_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001058, 0x0, (0x00000000 & 0x83fff7ff | 0), 0x0, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001058, 0x0, (0xFFFFFFFF & 0x83fff7ff | 0), 0x83fff7ff, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001058, 0x0, (0x55555555 & 0x83fff7ff | 0), 0x1555555, 0x83fff7ff);

    REG_PWR_Double_Check(0x11001058, 0x0, (0xAAAAAAAA & 0x83fff7ff | 0), 0x82aaa2aa, 0x83fff7ff);

    /*testing for  CFG_RXFIFO1_STATUS: addr=0x1100105c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff1_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff1_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff1_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff1_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff1_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x1100105c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100105c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100105c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100105c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RXFIFO1_ACK: addr=0x11001060*/
    /*
    field_name: rxf1_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_PWR_Double_Check(0x11001060, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001060, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001060, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001060, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_RX_DSIZE: addr=0x11001064*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rb_dsize                 field_range: 10:8             field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: f1_dsize                 field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: f0_dsize                 field_range: 2:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001064, 0x0, (0x00000000 & 0x777 | 0), 0x0, 0x777);

    REG_PWR_Double_Check(0x11001064, 0x0, (0xFFFFFFFF & 0x777 | 0), 0x777, 0x777);

    REG_PWR_Double_Check(0x11001064, 0x0, (0x55555555 & 0x777 | 0), 0x555, 0x777);

    REG_PWR_Double_Check(0x11001064, 0x0, (0xAAAAAAAA & 0x777 | 0), 0x222, 0x777);

    /*testing for  CFG_TXBUF: addr=0x11001070*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: txfifo_clr               field_range: 30:30            field_access: wo  
    field_name: txfifo_num               field_range: 29:24            field_access: rw  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: txbuf_num                field_range: 21:16            field_access: rw  
    field_name: reserved                 field_range: 15:11            field_access: ro  
    field_name: txbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_PWR_Double_Check(0x11001070, 0x0, (0x00000000 & 0x3f3f07ff | 0), 0x0, 0x3f3f07ff);

    REG_PWR_Double_Check(0x11001070, 0x0, (0xFFFFFFFF & 0x3f3f07ff | 0), 0x3f3f07ff, 0x3f3f07ff);

    REG_PWR_Double_Check(0x11001070, 0x0, (0x55555555 & 0x3f3f07ff | 0), 0x15150555, 0x3f3f07ff);

    REG_PWR_Double_Check(0x11001070, 0x0, (0xAAAAAAAA & 0x3f3f07ff | 0), 0x2a2a02aa, 0x3f3f07ff);

    /*testing for RO CFG_TXFIFO_STATUS: addr=0x11001074*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: txff_full                field_range: 21:21            field_access: ro  
    field_name: txff_wptr                field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: txff_rptr                field_range: 12:8             field_access: ro  
    field_name: reserved                 field_range: 7:6              field_access: ro  
    field_name: txff_entry_num           field_range: 5:0              field_access: ro  
    */
    REG_RO_Double_Check(0x11001074, 0x0, 0xFFFFFFFF);

    /*testing for  CFG_TX_DSIZE: addr=0x11001078*/
    /*
    field_name: reserved                 field_range: 31:3             field_access: ro  
    field_name: txb_dsize                field_range: 2:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001078, 0x0, (0x00000000 & 0x7 | 0), 0x0, 0x7);

    REG_PWR_Double_Check(0x11001078, 0x0, (0xFFFFFFFF & 0x7 | 0), 0x7, 0x7);

    REG_PWR_Double_Check(0x11001078, 0x0, (0x55555555 & 0x7 | 0), 0x5, 0x7);

    REG_PWR_Double_Check(0x11001078, 0x0, (0xAAAAAAAA & 0x7 | 0), 0x2, 0x7);

    /*testing for  CFG_TXABORT_FINISH: addr=0x11001084*/
    /*
    field_name: finish                   field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001084, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001084, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001084, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001084, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TXREQ_PENDING: addr=0x11001088*/
    /*
    field_name: req_pending              field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001088, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001088, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001088, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001088, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TXB_TRANS: addr=0x1100108c*/
    /*
    field_name: tx_complete              field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x1100108c, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100108c, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100108c, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100108c, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TIME_STAMP: addr=0x11001090*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: value                    field_range: 15:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x11001090, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001090, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001090, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x11001090, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_TIME_DIV: addr=0x11001094*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: div_num                  field_range: 3:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001094, 0x0, (0x00000000 & 0xf | 0), 0x0, 0xf);

    REG_PWR_Double_Check(0x11001094, 0x0, (0xFFFFFFFF & 0xf | 0), 0xf, 0xf);

    REG_PWR_Double_Check(0x11001094, 0x0, (0x55555555 & 0xf | 0), 0x5, 0xf);

    REG_PWR_Double_Check(0x11001094, 0x0, (0xAAAAAAAA & 0xf | 0), 0xa, 0xf);

    /*testing for  CFG_TRANS_DELAY_COMP: addr=0x11001098*/
    /*
    field_name: reserved                 field_range: 31:7             field_access: ro  
    field_name: offset_value             field_range: 6:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x11001098, 0x0, (0x00000000 & 0x7f | 0), 0x0, 0x7f);

    REG_PWR_Double_Check(0x11001098, 0x0, (0xFFFFFFFF & 0x7f | 0), 0x7f, 0x7f);

    REG_PWR_Double_Check(0x11001098, 0x0, (0x55555555 & 0x7f | 0), 0x55, 0x7f);

    REG_PWR_Double_Check(0x11001098, 0x0, (0xAAAAAAAA & 0x7f | 0), 0x2a, 0x7f);

    /*testing for  CFG_ESR: addr=0x1100109c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: lec                      field_range: 21:19            field_access: w1c 
    field_name: bus_off                  field_range: 18:18            field_access: ro  
    field_name: err_pass_flag            field_range: 17:17            field_access: ro  
    field_name: err_active_flag          field_range: 16:16            field_access: ro  
    field_name: rec                      field_range: 15:8             field_access: ro  
    field_name: tec                      field_range: 7:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x1100109c, 0x10000, (0x00000000 & 0x0 | 65536), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100109c, 0x10000, (0xFFFFFFFF & 0x0 | 65536), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100109c, 0x10000, (0x55555555 & 0x0 | 65536), 0x0, 0x0);

    REG_PWR_Double_Check(0x1100109c, 0x10000, (0xAAAAAAAA & 0x0 | 65536), 0x0, 0x0);

    /*testing for  CFG_TRANS_ERR: addr=0x110010a0*/
    /*
    field_name: tx_num                   field_range: 31:0             field_access: w1c 
    */
    REG_PWR_Double_Check(0x110010a0, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010a0, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010a0, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010a0, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    /*testing for  CFG_INT_EN: addr=0x110010b4*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_en             field_range: 22:22            field_access: rw  
    field_name: rxf1_full_en             field_range: 21:21            field_access: rw  
    field_name: rxf1_wm_en               field_range: 20:20            field_access: rw  
    field_name: rxf1_fill_en             field_range: 19:19            field_access: rw  
    field_name: rxf0_lost_en             field_range: 18:18            field_access: rw  
    field_name: rxf0_full_en             field_range: 17:17            field_access: rw  
    field_name: rxf0_wm_en               field_range: 16:16            field_access: rw  
    field_name: rxf0_fill_en             field_range: 15:15            field_access: rw  
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_en     field_range: 7:7              field_access: rw  
    field_name: arbitration_lost_int_en  field_range: 6:6              field_access: rw  
    field_name: bus_off_int_en           field_range: 5:5              field_access: rw  
    field_name: error_det_int_en         field_range: 4:4              field_access: rw  
    field_name: time_stamp_wrap_en       field_range: 3:3              field_access: rw  
    field_name: txff_uf_en               field_range: 2:2              field_access: rw  
    field_name: rxff_of_en               field_range: 1:1              field_access: rw  
    field_name: rxb_upd_en               field_range: 0:0              field_access: rw  
    */
    REG_PWR_Double_Check(0x110010b4, 0x0, (0x00000000 & 0x7f80ff | 0), 0x0, 0x7f80ff);

    REG_PWR_Double_Check(0x110010b4, 0x0, (0xFFFFFFFF & 0x7f80ff | 0), 0x7f80ff, 0x7f80ff);

    REG_PWR_Double_Check(0x110010b4, 0x0, (0x55555555 & 0x7f80ff | 0), 0x550055, 0x7f80ff);

    REG_PWR_Double_Check(0x110010b4, 0x0, (0xAAAAAAAA & 0x7f80ff | 0), 0x2a80aa, 0x7f80ff);

    /*testing for  CFG_INT_STATUS: addr=0x110010b8*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_int            field_range: 22:22            field_access: ro  
    field_name: rxf1_full_int            field_range: 21:21            field_access: ro  
    field_name: rxf1_wm_int              field_range: 20:20            field_access: ro  
    field_name: rxf1_fill_int            field_range: 19:19            field_access: w1c 
    field_name: rxf0_lost_int            field_range: 18:18            field_access: ro  
    field_name: rxf0_full_int            field_range: 17:17            field_access: ro  
    field_name: rxf0_wm_int              field_range: 16:16            field_access: ro  
    field_name: rxf0_fill_int            field_range: 15:15            field_access: w1c 
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_int    field_range: 7:7              field_access: ro  
    field_name: arbitration_lost_int     field_range: 6:6              field_access: w1c 
    field_name: bus_off_int              field_range: 5:5              field_access: w1c 
    field_name: error_det_int            field_range: 4:4              field_access: w1c 
    field_name: time_stamp_wrap_int      field_range: 3:3              field_access: w1c 
    field_name: txff_uf_int              field_range: 2:2              field_access: w1c 
    field_name: rxff_of_int              field_range: 1:1              field_access: w1c 
    field_name: rxb_upd_int              field_range: 0:0              field_access: ro  
    */
    REG_PWR_Double_Check(0x110010b8, 0x0, (0x00000000 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010b8, 0x0, (0xFFFFFFFF & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010b8, 0x0, (0x55555555 & 0x0 | 0), 0x0, 0x0);

    REG_PWR_Double_Check(0x110010b8, 0x0, (0xAAAAAAAA & 0x0 | 0), 0x0, 0x0);

    delta_mcycle2=__RV_CSR_READ(CSR_MCYCLE)-start_mcycle2;

    xkan1_clk_div(0);

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
    #ifdef MISC_HAS_XKAN1_HAS_CLK

    xkan1_clk_en(DISABLE);

    /*testing for RO RELEASE: addr=0x11001000*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001000);

    /*testing for  CFG_MCR: addr=0x11001004*/
    /*
    field_name: reserved                 field_range: 31:19            field_access: ro  
    field_name: protocol_exception_handling field_range: 18:17            field_access: rw  
    field_name: be_receiver_in_loss      field_range: 16:16            field_access: rw  
    field_name: restricted_operation     field_range: 15:15            field_access: rw  
    field_name: rx_pin_pull_up           field_range: 14:14            field_access: rw  
    field_name: tx_pin_pull_up           field_range: 13:13            field_access: rw  
    field_name: clk_stop_ack             field_range: 12:12            field_access: ro  
    field_name: clk_stop_req             field_range: 11:11            field_access: rw  
    field_name: reserved                 field_range: 10:10            field_access: ro  
    field_name: txpause                  field_range: 9:9              field_access: rw  
    field_name: brs_en                   field_range: 8:8              field_access: rw  
    field_name: fd_en                    field_range: 7:7              field_access: rw  
    field_name: timestamp_en             field_range: 6:6              field_access: rw  
    field_name: reserved                 field_range: 5:5              field_access: ro  
    field_name: auto_retrans_en          field_range: 4:4              field_access: rw  
    field_name: tdc_en                   field_range: 3:3              field_access: rw  
    field_name: sleep_req                field_range: 2:2              field_access: rw  
    field_name: initial_req              field_range: 1:1              field_access: rw  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001004);

    /*testing for  CFG_TEST: addr=0x11001008*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: silm                     field_range: 1:1              field_access: rw  
    field_name: lbkm                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001008);

    /*testing for  CFG_PSR: addr=0x1100100c*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: trans_delay_comp         field_range: 22:16            field_access: ro  
    field_name: reserved                 field_range: 15:5             field_access: ro  
    field_name: pxe                      field_range: 4:4              field_access: w1c 
    field_name: esi                      field_range: 3:3              field_access: w1c 
    field_name: brs                      field_range: 2:2              field_access: w1c 
    field_name: edl                      field_range: 1:1              field_access: w1c 
    field_name: ide                      field_range: 0:0              field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x1100100c);

    /*testing for  CFG_GLOBAL_FILTER: addr=0x11001014*/
    /*
    field_name: reserved                 field_range: 31:6             field_access: ro  
    field_name: accept_miss_st_frame     field_range: 5:4              field_access: rw  
    field_name: accept_miss_ex_frame     field_range: 3:2              field_access: rw  
    field_name: reject_st_remote         field_range: 1:1              field_access: rw  
    field_name: reject_ex_remote         field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001014);

    /*testing for  CFG_ST_FILTER: addr=0x11001018*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: stflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: stflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001018);

    /*testing for  CFG_EX_FILTER: addr=0x1100101c*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: exflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: exflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1100101c);

    /*testing for  CFG_STIDM: addr=0x11001020*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: sidm                     field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001020);

    /*testing for  CFG_EXIDM: addr=0x11001024*/
    /*
    field_name: reserved                 field_range: 31:29            field_access: ro  
    field_name: eidm                     field_range: 28:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001024);

    /*testing for  CFG_NBTR: addr=0x11001028*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: nsjw                     field_range: 30:24            field_access: rw  
    field_name: ntseg2                   field_range: 23:17            field_access: rw  
    field_name: ntseg1                   field_range: 16:9             field_access: rw  
    field_name: nbrp                     field_range: 8:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001028);

    /*testing for  CFG_DBTR: addr=0x1100102c*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: dsjw                     field_range: 17:14            field_access: rw  
    field_name: dtseg2                   field_range: 13:10            field_access: rw  
    field_name: dtseg1                   field_range: 9:5              field_access: rw  
    field_name: dbrp                     field_range: 4:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1100102c);

    /*testing for  CFG_ISO_MODE: addr=0x11001030*/
    /*
    field_name: reserved                 field_range: 31:1             field_access: ro  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001030);

    /*testing for  CFG_RXDATL_RSP: addr=0x11001040*/
    /*
    field_name: low                      field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001040);

    /*testing for  CFG_RXDATH_RSP: addr=0x11001044*/
    /*
    field_name: high                     field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001044);

    /*testing for  CFG_RXFIFO0: addr=0x11001048*/
    /*
    field_name: rxff0_opm                field_range: 31:31            field_access: rw  
    field_name: rxff0_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff0_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff0_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff0_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001048);

    /*testing for  CFG_RXFIFO0_STATUS: addr=0x1100104c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff0_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff0_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff0_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff0_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff0_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1100104c);

    /*testing for  CFG_RXFIFO0_ACK: addr=0x11001050*/
    /*
    field_name: rxf0_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001050);

    /*testing for  CFG_RXBUF: addr=0x11001054*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rxbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001054);

    /*testing for  CFG_RXFIFO1: addr=0x11001058*/
    /*
    field_name: rxff1_opm                field_range: 31:31            field_access: rw  
    field_name: rxff1_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff1_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff1_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff1_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001058);

    /*testing for  CFG_RXFIFO1_STATUS: addr=0x1100105c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff1_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff1_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff1_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff1_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff1_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1100105c);

    /*testing for  CFG_RXFIFO1_ACK: addr=0x11001060*/
    /*
    field_name: rxf1_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001060);

    /*testing for  CFG_RX_DSIZE: addr=0x11001064*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rb_dsize                 field_range: 10:8             field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: f1_dsize                 field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: f0_dsize                 field_range: 2:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001064);

    /*testing for  CFG_TXBUF: addr=0x11001070*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: txfifo_clr               field_range: 30:30            field_access: wo  
    field_name: txfifo_num               field_range: 29:24            field_access: rw  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: txbuf_num                field_range: 21:16            field_access: rw  
    field_name: reserved                 field_range: 15:11            field_access: ro  
    field_name: txbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001070);

    /*testing for RO CFG_TXFIFO_STATUS: addr=0x11001074*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: txff_full                field_range: 21:21            field_access: ro  
    field_name: txff_wptr                field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: txff_rptr                field_range: 12:8             field_access: ro  
    field_name: reserved                 field_range: 7:6              field_access: ro  
    field_name: txff_entry_num           field_range: 5:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001074);

    /*testing for  CFG_TX_DSIZE: addr=0x11001078*/
    /*
    field_name: reserved                 field_range: 31:3             field_access: ro  
    field_name: txb_dsize                field_range: 2:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001078);

    /*testing for  CFG_TXABORT_FINISH: addr=0x11001084*/
    /*
    field_name: finish                   field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001084);

    /*testing for  CFG_TXREQ_PENDING: addr=0x11001088*/
    /*
    field_name: req_pending              field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001088);

    /*testing for  CFG_TXB_TRANS: addr=0x1100108c*/
    /*
    field_name: tx_complete              field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x1100108c);

    /*testing for  CFG_TIME_STAMP: addr=0x11001090*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: value                    field_range: 15:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001090);

    /*testing for  CFG_TIME_DIV: addr=0x11001094*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: div_num                  field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001094);

    /*testing for  CFG_TRANS_DELAY_COMP: addr=0x11001098*/
    /*
    field_name: reserved                 field_range: 31:7             field_access: ro  
    field_name: offset_value             field_range: 6:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001098);

    /*testing for  CFG_ESR: addr=0x1100109c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: lec                      field_range: 21:19            field_access: w1c 
    field_name: bus_off                  field_range: 18:18            field_access: ro  
    field_name: err_pass_flag            field_range: 17:17            field_access: ro  
    field_name: err_active_flag          field_range: 16:16            field_access: ro  
    field_name: rec                      field_range: 15:8             field_access: ro  
    field_name: tec                      field_range: 7:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1100109c);

    /*testing for  CFG_TRANS_ERR: addr=0x110010a0*/
    /*
    field_name: tx_num                   field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x110010a0);

    /*testing for  CFG_INT_EN: addr=0x110010b4*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_en             field_range: 22:22            field_access: rw  
    field_name: rxf1_full_en             field_range: 21:21            field_access: rw  
    field_name: rxf1_wm_en               field_range: 20:20            field_access: rw  
    field_name: rxf1_fill_en             field_range: 19:19            field_access: rw  
    field_name: rxf0_lost_en             field_range: 18:18            field_access: rw  
    field_name: rxf0_full_en             field_range: 17:17            field_access: rw  
    field_name: rxf0_wm_en               field_range: 16:16            field_access: rw  
    field_name: rxf0_fill_en             field_range: 15:15            field_access: rw  
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_en     field_range: 7:7              field_access: rw  
    field_name: arbitration_lost_int_en  field_range: 6:6              field_access: rw  
    field_name: bus_off_int_en           field_range: 5:5              field_access: rw  
    field_name: error_det_int_en         field_range: 4:4              field_access: rw  
    field_name: time_stamp_wrap_en       field_range: 3:3              field_access: rw  
    field_name: txff_uf_en               field_range: 2:2              field_access: rw  
    field_name: rxff_of_en               field_range: 1:1              field_access: rw  
    field_name: rxb_upd_en               field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x110010b4);

    /*testing for  CFG_INT_STATUS: addr=0x110010b8*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_int            field_range: 22:22            field_access: ro  
    field_name: rxf1_full_int            field_range: 21:21            field_access: ro  
    field_name: rxf1_wm_int              field_range: 20:20            field_access: ro  
    field_name: rxf1_fill_int            field_range: 19:19            field_access: w1c 
    field_name: rxf0_lost_int            field_range: 18:18            field_access: ro  
    field_name: rxf0_full_int            field_range: 17:17            field_access: ro  
    field_name: rxf0_wm_int              field_range: 16:16            field_access: ro  
    field_name: rxf0_fill_int            field_range: 15:15            field_access: w1c 
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_int    field_range: 7:7              field_access: ro  
    field_name: arbitration_lost_int     field_range: 6:6              field_access: w1c 
    field_name: bus_off_int              field_range: 5:5              field_access: w1c 
    field_name: error_det_int            field_range: 4:4              field_access: w1c 
    field_name: time_stamp_wrap_int      field_range: 3:3              field_access: w1c 
    field_name: txff_uf_int              field_range: 2:2              field_access: w1c 
    field_name: rxff_of_int              field_range: 1:1              field_access: w1c 
    field_name: rxb_upd_int              field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x110010b8);

    xkan1_clk_en(ENABLE);

    #endif

    /*********************TEST FOR CLOCK RESET*********************/

    #ifdef MISC_HAS_XKAN1_RST

    xkan1_set_rst(DISABLE);

    /*testing for RO RELEASE: addr=0x11001000*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001000);

    /*testing for  CFG_MCR: addr=0x11001004*/
    /*
    field_name: reserved                 field_range: 31:19            field_access: ro  
    field_name: protocol_exception_handling field_range: 18:17            field_access: rw  
    field_name: be_receiver_in_loss      field_range: 16:16            field_access: rw  
    field_name: restricted_operation     field_range: 15:15            field_access: rw  
    field_name: rx_pin_pull_up           field_range: 14:14            field_access: rw  
    field_name: tx_pin_pull_up           field_range: 13:13            field_access: rw  
    field_name: clk_stop_ack             field_range: 12:12            field_access: ro  
    field_name: clk_stop_req             field_range: 11:11            field_access: rw  
    field_name: reserved                 field_range: 10:10            field_access: ro  
    field_name: txpause                  field_range: 9:9              field_access: rw  
    field_name: brs_en                   field_range: 8:8              field_access: rw  
    field_name: fd_en                    field_range: 7:7              field_access: rw  
    field_name: timestamp_en             field_range: 6:6              field_access: rw  
    field_name: reserved                 field_range: 5:5              field_access: ro  
    field_name: auto_retrans_en          field_range: 4:4              field_access: rw  
    field_name: tdc_en                   field_range: 3:3              field_access: rw  
    field_name: sleep_req                field_range: 2:2              field_access: rw  
    field_name: initial_req              field_range: 1:1              field_access: rw  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001004);

    /*testing for  CFG_TEST: addr=0x11001008*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: silm                     field_range: 1:1              field_access: rw  
    field_name: lbkm                     field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001008);

    /*testing for  CFG_PSR: addr=0x1100100c*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: trans_delay_comp         field_range: 22:16            field_access: ro  
    field_name: reserved                 field_range: 15:5             field_access: ro  
    field_name: pxe                      field_range: 4:4              field_access: w1c 
    field_name: esi                      field_range: 3:3              field_access: w1c 
    field_name: brs                      field_range: 2:2              field_access: w1c 
    field_name: edl                      field_range: 1:1              field_access: w1c 
    field_name: ide                      field_range: 0:0              field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x1100100c);

    /*testing for  CFG_GLOBAL_FILTER: addr=0x11001014*/
    /*
    field_name: reserved                 field_range: 31:6             field_access: ro  
    field_name: accept_miss_st_frame     field_range: 5:4              field_access: rw  
    field_name: accept_miss_ex_frame     field_range: 3:2              field_access: rw  
    field_name: reject_st_remote         field_range: 1:1              field_access: rw  
    field_name: reject_ex_remote         field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001014);

    /*testing for  CFG_ST_FILTER: addr=0x11001018*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: stflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: stflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001018);

    /*testing for  CFG_EX_FILTER: addr=0x1100101c*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: exflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: exflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1100101c);

    /*testing for  CFG_STIDM: addr=0x11001020*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: sidm                     field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001020);

    /*testing for  CFG_EXIDM: addr=0x11001024*/
    /*
    field_name: reserved                 field_range: 31:29            field_access: ro  
    field_name: eidm                     field_range: 28:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001024);

    /*testing for  CFG_NBTR: addr=0x11001028*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: nsjw                     field_range: 30:24            field_access: rw  
    field_name: ntseg2                   field_range: 23:17            field_access: rw  
    field_name: ntseg1                   field_range: 16:9             field_access: rw  
    field_name: nbrp                     field_range: 8:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001028);

    /*testing for  CFG_DBTR: addr=0x1100102c*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: dsjw                     field_range: 17:14            field_access: rw  
    field_name: dtseg2                   field_range: 13:10            field_access: rw  
    field_name: dtseg1                   field_range: 9:5              field_access: rw  
    field_name: dbrp                     field_range: 4:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x1100102c);

    /*testing for  CFG_ISO_MODE: addr=0x11001030*/
    /*
    field_name: reserved                 field_range: 31:1             field_access: ro  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001030);

    /*testing for  CFG_RXDATL_RSP: addr=0x11001040*/
    /*
    field_name: low                      field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001040);

    /*testing for  CFG_RXDATH_RSP: addr=0x11001044*/
    /*
    field_name: high                     field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001044);

    /*testing for  CFG_RXFIFO0: addr=0x11001048*/
    /*
    field_name: rxff0_opm                field_range: 31:31            field_access: rw  
    field_name: rxff0_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff0_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff0_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff0_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001048);

    /*testing for  CFG_RXFIFO0_STATUS: addr=0x1100104c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff0_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff0_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff0_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff0_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff0_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1100104c);

    /*testing for  CFG_RXFIFO0_ACK: addr=0x11001050*/
    /*
    field_name: rxf0_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001050);

    /*testing for  CFG_RXBUF: addr=0x11001054*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rxbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001054);

    /*testing for  CFG_RXFIFO1: addr=0x11001058*/
    /*
    field_name: rxff1_opm                field_range: 31:31            field_access: rw  
    field_name: rxff1_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff1_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff1_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff1_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001058);

    /*testing for  CFG_RXFIFO1_STATUS: addr=0x1100105c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff1_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff1_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff1_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff1_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff1_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1100105c);

    /*testing for  CFG_RXFIFO1_ACK: addr=0x11001060*/
    /*
    field_name: rxf1_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001060);

    /*testing for  CFG_RX_DSIZE: addr=0x11001064*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rb_dsize                 field_range: 10:8             field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: f1_dsize                 field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: f0_dsize                 field_range: 2:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001064);

    /*testing for  CFG_TXBUF: addr=0x11001070*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: txfifo_clr               field_range: 30:30            field_access: wo  
    field_name: txfifo_num               field_range: 29:24            field_access: rw  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: txbuf_num                field_range: 21:16            field_access: rw  
    field_name: reserved                 field_range: 15:11            field_access: ro  
    field_name: txbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001070);

    /*testing for RO CFG_TXFIFO_STATUS: addr=0x11001074*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: txff_full                field_range: 21:21            field_access: ro  
    field_name: txff_wptr                field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: txff_rptr                field_range: 12:8             field_access: ro  
    field_name: reserved                 field_range: 7:6              field_access: ro  
    field_name: txff_entry_num           field_range: 5:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x11001074);

    /*testing for  CFG_TX_DSIZE: addr=0x11001078*/
    /*
    field_name: reserved                 field_range: 31:3             field_access: ro  
    field_name: txb_dsize                field_range: 2:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001078);

    /*testing for  CFG_TXABORT_FINISH: addr=0x11001084*/
    /*
    field_name: finish                   field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001084);

    /*testing for  CFG_TXREQ_PENDING: addr=0x11001088*/
    /*
    field_name: req_pending              field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001088);

    /*testing for  CFG_TXB_TRANS: addr=0x1100108c*/
    /*
    field_name: tx_complete              field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x1100108c);

    /*testing for  CFG_TIME_STAMP: addr=0x11001090*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: value                    field_range: 15:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x11001090);

    /*testing for  CFG_TIME_DIV: addr=0x11001094*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: div_num                  field_range: 3:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001094);

    /*testing for  CFG_TRANS_DELAY_COMP: addr=0x11001098*/
    /*
    field_name: reserved                 field_range: 31:7             field_access: ro  
    field_name: offset_value             field_range: 6:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x11001098);

    /*testing for  CFG_ESR: addr=0x1100109c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: lec                      field_range: 21:19            field_access: w1c 
    field_name: bus_off                  field_range: 18:18            field_access: ro  
    field_name: err_pass_flag            field_range: 17:17            field_access: ro  
    field_name: err_active_flag          field_range: 16:16            field_access: ro  
    field_name: rec                      field_range: 15:8             field_access: ro  
    field_name: tec                      field_range: 7:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x1100109c);

    /*testing for  CFG_TRANS_ERR: addr=0x110010a0*/
    /*
    field_name: tx_num                   field_range: 31:0             field_access: w1c 
    */
    REG_CLK_DIS_CHECK(0x110010a0);

    /*testing for  CFG_INT_EN: addr=0x110010b4*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_en             field_range: 22:22            field_access: rw  
    field_name: rxf1_full_en             field_range: 21:21            field_access: rw  
    field_name: rxf1_wm_en               field_range: 20:20            field_access: rw  
    field_name: rxf1_fill_en             field_range: 19:19            field_access: rw  
    field_name: rxf0_lost_en             field_range: 18:18            field_access: rw  
    field_name: rxf0_full_en             field_range: 17:17            field_access: rw  
    field_name: rxf0_wm_en               field_range: 16:16            field_access: rw  
    field_name: rxf0_fill_en             field_range: 15:15            field_access: rw  
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_en     field_range: 7:7              field_access: rw  
    field_name: arbitration_lost_int_en  field_range: 6:6              field_access: rw  
    field_name: bus_off_int_en           field_range: 5:5              field_access: rw  
    field_name: error_det_int_en         field_range: 4:4              field_access: rw  
    field_name: time_stamp_wrap_en       field_range: 3:3              field_access: rw  
    field_name: txff_uf_en               field_range: 2:2              field_access: rw  
    field_name: rxff_of_en               field_range: 1:1              field_access: rw  
    field_name: rxb_upd_en               field_range: 0:0              field_access: rw  
    */
    REG_CLK_DIS_CHECK(0x110010b4);

    /*testing for  CFG_INT_STATUS: addr=0x110010b8*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_int            field_range: 22:22            field_access: ro  
    field_name: rxf1_full_int            field_range: 21:21            field_access: ro  
    field_name: rxf1_wm_int              field_range: 20:20            field_access: ro  
    field_name: rxf1_fill_int            field_range: 19:19            field_access: w1c 
    field_name: rxf0_lost_int            field_range: 18:18            field_access: ro  
    field_name: rxf0_full_int            field_range: 17:17            field_access: ro  
    field_name: rxf0_wm_int              field_range: 16:16            field_access: ro  
    field_name: rxf0_fill_int            field_range: 15:15            field_access: w1c 
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_int    field_range: 7:7              field_access: ro  
    field_name: arbitration_lost_int     field_range: 6:6              field_access: w1c 
    field_name: bus_off_int              field_range: 5:5              field_access: w1c 
    field_name: error_det_int            field_range: 4:4              field_access: w1c 
    field_name: time_stamp_wrap_int      field_range: 3:3              field_access: w1c 
    field_name: txff_uf_int              field_range: 2:2              field_access: w1c 
    field_name: rxff_of_int              field_range: 1:1              field_access: w1c 
    field_name: rxb_upd_int              field_range: 0:0              field_access: ro  
    */
    REG_CLK_DIS_CHECK(0x110010b8);

    xkan1_set_rst(ENABLE);

    /*testing for RO RELEASE: addr=0x11001000*/
    /*
    field_name: version                  field_range: 31:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x11001000,0x20201);

    /*testing for  CFG_MCR: addr=0x11001004*/
    /*
    field_name: reserved                 field_range: 31:19            field_access: ro  
    field_name: protocol_exception_handling field_range: 18:17            field_access: rw  
    field_name: be_receiver_in_loss      field_range: 16:16            field_access: rw  
    field_name: restricted_operation     field_range: 15:15            field_access: rw  
    field_name: rx_pin_pull_up           field_range: 14:14            field_access: rw  
    field_name: tx_pin_pull_up           field_range: 13:13            field_access: rw  
    field_name: clk_stop_ack             field_range: 12:12            field_access: ro  
    field_name: clk_stop_req             field_range: 11:11            field_access: rw  
    field_name: reserved                 field_range: 10:10            field_access: ro  
    field_name: txpause                  field_range: 9:9              field_access: rw  
    field_name: brs_en                   field_range: 8:8              field_access: rw  
    field_name: fd_en                    field_range: 7:7              field_access: rw  
    field_name: timestamp_en             field_range: 6:6              field_access: rw  
    field_name: reserved                 field_range: 5:5              field_access: ro  
    field_name: auto_retrans_en          field_range: 4:4              field_access: rw  
    field_name: tdc_en                   field_range: 3:3              field_access: rw  
    field_name: sleep_req                field_range: 2:2              field_access: rw  
    field_name: initial_req              field_range: 1:1              field_access: rw  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001004,0x36004);

    /*testing for  CFG_TEST: addr=0x11001008*/
    /*
    field_name: reserved                 field_range: 31:2             field_access: ro  
    field_name: silm                     field_range: 1:1              field_access: rw  
    field_name: lbkm                     field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001008,0x0);

    /*testing for  CFG_PSR: addr=0x1100100c*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: trans_delay_comp         field_range: 22:16            field_access: ro  
    field_name: reserved                 field_range: 15:5             field_access: ro  
    field_name: pxe                      field_range: 4:4              field_access: w1c 
    field_name: esi                      field_range: 3:3              field_access: w1c 
    field_name: brs                      field_range: 2:2              field_access: w1c 
    field_name: edl                      field_range: 1:1              field_access: w1c 
    field_name: ide                      field_range: 0:0              field_access: w1c 
    */
    REG_DefVal_CHECK(0x1100100c,0x0);

    /*testing for  CFG_GLOBAL_FILTER: addr=0x11001014*/
    /*
    field_name: reserved                 field_range: 31:6             field_access: ro  
    field_name: accept_miss_st_frame     field_range: 5:4              field_access: rw  
    field_name: accept_miss_ex_frame     field_range: 3:2              field_access: rw  
    field_name: reject_st_remote         field_range: 1:1              field_access: rw  
    field_name: reject_ex_remote         field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001014,0x3c);

    /*testing for  CFG_ST_FILTER: addr=0x11001018*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: stflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: stflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001018,0x0);

    /*testing for  CFG_EX_FILTER: addr=0x1100101c*/
    /*
    field_name: reserved                 field_range: 31:20            field_access: ro  
    field_name: exflt_num                field_range: 19:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: exflt_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x1100101c,0x0);

    /*testing for  CFG_STIDM: addr=0x11001020*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: sidm                     field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001020,0x7ff);

    /*testing for  CFG_EXIDM: addr=0x11001024*/
    /*
    field_name: reserved                 field_range: 31:29            field_access: ro  
    field_name: eidm                     field_range: 28:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001024,0x1fffffff);

    /*testing for  CFG_NBTR: addr=0x11001028*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: nsjw                     field_range: 30:24            field_access: rw  
    field_name: ntseg2                   field_range: 23:17            field_access: rw  
    field_name: ntseg1                   field_range: 16:9             field_access: rw  
    field_name: nbrp                     field_range: 8:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001028,0x1040600);

    /*testing for  CFG_DBTR: addr=0x1100102c*/
    /*
    field_name: reserved                 field_range: 31:18            field_access: ro  
    field_name: dsjw                     field_range: 17:14            field_access: rw  
    field_name: dtseg2                   field_range: 13:10            field_access: rw  
    field_name: dtseg1                   field_range: 9:5              field_access: rw  
    field_name: dbrp                     field_range: 4:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x1100102c,0x0);

    /*testing for  CFG_ISO_MODE: addr=0x11001030*/
    /*
    field_name: reserved                 field_range: 31:1             field_access: ro  
    field_name: enable                   field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001030,0x1);

    /*testing for  CFG_RXDATL_RSP: addr=0x11001040*/
    /*
    field_name: low                      field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001040,0x0);

    /*testing for  CFG_RXDATH_RSP: addr=0x11001044*/
    /*
    field_name: high                     field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001044,0x0);

    /*testing for  CFG_RXFIFO0: addr=0x11001048*/
    /*
    field_name: rxff0_opm                field_range: 31:31            field_access: rw  
    field_name: rxff0_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff0_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff0_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff0_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001048,0x0);

    /*testing for  CFG_RXFIFO0_STATUS: addr=0x1100104c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff0_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff0_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff0_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff0_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff0_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1100104c,0x0);

    /*testing for  CFG_RXFIFO0_ACK: addr=0x11001050*/
    /*
    field_name: rxf0_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x11001050,0x0);

    /*testing for  CFG_RXBUF: addr=0x11001054*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rxbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001054,0x0);

    /*testing for  CFG_RXFIFO1: addr=0x11001058*/
    /*
    field_name: rxff1_opm                field_range: 31:31            field_access: rw  
    field_name: rxff1_clr                field_range: 30:30            field_access: wo  
    field_name: reserved                 field_range: 29:26            field_access: ro  
    field_name: rxff1_wm                 field_range: 25:19            field_access: rw  
    field_name: rxff1_num                field_range: 18:12            field_access: rw  
    field_name: reserved                 field_range: 11:11            field_access: ro  
    field_name: rxff1_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001058,0x0);

    /*testing for  CFG_RXFIFO1_STATUS: addr=0x1100105c*/
    /*
    field_name: reserved                 field_range: 31:26            field_access: ro  
    field_name: rxff1_lost               field_range: 25:25            field_access: w1c 
    field_name: rxff1_full               field_range: 24:24            field_access: ro  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: rxff1_wptr               field_range: 21:16            field_access: ro  
    field_name: reserved                 field_range: 15:14            field_access: ro  
    field_name: rxff1_rptr               field_range: 13:8             field_access: ro  
    field_name: rxff1_entry_num          field_range: 7:1              field_access: ro  
    field_name: reserved                 field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1100105c,0x0);

    /*testing for  CFG_RXFIFO1_ACK: addr=0x11001060*/
    /*
    field_name: rxf1_ack                 field_range: 31:31            field_access: wo  
    field_name: reserved                 field_range: 30:0             field_access: ro  
    */
    REG_DefVal_CHECK(0x11001060,0x0);

    /*testing for  CFG_RX_DSIZE: addr=0x11001064*/
    /*
    field_name: reserved                 field_range: 31:11            field_access: ro  
    field_name: rb_dsize                 field_range: 10:8             field_access: rw  
    field_name: reserved                 field_range: 7:7              field_access: ro  
    field_name: f1_dsize                 field_range: 6:4              field_access: rw  
    field_name: reserved                 field_range: 3:3              field_access: ro  
    field_name: f0_dsize                 field_range: 2:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001064,0x0);

    /*testing for  CFG_TXBUF: addr=0x11001070*/
    /*
    field_name: reserved                 field_range: 31:31            field_access: ro  
    field_name: txfifo_clr               field_range: 30:30            field_access: wo  
    field_name: txfifo_num               field_range: 29:24            field_access: rw  
    field_name: reserved                 field_range: 23:22            field_access: ro  
    field_name: txbuf_num                field_range: 21:16            field_access: rw  
    field_name: reserved                 field_range: 15:11            field_access: ro  
    field_name: txbuf_addr               field_range: 10:0             field_access: rw  
    */
    REG_DefVal_CHECK(0x11001070,0x0);

    /*testing for RO CFG_TXFIFO_STATUS: addr=0x11001074*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: txff_full                field_range: 21:21            field_access: ro  
    field_name: txff_wptr                field_range: 20:16            field_access: ro  
    field_name: reserved                 field_range: 15:13            field_access: ro  
    field_name: txff_rptr                field_range: 12:8             field_access: ro  
    field_name: reserved                 field_range: 7:6              field_access: ro  
    field_name: txff_entry_num           field_range: 5:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x11001074,0x0);

    /*testing for  CFG_TX_DSIZE: addr=0x11001078*/
    /*
    field_name: reserved                 field_range: 31:3             field_access: ro  
    field_name: txb_dsize                field_range: 2:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001078,0x0);

    /*testing for  CFG_TXABORT_FINISH: addr=0x11001084*/
    /*
    field_name: finish                   field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001084,0x0);

    /*testing for  CFG_TXREQ_PENDING: addr=0x11001088*/
    /*
    field_name: req_pending              field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001088,0x0);

    /*testing for  CFG_TXB_TRANS: addr=0x1100108c*/
    /*
    field_name: tx_complete              field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x1100108c,0x0);

    /*testing for  CFG_TIME_STAMP: addr=0x11001090*/
    /*
    field_name: reserved                 field_range: 31:16            field_access: ro  
    field_name: value                    field_range: 15:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x11001090,0x0);

    /*testing for  CFG_TIME_DIV: addr=0x11001094*/
    /*
    field_name: reserved                 field_range: 31:4             field_access: ro  
    field_name: div_num                  field_range: 3:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001094,0x0);

    /*testing for  CFG_TRANS_DELAY_COMP: addr=0x11001098*/
    /*
    field_name: reserved                 field_range: 31:7             field_access: ro  
    field_name: offset_value             field_range: 6:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x11001098,0x0);

    /*testing for  CFG_ESR: addr=0x1100109c*/
    /*
    field_name: reserved                 field_range: 31:22            field_access: ro  
    field_name: lec                      field_range: 21:19            field_access: w1c 
    field_name: bus_off                  field_range: 18:18            field_access: ro  
    field_name: err_pass_flag            field_range: 17:17            field_access: ro  
    field_name: err_active_flag          field_range: 16:16            field_access: ro  
    field_name: rec                      field_range: 15:8             field_access: ro  
    field_name: tec                      field_range: 7:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x1100109c,0x10000);

    /*testing for  CFG_TRANS_ERR: addr=0x110010a0*/
    /*
    field_name: tx_num                   field_range: 31:0             field_access: w1c 
    */
    REG_DefVal_CHECK(0x110010a0,0x0);

    /*testing for  CFG_INT_EN: addr=0x110010b4*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_en             field_range: 22:22            field_access: rw  
    field_name: rxf1_full_en             field_range: 21:21            field_access: rw  
    field_name: rxf1_wm_en               field_range: 20:20            field_access: rw  
    field_name: rxf1_fill_en             field_range: 19:19            field_access: rw  
    field_name: rxf0_lost_en             field_range: 18:18            field_access: rw  
    field_name: rxf0_full_en             field_range: 17:17            field_access: rw  
    field_name: rxf0_wm_en               field_range: 16:16            field_access: rw  
    field_name: rxf0_fill_en             field_range: 15:15            field_access: rw  
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_en     field_range: 7:7              field_access: rw  
    field_name: arbitration_lost_int_en  field_range: 6:6              field_access: rw  
    field_name: bus_off_int_en           field_range: 5:5              field_access: rw  
    field_name: error_det_int_en         field_range: 4:4              field_access: rw  
    field_name: time_stamp_wrap_en       field_range: 3:3              field_access: rw  
    field_name: txff_uf_en               field_range: 2:2              field_access: rw  
    field_name: rxff_of_en               field_range: 1:1              field_access: rw  
    field_name: rxb_upd_en               field_range: 0:0              field_access: rw  
    */
    REG_DefVal_CHECK(0x110010b4,0x0);

    /*testing for  CFG_INT_STATUS: addr=0x110010b8*/
    /*
    field_name: reserved                 field_range: 31:23            field_access: ro  
    field_name: rxf1_lost_int            field_range: 22:22            field_access: ro  
    field_name: rxf1_full_int            field_range: 21:21            field_access: ro  
    field_name: rxf1_wm_int              field_range: 20:20            field_access: ro  
    field_name: rxf1_fill_int            field_range: 19:19            field_access: w1c 
    field_name: rxf0_lost_int            field_range: 18:18            field_access: ro  
    field_name: rxf0_full_int            field_range: 17:17            field_access: ro  
    field_name: rxf0_wm_int              field_range: 16:16            field_access: ro  
    field_name: rxf0_fill_int            field_range: 15:15            field_access: w1c 
    field_name: reserved                 field_range: 14:8             field_access: ro  
    field_name: tx_trans_complete_int    field_range: 7:7              field_access: ro  
    field_name: arbitration_lost_int     field_range: 6:6              field_access: w1c 
    field_name: bus_off_int              field_range: 5:5              field_access: w1c 
    field_name: error_det_int            field_range: 4:4              field_access: w1c 
    field_name: time_stamp_wrap_int      field_range: 3:3              field_access: w1c 
    field_name: txff_uf_int              field_range: 2:2              field_access: w1c 
    field_name: rxff_of_int              field_range: 1:1              field_access: w1c 
    field_name: rxb_upd_int              field_range: 0:0              field_access: ro  
    */
    REG_DefVal_CHECK(0x110010b8,0x0);

    #endif

    #ifdef MISC_HAS_XKAN1_CLK_DIV

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
