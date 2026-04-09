# platform/utb_hal build integration

PLATFORM_UTB_HAL_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_SRCDIRS += $(PLATFORM_UTB_HAL_DIR)src
INCDIRS   += $(PLATFORM_UTB_HAL_DIR)include
INCDIRS   += $(PLATFORM_UTB_HAL_DIR)src/hal/include
INCDIRS   += $(PLATFORM_UTB_HAL_DIR)src/ql25_sdk
INCDIRS   += $(PLATFORM_UTB_HAL_DIR)../utb_port/include
