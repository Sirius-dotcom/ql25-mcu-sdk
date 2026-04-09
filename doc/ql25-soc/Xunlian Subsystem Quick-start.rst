Nuclei® Xunlian Subsystem Quick-start
Manual
Nucleisys
All rights reserved by Nucleisys, Inc.
Contents
1 Copyright Notice 3
2 Contact Information 4
3 Nuclei SoC Subsystem Delivery Package 5
3.1 File Package Hierarchy . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 5
3.2 View RTL Code . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 6
4 System Simulation 7
4.1 C/C++ Test Case . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 7
4.1.1 The user added Ctest . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8
4.2 Simulation Platform . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8
4.3 Run C Test simulation . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8
4.4 Run SRAM Scan . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 10
4.5 Connect JTAG GDB . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 10
4.5.1 Integrate JTAGVPI into your env . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 13
5 FPGA Validation 14
5.1 Generate FPGA Version . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 14
5.2 Run C Test Use Case . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 15
5.3 USART Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 16
5.3.1 Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 16
5.3.2 Running results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 16
5.4 SPI Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 17
5.4.1 Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 17
5.4.2 Running results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 17
5.4.3 Read and Write Flash Test . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 17
5.5 TIMER Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 18
5.5.1 Test Program Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 18
5.5.2 Running Results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 18
5.6 TIMER_pwmout Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 18
5.6.1 Test Program Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 18
5.6.2 Running Results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
5.7 I2C Eeprom-AT24C02 Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
5.7.1 Test Program Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
5.8 SDIO Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
5.8.1 Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
5.8.2 Running Results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
5.9 Ethernet Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
5.9.1 Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
5.9.2 Running Results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
5.10 USB Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21
5.10.1 Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21
i
5.10.2 Running Results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21
5.11 CAN Functional Testing . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 22
5.11.1 Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 22
5.11.2 Running Results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 22
6 DC Synthesis 23
6.1 Synthesis Steps . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 23
7 Caution 24
8 Appendix 25
ii
List of Figures
4.1 Printf simulation output results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 10
4.2 Waiting for JTAG connection . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 11
4.3 JTAG connection information . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 12
4.4 JTAG connection successful . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 12
4.5 GDB Start . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 12
4.6 GDB debugging helloworld program . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 13
5.1 Helloworld sample program C test run results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 16
5.2 USART operation results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 16
5.3 Test results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 17
5.4 Read and write flash test results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 18
5.5 TIMER pwmout test results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
5.6 I2C Eeprom-AT24C02I2C test results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19
5.7 SDIO running results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20
5.8 Ethernet running results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21
5.9 USB running results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21
5.10 CAN running results . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 22
1
List of Tables
3.1 The Package Introduction . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 5
2
1
Copyright Notice
Copyright© 2018-2026 Nuclei System Technology. All rights reserved.
Nuclei®, Nucleisys®, NMSIS®, ECLIC®, are trademarks owned by Nuclei System Technology. All other trademarks
used herein are the property of their respective owners.
The product described herein is subject to continuous development and improvement; information herein is given by Nuclei
in good faith but without warranties.
This document is intended only to assist the reader in the use of the product. Nuclei do not assume any liability arising
out of the application or use of any product or circuit, and specifically disclaims any and all liability, including without
limitation indirect, incidental, special, exemplary, or consequential damages. incorrect use of the product.
3
2
Contact Information
Should you have any problems with the information contained herein or any suggestions, please contact Nuclei System
Technology by email support@nucleisys.com, or visit “Nuclei User Center” website http://user.nucleisys.com for supports
or online discussion.
Commit: f07f7171bc293a6626753c40c5abfd22ceee172a
4
3
Nuclei SoC Subsystem Delivery Package
The content of the delivered by the Nuclei SoC Subsystem, xunlian_rls_pkg.tar.gz is a file compressed package, the intro duction is shown in The Package Introduction (page 5) .
The following is the version of EDA tool we use:
synopsys/dc-2021.06-SP1
synopsys/vcs-2022.06-SP2-1
synopsys/verdi-2022.06-SP2-1
fpga/vitis-2021.1
python/python-3.7.3
Table 3.1: The Package Introduction
Package Name xunlian_*.tar.gz
Introduction Including Testbench for the Verilog RTL delivery,SoC,Simulation Environment and
FPGA source code file compression package.
The files of Nuclei SoC Subsystem be obtained from Nuclei. Decompress the xunlian_*.tar.gz folder in the Linux system.
# Decompress the xunlian_rls_pkg to the native Linux environment.
# See the readme file of the FTP path for details.
$ unzip private_*.zip
# Enter password
$ tar -zxvf xunlian.tar.gz
$ cp private.pem ./xunlian_rls_pkg/
$ cd xunlian_rls_pkg
$ source env.sh(env.csh)
$ ./nuclei_soc_gen
3.1 File Package Hierarchy
The files in the xunlian_rls_pkg.tar.gz package are introduced as below(subject to the actual delivery ip).
xunlian_rls_pkg
xunlian
rtl # Store rtl code for RISC-V SoC designs
core # CPU rtl code
main_fab # Store the rtl code for the SoC bus fabric
usart # Store the rtl code for the usart
<ip_name> # Store the rtl code for the <ip_name>
(continues on next page)
5
Nuclei® Xunlian Subsystem Quick-start Manual
(continued from previous page)
...
doc # Reference Manual
addr_map.csv # Memory map and register boundary addresses
Nuclei_xunlian_Subsystem_Quick_Start_Manual.pdf
Nuclei_xunlian_Reference_Manual.pdf
<cpu>-doc-<version>.zip # CPU Reference Manual
...
Makefile # Main Makefile
syn # Store synthetical netlist information
test_run # Information generated when running tests
testbench # Directory for Verilog Testbench
jtag_vpi # Storage of JTAG related file
flash_model # Storage of flashmodel related files
tb_*.v # Simple Verilog TestBench file.
...
tests # Directory where test cases are stored
core0
system_test # Storage of ctest test cases
fpga # Directory where FPGA projects and scripts are stored
nuclei-sdk # Store the SoC corresponding driver and Ctest test
...
Note
RTL contains a lot of RTL source code, which contains the processor core and all the synthesizable RTL source code
of SoC. Users only need to care about the IP in the subsystem, and do not need to care about the IP without it.
3.2 View RTL Code
Suppose the user wants to quickly view and compile the source code of the Nuclei SoC Subsystem, you can use the
following steps to proceed.
# Step 1: Go to the xunlian_rls_pkg/xunlian directory.
$ cd xunlian_rls_pkg/xunlian
# Step 2: View RTL code.
$ make verdi_rtl
3.2. View RTL Code 6
4
System Simulation
Nuclei SoC Subsystem provides software development kits (SDK, Software Development Kit). For details, please refer to
Nuclei-SDK User Guide.
4.1 C/C++ Test Case
The C/C++ test cases are some demo programs developed by Nuclei, which need to be carried out with the help of nuclei sdk. The relevant C program files are stored in the following directory(subject to the actual delivery ip).
xunlian_rls_pkg
xunlian
nuclei-sdk
soc_test_cases
ns # Store the test case directory of the soc
USART # USART test program
I2C # I2C test program
WWDG # WWDG test program
LGPIO # LGPIO test program
SPI # SPI test program
ADV_TIMER # ADV_TIMER test program
BSC_TIMER # BSC_TIMER test program
top_ctest # top test program
...
These test programs are written in C/C++ language. Under nuclei-sdk/soc_test_cases/ns , include script files, C/C++ and
header files. If the compiled file does not exist, the user needs to compile the C/C++ program by himself. The following
is an example of Printf test case of usart0. The compiled program is an elf file and a disassembly file, as well as a file that
can be read by the readmemh function of Verilog, as shown below.
USART
usart0 # usart0 test case
printf # Printf test case
main.c # C source File
Makefile
README.TXT # test README file
USART_usart0_printf.elf # Compiled elf file
USART_usart0_printf.dump # Disassembly file
USART_usart0_printf.verilog # can be read by the $readmemh
...
Note
7
Nuclei® Xunlian Subsystem Quick-start Manual
The *.elf, *.dump, *.verilog files are generated only after the test program is executed.
4.1.1 The user added Ctest
The new cases follow the following directory structure:
1.
nuclei-sdk/soc_test_cases/ns/<IP_NAME>/<ip_instance_name>/<case_name>/main.c
nuclei-sdk/soc_test_cases/ns/<IP_NAME>/<ip_instance_name>/<case_name>/Makefile
Ctest name is <IP_NAME>_<ip_instance_name>_<case_name>
2.
nuclei-sdk/soc_test_cases/ns/top_ctest/<ctest_name>/main.c
nuclei-sdk/soc_test_cases/ns/top_ctest/<ctest_name>/Makefile
Ctest name is <ctest_name>
Ctests for both of these rules can be compiled using ctest_gun
How to use ctest_gun see :$ ctest_gun –help
4.2 Simulation Platform
A simple Testbench written in Verilog has been created in the xunlian_rls_pkg/xunlian directory.
xunlian_rls_pkg
xunlian
testbench # The Verilog TestBench(Test platform)
tb_*.v # Simple Verilog TestBench file.
...
The main functions in the testbed are as follows:
• Routine DUT file, generate clock and reset signals
• The name of the test case is resolved according to the run command, and the contents of the corresponding file (e.g.
USART_usart0_printf.verilog) are read in using Verilog’s readmemh function, and then the contents of the file are
used to initialize the SoC’s Instruction Menory (a two-dimensional array written in Verilog that acts as the SRAM
behavior model).
Note
Users can also integrate correlation tb_*.v into the SoC to facilitate running self-test cases in the SoC environment.
4.3 Run C Test simulation
Assuming that a program written in C/C++ is run in the user simulation environment, this can be done using the following
steps. Using the sample program(Printf) as an example, you can proceed using the following steps. The SDK, which
requires users to contact Nuclei to obtain.
Note
If there is already a *.verilog file under the provided tests/core0/system_tests path, it does not need to be compiled by
the user.
4.2. Simulation Platform 8
Nuclei® Xunlian Subsystem Quick-start Manual
Step 1:Compile c test programs.
$ cd xunlian_rls_pkg/xunlian
$ chmod +x testbench/script/ctest_gun
# ilm Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=ilm
# sramxip Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=sramxip
# flash Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=flash<0/1/2>
# flashxip Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=flashxip<0/1/2>
Step 2:Run the C test executable under the simulation platform.
$ cd xunlian_rls_pkg/xunlian
# ilm Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_ilm
# sramxip Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_sramxip
# flash Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_flash<0/1/2>
# flashxip Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_flashxip<0/1/2>
# make run will compile the rtl code and then execute the Printf sample program under
# the path.
# ilm Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_ilm
# sramxip Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_sramxip
# flash Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_flash<0/1/2>
# flashxip Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_flashxip<0/1/2>
# make ncrun will directly execute the Printf sample program under the path.
Note
make run is equivalent to make compile and make ncrun. When switching ctest, you do not need to recompile rtl.
Please use make ncrun to save time
Step 3:Run wave(View Waveform),use the following command.
# ilm Call verdi steps:
$ make verdi TESTNAME=USART_usart0_printf_ilm
# sramxip Call verdi steps:
$ make verdi TESTNAME=USART_usart0_printf_sramxip<0/1/2>
# flash Call verdi steps:
$ make verdi TESTNAME=USART_usart0_printf_flash<0/1/2>
# flashxip Call verdi steps:
$ make verdi TESTNAME=USART_usart0_printf_flashxip<0/1/2>
# Call verdi to see the waveform after this test is executed.
# Module core_wrapper under Trace and other related interfaces are used to output the
# internal status information required by external devices, which can debug with this
# information.
C Test simulation results (Printf for example) as shown in Printf simulation output results (page 10) .
4.3. Run C Test simulation 9
Nuclei® Xunlian Subsystem Quick-start Manual
Fig. 4.1: Printf simulation output results
4.4 Run SRAM Scan
The Verilog Testbench source codes are under the “testbench” directory as below.
xunlian_rls_pkg
xunlian
testbench
tb_sram_scan.v # sram scan source codes
...
The SRAM Scan module is used to test all SRAM of the Core/Cluster, if user has replace the SRAM, then can integrate
this module to the SoC and run the task to check the SRAM replacement is complete before running software.
$ ctest_gun -t tb_sram_scan --bm=<ilm or sramxip>
$ make run TESTNAME=tb_sram_scan_<ilm or sramxip>
When it shows PASS, it is all OK. When it shows FAIL, user should check more details.
Note
It should not run any other case when do sram scan task
4.5 Connect JTAG GDB
Use the following steps to emulate the JTAG debug connection, using openocd in the emulation environment to confirm
the JTAG debug debug interface connection.
Step 1:Compile c test programs.
$ cd xunlian_rls_pkg/xunlian
# ilm Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=ilm
# sramxip Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=sramxip
# flash Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=flash
(continues on next page)
4.4. Run SRAM Scan 10
Nuclei® Xunlian Subsystem Quick-start Manual
(continued from previous page)
# flashxip Compile c test steps:
$ ctest_gun -t USART_usart0_printf --bm=flashxip
Step 2:Compile the RTL, Run the compiled C/C++ program, using the following command:
$ cd xunlian_rls_pkg/xunlian
# Go to the xunlian_rls_pkg/xunlian directory
# ilm Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_ilm JTAGVPI=1
# sramxip Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_sramxip JTAGVPI=1
# flash Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_flash<0/1/2> JTAGVPI=1
# flashxip Start procedure steps:
$ make run TESTNAME=USART_usart0_printf_flashxip<0/1/2> JTAGVPI=1
# ilm Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_ilm JTAGVPI=1
# sramxip Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_sramxip JTAGVPI=1
# flash Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_flash<0/1/2> JTAGVPI=1
# flashxip Restart procedure steps:
$ make ncrun TESTNAME=USART_usart0_printf_flashxip<0/1/2> JTAGVPI=1
# JTAGVPI=1 is using JTAG debugging and waiting for JTAG connection.
The simulation is correctly waiting for the JTAG connection results as shown in Waiting for JTAG connection (page 11) .
Fig. 4.2: Waiting for JTAG connection
Step 3:Start openocd emulation JTAG connection
Download from Nuclei Toolchain <https://www.nucleisys.com/download.php>
$ tar zxvf nuclei-openocd-2022.04-linux-x64.tgz
$ cd xunlian_rls_pkg/xunlian/nuclei-sdk/SoC/ns/Board/fpga_eval
$ ${your_path}/Nuclei/openocd/2022.04/bin/openocd -f openocd_ns_sim.cfg
Note
This step needs to be executed immediately after the second make run step to avoid the openocd connection timeout.
Using openocd simulation JTAG connection correct results as shown in JTAG connection information (page 12) .
4.5. Connect JTAG GDB 11
Nuclei® Xunlian Subsystem Quick-start Manual
Fig. 4.3: JTAG connection information
After successful connection, the JTAG VPI connection can be seen in the waiting connection simulation interface:
Fig. 4.4: JTAG connection successful
Step 4:Start GDB emulation debugging:
$ cd soc_test_cases/ns/USART/usart0/printf
# Go to the corresponding test directory, here is an example of USART_usart0_printf.
$ make run_gdb
# Perform GDB debugging.
GDB starts as shown in GDB Start (page 12) . GDB debugging results (take USART_usart0_printf as an example) are as
GDB debugging helloworld program (page 13) .
Fig. 4.5: GDB Start
4.5. Connect JTAG GDB 12
Nuclei® Xunlian Subsystem Quick-start Manual
Fig. 4.6: GDB debugging helloworld program
4.5.1 Integrate JTAGVPI into your env
If you want to integrate the JTAGVPI into your env to test the jtag connect function, you need belows files.
• jtag_vpi.c
• jtag_vpi.tab
• jtag_vpi.v
Note
these files are in testbench/jtag_vpi dir in your soc package.
Step 1:add below args into your compile arg.
-P jtag_vpi.tab -CC "-DVCS_VPI" jtag_vpi.c jtag_vpi.v
Step 2:instance the jtagvpi module in your testbench, and connect the jtag signal.
bit jtag_enable = 1'b0;
JTAGVPI u_jtagvpi(
.jtag_TMS (jtag_tms),
.jtag_TCK (jtag_tck),
.jtag_TDI (jtag_tdi),
.jtag_TDO (jtag_tdo),
.jtag_TRST (1'b0), // unused
.enable (1'b1),
.init_done (jtag_enable)
);
initial begin
// use the simulation arg to control the enable for jtag vpi test
if($test$plusargs("JTAG_VPI"))begin
jtag_enable = 1'b1;
end
end
Step 3:add below args into your simulation arg.
+JTAG_VPI +jtag_port=6666
• +JTAG_VPI : enable the jtag vpi test
• +jtag_port: specify the jtag use port
4.5. Connect JTAG GDB 13
5
FPGA Validation
Please refer to the FPGA hardware manual for details of the RISC-V customized dedicated JTAG debugger and dedicated
FPGA development board.
Note
Users should pay attention to the size of the SRAM, if you need to increase or decrease your own SRAM size,
you can adjust according to the RAM depth. Nuclei in order to adapt to its own spontaneous version, maybe
reduce the RAM depth.
If the program is running in SRAM, you need to change the size of the link script SRAM to the corresponding size.
$ vi nuclei-sdk/SoC/ns/Board/fpga_eval/Source/GCC/gcc_ns_sram.ld
# sram (rxa!w) : ORIGIN = 0x1c000000, LENGTH = 256K
5.1 Generate FPGA Version
The FPGA version can be generated using the following steps.
14
Nuclei® Xunlian Subsystem Quick-start Manual
# To generate the FPGA version, use the following command:
$ cd xunlian_rls_pkg/xunlian/fpga/
# Go to the xunlian_rls_pkg/xunlian/fpga directory
$ module load fpga/vitis-2020.1
# First, the user should install vivado and click set to environment variable during
installation. (For example, load vivado command: module load FPGA / vitis-2020.1)
If it has been installed, this sentence can be ignored.
$ make mcs
# Compile RTL:if expand("%") == ""|browse confirm w|else|confirm w|endif
5.2 Run C Test Use Case
The test cases for chip FPGA verification are programs written in C/C++ language and can be performed using the following
steps. It needs to be carried out with the help of nuclei-sdk. Take the sample program (helloworld) as an example, you can
use the following steps to proceed.
# Step 1:To compile the helloworld sample program, use the following command:
$ cd soc_test_cases/ns/USART/usart0/printf
# The ctestcase path of the test
$ make clean
$ make dasm BOARD=fpga_eval DOWNLOAD=ilm
# DOWNLOAD=ilm Download as ilm
Note
If option DOWNLOAD=ilm is specified here, the program will be compiled by “downloading the program to ILM for
execution”.
# Step 2:Download the compiled C test program into the FPGA prototype development board
# using the following command:
$ make upload BOARD=fpga_eval DOWNLOAD=ilm
# Step 3:Running the helloworld program on the FPGA prototype development board:
# As the sample program will need to print the structure to the host PC's
# display via USART. Refer to the method described in Section Error: Reference source not
# found to set up the serial port to display the computer screen so that the program's
# print information can be displayed on the computer screen.
Note
MISA varies depending on the customer’s choice of RISC-V core.
5.2. Run C Test Use Case 15
Nuclei® Xunlian Subsystem Quick-start Manual
Fig. 5.1: Helloworld sample program C test run results
5.3 USART Functional Testing
If your Nuclei SoC Subsystem has USART, please refer to the following instructions:
5.3.1 Introduction
The test program is used to verify the serial communication functions and interrupts, and its functions are briefly described
as follows:
• Configure USART0 baud rate to 115200, data bit to 8bit, stop bit to 1bit, disable hardware flow control.
• Enables global interrupts and USART0 transmit and receive interrupts.
• The USART sends a specific string through the middle and the upper bit receives it through the serial debug assistant.
Wait for the serial debug assistant of the upper unit to send the string to the USART.
• The upper unit sends characters to the USART (the number of characters is greater than 32) and outputs the test
result “successfully” through the printf function.
5.3.2 Running results
The results of the test run on the chip FPGA verification board are shown in USART operation results (page 16) : Serial
debug assistant sends data to FPGA verification board (number of characters > 32).
Fig. 5.2: USART operation results
5.3. USART Functional Testing 16
Nuclei® Xunlian Subsystem Quick-start Manual
5.4 SPI Functional Testing
If your Nuclei SoC Subsystem has SPI, please refer to the following instructions:
5.4.1 Introduction
The program can be downloaded in flashxip mode to verify the communication function of SPI, and its function is briefly
described as follows:
• Set the DOWNLOAD parameter to flashxip. SPI0 and SPI1 send and receive 10 bytes of data to and from each
other. Finally, verify that the data sent and received are the same, as reflected by the LED2 and LED3.
5.4.2 Running results
Download the test program to run on the FPGA verification board and observe LED2 and LED3. LED2 and LED3
are lit, which means that SPI0 and SPI1 send and receive data consistently and the communication function is nor mal.(LED2:Green Light;LED3:Blue Light)
Fig. 5.3: Test results
5.4.3 Read and Write Flash Test
• SPI is initially in Master mode
• Read flash ID and print
• Erase 3byte address mode Flash
• Write 3byte address mode Flash
• Read 3byte address mode Flash
• Calibration data
5.4. SPI Functional Testing 17
Nuclei® Xunlian Subsystem Quick-start Manual
Fig. 5.4: Read and write flash test results
5.5 TIMER Functional Testing
If your Nuclei SoC Subsystem has TIMER, please refer to the following instructions:
5.5.1 Test Program Introduction
TIMER_timebase test program is used to verify the Timer timing function, which is briefly described as follows:
• Configure Timer1 divider, count value, count mode and auto reload, and channel CH0 set to output compare mode.
• Enable CH0 interrupt, when the count value is equal to the threshold, generate an interrupt and print in the interrupt
Headler.
• TIMER0 periodically generates interrupts and the serial port also performs periodic printing.
5.5.2 Running Results
Download the test program to run on the chip FPGA verification board and observe the serial port printing. The serial port
prints periodically and Timer0 timing function is normal.
5.6 TIMER_pwmout Functional Testing
If your Nuclei SoC Subsystem has TIMER_pwmout, please refer to the following instructions:
5.6.1 Test Program Introduction
• Configure Timer divider, count value, count mode and auto reload. Channels CH1, CH2 and CH3 are set to PWM
mode.
• Set the duty cycle of CH1 to 25%, CH2 to 50%, and CH3 to 75%.
5.5. TIMER Functional Testing 18
Nuclei® Xunlian Subsystem Quick-start Manual
5.6.2 Running Results
Download the test program to run on the chip FPGA verification board. The output results are shown in TIMER pwmout
test results (page 19) , and the waveform results are normal.
Fig. 5.5: TIMER pwmout test results
5.7 I2C Eeprom-AT24C02 Testing
If your Nuclei SoC Subsystem has I2C Eeprom-AT24C02, please refer to the following instructions:
5.7.1 Test Program Introduction
The test program uses I2C to read and write Eeprom(AT24C02). The functions are briefly described as follows:
• Set I2C0 to Master mode
• Enable I2C0 transmit and receive, configure Eeprom’s Slave address
• Initialize data, write Eeprom
• Read back data for verification
• Output results
Fig. 5.6: I2C Eeprom-AT24C02I2C test results
5.8 SDIO Functional Testing
If your Nuclei SoC Subsystem has SDIO, please refer to the following instructions:
5.7. I2C Eeprom-AT24C02 Testing 19
Nuclei® Xunlian Subsystem Quick-start Manual
5.8.1 Introduction
The test program is used to verify the SDIO function of reading and writing SD cards, and its function is briefly described
as follows:
• Send cmd through SDIO to identify the type of SD card
• Get the CID, CSD, etc. of the SD card and parse it out
• Read and write 512 bytes (one block size) of data
• Compare the written and read data, if the data is the same, then print successfully and display the SD card information,
otherwise print fail
5.8.2 Running Results
Firstly, insert the SD card into the card slot. Download the test program. The result of running the test program on the chip
FPGA verification board. Observe the serial debug assistant printing, which indicates that the test read/write is normal
and the card information is correctly parsed.
Fig. 5.7: SDIO running results
5.9 Ethernet Functional Testing
If your Nuclei SoC Subsystem has Ethernet, please refer to the following instructions:
5.9.1 Introduction
The test program is used to verify the RGMII communication function of the Ethernet interface, and its functions are
briefly described as follows:
• Ported lwip protocol line, the program set the development board IP ADDR: 192.168.200.10 ,UDP Port:8089
• After using the network debug assistant to bind the UDP port number, send any data to the development board, and
the board will ECHO back the same data.
5.9.2 Running Results
First of all, the IP address of the upper unit is set up, and the IP address of the development board is in the same network
segment, and the ETH1 network port of the development board is connected to the upper unit through the network cable.
After downloading the program, the development board will first send the “Nuclei XEC UDP demo send data” string to
the PC, which means the PC and the development board network connection is normal. Then the PC will send any data to
the development board, and the development board will ECHO back the same data.
5.9. Ethernet Functional Testing 20
Nuclei® Xunlian Subsystem Quick-start Manual
Fig. 5.8: Ethernet running results
5.10 USB Functional Testing
If your Nuclei SoC Subsystem has USB, please refer to the following instructions:
5.10.1 Introduction
The test program is used to verify the communication function of the USB interface, and its functions are briefly described
as follows:
• Use ULPI interface to connect to external USB phy, development board connects to PC via type-c interface.
• Enumeration experiments using the interface descriptors of Audio devices.
5.10.2 Running Results
Download the driver for USB to the development board. Then plug the development board USB port to PC. The PC’s
device manager will refresh an USB Audio Device.
Fig. 5.9: USB running results
5.10. USB Functional Testing 21
Nuclei® Xunlian Subsystem Quick-start Manual
5.11 CAN Functional Testing
If your Nuclei SoC Subsystem has CAN, please refer to the following instructions:
5.11.1 Introduction
The test program is used to verify function CAN loopback. Its function is briefly described as follows:
• Configure the CAN to loopback mode, use the CAN to send several bytes of data, and read the received data.
Compare sending and receiving data.
• Enables the CAN receive interrupt. Use CAN to send two bytes of data. And read the received data. Consistently
print success.
5.11.2 Running Results
Fig. 5.10: CAN running results
5.11. CAN Functional Testing 22
6
DC Synthesis
This SoC package(xunlian_*.tar.gz) provides comprehensive Flow-related makefile, tcl scripts and constraint files.
6.1 Synthesis Steps
After configuring the relevant parameters and lib path in syn/Makefile, you can run DC synthesis with the following
command.
$ cd xunlian_rls_pkg/xunlian/syn/
$ make dc
Note
The above files need to be modified according to the user’s EDA tool configuration.
23
7
Caution
Please pay attention to the following notes.
Note
Users should pay attention to the base addr(0x1c000000, no device region) of test_sram, if you need to use the test_fab
slv port, you can not use the address range(0x1c000000~0x1c0fffff)for your device.
24
8
Appendix
• Nuclei RISC-V IP Products: https://www.nucleisys.com/product.php
• Nuclei Spec Documentation: https://nucleisys.com/download.php#spec
• Nuclei RISCV Tools and Documents: https://nucleisys.com/download.php
• Nuclei Prebuilt Toolchain and IDE: https://nucleisys.com/download.php#tools
• NMSIS: https://github.com/Nuclei-Software/NMSIS
• Nuclei SDK: https://github.com/Nuclei-Software/nuclei-sdk
• Nuclei Linux SDK: https://github.com/Nuclei-Software/nuclei-linux-sdk
• Nuclei Software Organization in Github: https://github.com/Nuclei-Software/
• RISC-V MCU Organization in Github: https://github.com/riscv-mcu/
• RISC-V MCU Community Website: https://www.rvmcu.com/
• Nuclei riscv-openocd: https://github.com/riscv-mcu/riscv-openocd
• Nuclei riscv-gnu-toolchain: https://github.com/riscv-mcu/riscv-gnu-toolchain
25
