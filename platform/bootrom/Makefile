# QL25 BootROM 构建
#
# 独立构建，不依赖 SDK 构建系统。
# BootROM 是自包含的，只依赖 RISC-V 工具链。
#
# 用法：
#   make                    # 构建 bootrom.elf + bootrom.bin
#   make size               # 查看 ROM 大小
#   make dump               # 反汇编
#   make clean
#
# TODO: CROSS_COMPILE 根据实际工具链路径修改

CROSS_COMPILE ?= riscv-nuclei-elf-

CC      = $(CROSS_COMPILE)gcc
AS      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE    = $(CROSS_COMPILE)size

# RISC-V 架构：rv32imafc（单精度 FPU）
ARCH    = rv32imafc
ABI     = ilp32f

TARGET  = bootrom

# 源文件
ASM_SRCS = src/bootrom_entry.S
C_SRCS   = src/bootrom_main.c

# 头文件路径
INCLUDES = -Iinclude

# riscv_encoding.h 在 NMSIS 中，需要引用
NMSIS_INCLUDE = -I../../NMSIS/Core/Include

# 编译选项：最小化体积
CFLAGS  = -march=$(ARCH) -mabi=$(ABI) \
          -Os -ffunction-sections -fdata-sections \
          -fno-builtin -fno-common \
          -Wall -Wextra -Werror \
          $(INCLUDES) $(NMSIS_INCLUDE) \
          -DBOOTROM_BUILD

ASFLAGS = -march=$(ARCH) -mabi=$(ABI) \
          $(NMSIS_INCLUDE) \
          -DBOOTROM_BUILD

LDFLAGS = -march=$(ARCH) -mabi=$(ABI) \
          -T bootrom.ld \
          -nostdlib -nostartfiles \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map

OBJS    = $(ASM_SRCS:.S=.o) $(C_SRCS:.c=.o)

.PHONY: all clean size dump

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	@echo "BootROM built: $@"

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	@echo "BootROM binary: $@ ($$(wc -c < $@) bytes / 16384 max)"

%.o: %.S
	$(AS) $(ASFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

size: $(TARGET).elf
	$(SIZE) $<

dump: $(TARGET).elf
	$(OBJDUMP) -d -S $< > $(TARGET).dis
	@echo "Disassembly: $(TARGET).dis"

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).map $(TARGET).dis
