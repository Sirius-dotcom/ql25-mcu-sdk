C_SRCDIRS += $(NUCLEI_SDK_RTOS)/Source $(NUCLEI_SDK_RTOS)/Source/portable/GCC
# heap management selection, choose 1 from the portable/MemMang/heap_*.c
FREERTOS_HEAP ?= 4
ifneq ($(FREERTOS_HEAP),none)
C_SRCS += $(NUCLEI_SDK_RTOS)/Source/portable/MemMang/heap_$(FREERTOS_HEAP).c
endif
C_SRCS += $(NUCLEI_SDK_RTOS)/Source/portable/port.c

ASM_SRCDIRS += $(NUCLEI_SDK_RTOS)/Source/portable/GCC

INCDIRS += $(NUCLEI_SDK_RTOS)/Source/include \
		$(NUCLEI_SDK_RTOS)/Source/portable
