# QL25 IPC 层构建集成
#
# 用法：在应用 Makefile 中 include 此文件
#   include $(NUCLEI_SDK_ROOT)/platform/ipc/build.mk
#
# 按 SOC 条件编译：
#   ns_core0（从核）：不编译 recovery 模块
#   ns_core1（主核）：编译全部

PLATFORM_IPC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# 公共源文件（双核共用）
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_ring.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_notify.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_init.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_event.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_snapshot.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_stats.c

# 恢复模块：仅主核编译（ns_core1）
ifneq ($(SOC),ns_core0)
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_recovery.c
endif

INCDIRS += $(PLATFORM_IPC_DIR)include
