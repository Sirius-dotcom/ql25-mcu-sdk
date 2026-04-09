# QL25 AMP 主核应用（管理面）
#
# 构建命令：
#   make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=ilm all        # FPGA bringup
#   make SOC=ns_core1 BOARD=fpga_eval DOWNLOAD=flashxip0 all  # 目标模式
#
# 主核无严格 bin 大小约束，使用动态分配。

TARGET = ql25_amp_master
RTOS = FreeRTOS

NUCLEI_SDK_ROOT = ../../..

SRCDIRS = .
INCDIRS = .

COMMON_FLAGS := -O2

include $(NUCLEI_SDK_ROOT)/platform/osal/build.mk
include $(NUCLEI_SDK_ROOT)/platform/ipc/build.mk

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
