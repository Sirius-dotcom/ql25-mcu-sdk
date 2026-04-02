# platform/osal build integration
# 在应用 Makefile 中 include 本文件即可引入 OSAL 层

PLATFORM_OSAL_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_SRCDIRS += $(PLATFORM_OSAL_DIR)src
INCDIRS   += $(PLATFORM_OSAL_DIR)include
