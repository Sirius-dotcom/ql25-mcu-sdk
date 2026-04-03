# QL25 IPC 层构建集成
#
# 用法：在应用 Makefile 中 include 此文件
#   include $(NUCLEI_SDK_ROOT)/platform/ipc/build.mk

PLATFORM_IPC_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_SRCDIRS += $(PLATFORM_IPC_DIR)src
INCDIRS   += $(PLATFORM_IPC_DIR)include
