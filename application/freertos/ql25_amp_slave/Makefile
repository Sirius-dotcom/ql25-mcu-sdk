# QL25 AMP 从核应用（数据面）
#
# 构建命令：
#   make SOC=ns_core0 BOARD=fpga_eval DOWNLOAD=ilm all
#
# 约束：
#   - 从核整个 bin 加载到 ILM，总体 <= 48KB
#   - 静态分配，HEAPSZ=0
#   - DOWNLOAD 必须为 ilm

TARGET = ql25_amp_slave
RTOS = FreeRTOS

NUCLEI_SDK_ROOT = ../../..

# 从核 48KB ILM 约束：禁用 C 运行时堆
HEAPSZ = 0
STACKSZ = 1K

SRCDIRS = .
INCDIRS = .

COMMON_FLAGS := -Os

# 从核不使用 OSAL（OSAL 内部使用动态分配，与 configSUPPORT_DYNAMIC_ALLOCATION=0 冲突）
# 从核直接调用 FreeRTOS 静态 API + IPC API
include $(NUCLEI_SDK_ROOT)/platform/ipc/build.mk

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
