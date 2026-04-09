# QL25 IPC build integration
#
# Usage:
#   include $(NUCLEI_SDK_ROOT)/platform/ipc/build.mk
#
# Build selection by SOC:
#   ns_core0: exclude recovery module
#   ns_core1: include all IPC sources

PLATFORM_IPC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

# Common sources shared by both cores.
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_ring.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_notify.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_init.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_event.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_heartbeat.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_snapshot.c
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_stats.c

# Recovery is built only for the master core.
ifneq ($(SOC),ns_core0)
C_SRCS += $(PLATFORM_IPC_DIR)src/utb_ipc_recovery.c
endif

INCDIRS += $(PLATFORM_IPC_DIR)include
