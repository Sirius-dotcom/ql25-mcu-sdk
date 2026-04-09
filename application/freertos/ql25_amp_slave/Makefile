TARGET = ql25_amp_slave
RTOS = FreeRTOS
FREERTOS_HEAP = none

NUCLEI_SDK_ROOT = ../../..

SRCDIRS = . src dataplane
INCDIRS = . src dataplane

include $(NUCLEI_SDK_ROOT)/platform/ipc/build.mk
include $(NUCLEI_SDK_ROOT)/platform/utb_hal/build.mk

include $(NUCLEI_SDK_ROOT)/Build/Makefile.base
