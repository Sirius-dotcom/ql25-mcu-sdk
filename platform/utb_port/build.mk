# platform/utb_port build integration

PLATFORM_UTB_PORT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

C_SRCDIRS += $(PLATFORM_UTB_PORT_DIR)src
INCDIRS   += $(PLATFORM_UTB_PORT_DIR)include
