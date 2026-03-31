##
#### SoC Build Settings Notice
# The following variables need be defined according to your SoC and Board
# BOARD: MUST, set this variable to one of the valid folder basename under $(NUCLEI_SDK_SOC)/Board/
# CORE: MUST, set this variable to one of SUPPORTED_CORES defined in $(NUCLEI_SDK_BUILD)/Makefile.core
# VARIANT: OPTIONAL, set this variable to consecutive characters better less than 8 chars, such as lite, v1, v2.
#          It can be used to define different variant of similar board
# NUCLEI_SDK_SOC_BOARD: MUST, set this variable to $(NUCLEI_SDK_SOC)/Board/$(BOARD), this means the
#                       base folder for selected BOARD
# NUCLEI_SDK_SOC_COMMON: MUST, set this variable to $(NUCLEI_SDK_SOC)/Common, this means the base
#                       folder for selected SOC
# OPENOCD_CFG: MUST, set this variable to openocd configuration file used for the Board or SoC
# LINKER_SCRIPT: MUST, set this variable to linker script file for selected board and download mode
# RISCV_ARCH: MUST, set this variable to the riscv arch you are using, see $(NUCLEI_SDK_BUILD)/Makefile.core
# RISCV_ABI: MUST, set this variable to the riscv abi you are using, see $(NUCLEI_SDK_BUILD)/Makefile.core
# RISCV_TUNE: Optional, set this variable to the mtune you are using, see $(NUCLEI_SDK_BUILD)/Makefile.core
# If your SoC used a fixed BOARD, CORE or DOWNLOAD mode, please use override to define a fixed variable value, eg.
# override CORE := n205
# override DOWNLOAD := flashxip
# override BOARD := nuclei_fpga_eval
#### Source Code Management Notice
# you need to declare the following items:
# 1. C/ASM/CPP source code folders which need to be compiled
# 2. Some C/ASM/CPP source code files whihc need to be compiled
# You can use these makefile variables defined in $(NUCLEI_SDK_BUILD)/Makefile.files to achieve this, for details,
# please check https:
##

##### Put your SoC build configurations below #####

BOARD ?= fpga_eval

include $(NUCLEI_SDK_SOC)/ns_core0_feature.mk
# available core list can be found in $(NUCLEI_SDK_BUILD)/Makefile.core
override CORE := core0_n300f
# SMP must be a number, and above 1
# it will define c macro SMP_CPU_CNT to be SMP value
# and define a ld symbol __SMP_CPU_CNT to be used by linker script
SMP ?= 
BOOT_HARTID ?= 0
# HARTID_OFS is used to set hartid offset to the cpu hartid to get current hart index
# for example, cpu hartid maybe 1, but hart index is 0, so the offset is 1
# This is not neccessary for most cases, only useful in cases such as a multiple cpu
# system, cpu0 has 1 hart, hartid is 0, cpu1 has 1 hart, hartid is 1, but cpu1
# hart index is 0, so in this case set this value to 1
HARTID_OFS ?=
# JTAGSN must be a jtag serial number
# If not specified, it will not bind serial number
JTAGSN ?=

# Per-Core HEAP and STACK Size Settings
HEAPSZ ?= 2K
STACKSZ ?= 2K

NUCLEI_SDK_SOC_BOARD  := $(NUCLEI_SDK_SOC)/Board/$(BOARD)
NUCLEI_SDK_SOC_COMMON := $(NUCLEI_SDK_SOC)/Common

OPENOCD_CFG   ?= $(NUCLEI_SDK_SOC_BOARD)/openocd_ns_core0.cfg
LINKER_SCRIPT ?= $(NUCLEI_SDK_SOC_BOARD)/Source/GCC/gcc_ns_core0_$(DOWNLOAD).ld

# File existence check for OPENOCD_CFG and LINKER_SCRIPT
ifeq ($(wildcard $(OPENOCD_CFG)),)
$(error The openocd configuration file $(OPENOCD_CFG) for $(SOC) doesn't exist, please check!)
endif
# Allow non-existance of LINKER_SCRIPT, it might be generated
ifeq ($(wildcard $(LINKER_SCRIPT)),)
$(warning The link script file $(LINKER_SCRIPT) for $(SOC) doesn't exist, please check!)
endif

ifeq ($(SIMULATION),1)
COMMON_FLAGS += -gdwarf-4 -gstrict-dwarf
endif

ifneq ($(SMP),)
$(call assert,$(call gt,$(SMP),1),SMP must be a integer number >= 2)
QEMU_OPT += -smp $(SMP)
COMMON_FLAGS += -DSMP_CPU_CNT=$(SMP)
LDFLAGS += -Wl,--defsym=__SMP_CPU_CNT=$(SMP)
endif

# Set RISCV_ARCH and RISCV_ABI
CORE_UPPER = $(call uc, $(CORE))

CORE0_N300F_CORE_ARCH_ABI = rv32imafc ilp32f nuclei-300-series

CORE_ARCH_ABI := $($(CORE_UPPER)_CORE_ARCH_ABI)
ARCH_EXT ?= 
# Handle Nuclei RISC-V ARCH/ABI/CMODEL/TUNE
## ARCH_EXT could be combination of in order of bkpv, legal combination is list as below:
## bp: Bitmanip and Packed SIMD Extension present
## bpv: Bitmanip, Packed SIMD and Vector Extension present
## bkpv: Bitmanip, Packed SIMD, Scalar Cryptography and Vector Extension present
RISCV_ARCH ?= $(word 1, $(CORE_ARCH_ABI))$(ARCH_EXT)
RISCV_ABI ?= $(word 2, $(CORE_ARCH_ABI))
RISCV_TUNE ?= $(word 3, $(CORE_ARCH_ABI))

COMMON_FLAGS += -mtune=nuclei-300-series
# Handle QEMU Emulation
ifneq ($(findstring u,$(CORE)),)
QEMU_MACHINE := nuclei_u,download=$(DOWNLOAD)
else
QEMU_MACHINE := nuclei_n,download=$(DOWNLOAD)
endif
QEMU_CPU := nuclei-$(CORE),ext=$(ARCH_EXT)

##### Put your Source code Management configurations below #####

INCDIRS += $(NUCLEI_SDK_SOC_COMMON)/Include

C_SRCDIRS += $(NUCLEI_SDK_SOC_COMMON)/Source $(NUCLEI_SDK_SOC_COMMON)/Source/Drivers

# If semihosting is enabled, no stub function is needed
ifeq ($(SEMIHOST),)
ifneq ($(findstring newlib,$(STDCLIB)),)
C_SRCDIRS += $(NUCLEI_SDK_SOC_COMMON)/Source/Stubs/newlib
else
# no stubs will be used
endif
endif

ifneq ($(findstring libncrt,$(STDCLIB)),)
# semihosting currently not ported to support libncrt
C_SRCDIRS += $(NUCLEI_SDK_SOC_COMMON)/Source/Stubs/libncrt
endif

ASM_SRCS += $(NUCLEI_SDK_SOC_COMMON)/Source/GCC/startup_ns_core0.S \
	        $(NUCLEI_SDK_SOC_COMMON)/Source/GCC/intexc_ns_core0.S \
			$(NUCLEI_SDK_SOC_COMMON)/Source/GCC/intexc_ns_core0_s.S

# Add extra board related source files and header files
VALID_NUCLEI_SDK_SOC_BOARD := $(wildcard $(NUCLEI_SDK_SOC_BOARD))
ifneq ($(VALID_NUCLEI_SDK_SOC_BOARD),)
INCDIRS   += $(VALID_NUCLEI_SDK_SOC_BOARD)/Include
C_SRCDIRS += $(VALID_NUCLEI_SDK_SOC_BOARD)/Source
endif
