# platform/utb_osal build integration

PLATFORM_UTB_OSAL_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

include $(PLATFORM_UTB_OSAL_DIR)../osal/build.mk

C_SRCDIRS += $(PLATFORM_UTB_OSAL_DIR)src
INCDIRS   += $(PLATFORM_UTB_OSAL_DIR)include
INCDIRS   += $(PLATFORM_UTB_OSAL_DIR)../utb_port/include
