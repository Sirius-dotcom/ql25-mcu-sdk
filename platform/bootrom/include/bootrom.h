/**
 * @file bootrom.h
 * @brief QL25 BootROM 定义
 *
 * BootROM 固化在主核（Core 1）16KB ROM 中，上电后第一段执行的代码。
 *
 * 硬件前提：
 *   - Core 1（主核）无 ILM/DLM，只有 16KB ROM
 *   - Core 0（从核）有 ILM 48KB + DLM 32KB，上电后被 hold 在复位
 *   - 片上 SRAM 4 banks 共 384KB，上电即可用
 *   - Flash XIP 基址 0x20000000
 */

#ifndef BOOTROM_H
#define BOOTROM_H

#include <stdint.h>

/* ========================================================================
 * 地址定义（需根据 SoC 实际内存映射调整）
 * ======================================================================== */

/* BootROM 自身地址（SoC 硬件定义，主核复位向量指向此处） */
#define BOOTROM_BASE            0x00000000UL    /* TODO: 根据 SoC 实际地址修改 */
#define BOOTROM_SIZE            (16 * 1024)

/* 片上 SRAM（上电即可用） */
#define SRAM0_BASE              0x30000000UL    /* 128KB */
#define SRAM1_BASE              0x30020000UL    /* 128KB */
#define SRAM2_BASE              0x30040000UL    /* 64KB  */
#define SRAM3_BASE              0x30060000UL    /* 64KB  */
#define SRAM3_SIZE              (64 * 1024)

/* BootROM 临时栈：SRAM3 顶部（向下生长，BootROM 用几百字节） */
#define BOOTROM_STACK_TOP       (SRAM3_BASE + SRAM3_SIZE)   /* 0x30070000 */

/* 从核 ILM（Core 0），主核通过总线 Slave Port 写入 */
#define CORE0_ILM_BASE          0x08000000UL
#define CORE0_ILM_SIZE          (48 * 1024)     /* 实际可用 48KB */

/* Flash XIP 基址 */
#define FLASH_XIP_BASE          0x20000000UL

/* ========================================================================
 * 外设基址（最小集，BootROM 只用这几个）
 * ======================================================================== */

#define QSPI_XIP0_BASE          0x10060000UL    /* QSPI XIP0 控制器（ns.h: QSPI_XIP0_BASE） */
#define USART0_BASE             0x10030000UL    /* USART0（ns.h: USART0_BASE） */
#define MISC_BASE               0x12000000UL    /* MISC 控制寄存器（ns.h: MISC_CTL_BASE） */

/* MISC 寄存器偏移（核释放控制） */
#define MISC_CTRL5_OFS          0xC94UL         /* MISC_CTRL5（ns_misc.h: MISC_CTRL5_OFS） */
#define MISC_CTRL5_CORE0_STOP   (1UL << 0)     /* BIT(0): soc_clk_core0_stop_on_reset */
#define MISC_CTRL5_CORE1_STOP   (1UL << 1)     /* BIT(1): soc_clk_core1_stop_on_reset */

/* ========================================================================
 * USART 寄存器偏移（最小集）
 * ======================================================================== */

#define USART_TXDATA_OFS        0x00
#define USART_RXDATA_OFS        0x04
#define USART_TXCTRL_OFS        0x08
#define USART_RXCTRL_OFS        0x0C
#define USART_IE_OFS            0x10
#define USART_IP_OFS            0x14
#define USART_DIV_OFS           0x18
#define USART_TXDATA_FULL       (1UL << 31)
#define USART_RXDATA_EMPTY      (1UL << 31)

/* ========================================================================
 * 镜像头格式（64 字节，兼容 A/B 双分区）
 * ======================================================================== */

#define IMAGE_MAGIC             0x514C3235UL    /* "QL25" ASCII */
#define IMAGE_HDR_VERSION       1
#define IMAGE_HDR_SIZE          64

/* boot_mode 字段 */
#define BOOT_MODE_ILM           0               /* 搬到目标地址执行 */
#define BOOT_MODE_XIP           1               /* Flash XIP 直接执行 */

/* slot_id 字段 */
#define SLOT_A                  0
#define SLOT_B                  1

typedef struct {
    uint32_t magic;             /* 0x00: IMAGE_MAGIC */
    uint32_t hdr_version;       /* 0x04: 头格式版本 */
    uint32_t image_size;        /* 0x08: payload 大小（不含头） */
    uint32_t entry_point;       /* 0x0C: 入口地址（_start） */
    uint32_t load_addr;         /* 0x10: 加载目标地址 */
    uint32_t image_crc32;       /* 0x14: payload CRC32 */
    uint32_t boot_mode;         /* 0x18: BOOT_MODE_ILM / BOOT_MODE_XIP */
    uint32_t slot_id;           /* 0x1C: SLOT_A / SLOT_B */
    uint32_t version;           /* 0x20: 固件版本号 */
    uint32_t hdr_crc32;         /* 0x24: 头部 CRC32（计算时本字段为 0）*/
    uint8_t  reserved[24];      /* 0x28~0x3F: 预留扩展 */
} image_header_t;

_Static_assert(sizeof(image_header_t) == IMAGE_HDR_SIZE, "image_header_t size mismatch");

/* ========================================================================
 * Flash 分区表（支持 A/B 双镜像回滚）
 * ======================================================================== */

#define PARTITION_TABLE_MAGIC   0x5054424CUL    /* "PTBL" */
#define PARTITION_TABLE_ADDR    FLASH_XIP_BASE  /* Flash 起始 1KB 放分区表 */

/* Flash 分区布局（2MB 总量）：
 *   0x20000000 ~ 0x200003FF : 分区表（1KB）
 *   0x20000400 ~ 0x200FFFFF : Slot A（≈1MB - 1KB）
 *   0x20100000 ~ 0x201FFFFF : Slot B（1MB）
 */
#define SLOT_A_OFFSET           0x00000400UL
#define SLOT_B_OFFSET           0x00100000UL
#define SLOT_MAX_SIZE           0x000FFC00UL    /* ≈1MB - 1KB */

typedef struct {
    uint32_t magic;             /* PARTITION_TABLE_MAGIC */
    uint32_t active_slot;       /* 0 = Slot A, 1 = Slot B */
    uint32_t slot_a_offset;     /* Slot A 在 Flash 中的偏移 */
    uint32_t slot_b_offset;     /* Slot B 在 Flash 中的偏移 */
    uint32_t boot_count;        /* 启动计数（回滚判断用） */
    uint32_t rollback_limit;    /* 连续失败几次后回滚（默认 3） */
    uint8_t  reserved[8];       /* 预留 */
    uint32_t crc32;             /* 分区表自身 CRC32 */
} partition_table_t;

/* ========================================================================
 * UART 下载协议
 * ======================================================================== */

/* BootROM 发送同步标记，主机识别后开始下载 */
#define UART_SYNC_MARKER        "QL25BOOT\n"
#define UART_SYNC_MARKER_LEN    9
#define UART_ACK                'A'
#define UART_NAK                'N'
#define UART_BAUDRATE           115200

/* ========================================================================
 * BootROM Shell（交互式命令行，开发调试用）
 * ======================================================================== */

/*
 * boot pin = 00 时，BootROM 在 UART 上等待短暂窗口：
 *   - 超时无输入 → 正常 Flash 启动（量产零延迟）
 *   - 收到空格键 → 进入 BootROM Shell，支持命令：
 *       md <addr> [len]   — 内存 dump（查看寄存器/内存）
 *       mw <addr> <val>   — 内存写入（改寄存器/内存）
 *       info              — 显示 Flash 分区表和镜像状态
 *       go <addr>         — 跳转到指定地址执行
 *       boot flash|uart|jtag — 选择启动模式
 *       help              — 命令帮助
 *
 * 类似 U-Boot Shell 机制。
 * 量产固化时将 BOOTROM_MENU_TIMEOUT_CYCLES 设为 0 即可完全禁用，
 * 从而消除启动延迟（Shell 代码仍在 ROM 中但不会被执行）。
 */
#define BOOTROM_MENU_TIMEOUT_CYCLES  0x01000000UL   /* 等待窗口（约 1~2 秒 @~26MHz） */
#define BOOTROM_MENU_KEY             ' '            /* 空格键触发 Shell */

/* ========================================================================
 * 启动模式（2-bit boot pin，4 种模式）
 * ======================================================================== */

#define BOOT_MODE_FLASH         0       /* 00: Flash 正常启动（量产默认） */
#define BOOT_MODE_UART_DL       1       /* 01: UART 下载模式 */
#define BOOT_MODE_JTAG_DEBUG    2       /* 10: JTAG 调试模式（停住等调试器） */
#define BOOT_MODE_RESERVED      3       /* 11: 预留（芯片测试等） */

/*
 * Boot Pin 硬件定义
 *
 * 使用 2 个 GPIO pin，上电复位时采样电平：
 *   BOOT_PIN0 = 低位 (bit 0)
 *   BOOT_PIN1 = 高位 (bit 1)
 *   boot_mode = (BOOT_PIN1 << 1) | BOOT_PIN0
 *
 * 硬件建议：
 *   - 两个 pin 默认内部下拉（默认 00 = Flash 启动）
 *   - 外部通过拨码开关或跳线帽上拉选择其他模式
 *   - 采样窗口在 BootROM 读取时，之后 pin 可复用为普通 GPIO
 *
 * TODO: 根据硬件原理图设置实际 GPIO 基址和 pin 编号
 */
#define BOOT_PIN_GPIO_BASE      0x10050000UL    /* TODO: LGPIO0 基址，需确认 */
#define BOOT_PIN0_BIT           0               /* TODO: BOOT_PIN0 对应 GPIO bit */
#define BOOT_PIN1_BIT           1               /* TODO: BOOT_PIN1 对应 GPIO bit */

/* LGPIO 寄存器偏移（最小集，只用输入读取） */
#define LGPIO_DATAIN_OFS        0x00            /* TODO: 输入数据寄存器偏移，需确认 */

/* ========================================================================
 * 错误码
 * ======================================================================== */

#define BOOTROM_OK              0
#define BOOTROM_ERR_NO_IMAGE    1       /* Flash 中无有效镜像 */
#define BOOTROM_ERR_BAD_MAGIC   2       /* 镜像 magic 错误 */
#define BOOTROM_ERR_BAD_CRC     3       /* CRC 校验失败 */
#define BOOTROM_ERR_TOO_LARGE   4       /* 镜像超出目标区域大小 */
#define BOOTROM_ERR_BAD_ENTRY   5       /* 入口地址不合法 */
#define BOOTROM_ERR_UART_TIMEOUT 6      /* UART 下载超时 */
#define BOOTROM_ERR_PARTITION   7       /* 分区表损坏 */

/* ========================================================================
 * 函数声明
 * ======================================================================== */

/* bootrom_main.c */
void bootrom_main(void);

/* 内部函数 */
uint32_t bootrom_crc32(const void *data, uint32_t len);
int  bootrom_detect_boot_mode(void);
int  bootrom_uart_boot_menu(void);
int  bootrom_boot_from_flash(void);
int  bootrom_boot_from_uart(void);
void bootrom_enter_jtag_mode(void);
void bootrom_jump_to_app(uint32_t entry_point);
void bootrom_fatal(uint32_t error_code);

/* 硬件操作 */
void bootrom_uart_init(uint32_t baudrate);
void bootrom_uart_putc(char c);
void bootrom_uart_puts(const char *s);
int  bootrom_uart_getc(uint32_t timeout_cycles);
void bootrom_uart_send(const uint8_t *data, uint32_t len);
int  bootrom_uart_recv(uint8_t *buf, uint32_t len, uint32_t timeout_cycles);

void bootrom_hold_core0_reset(void);

#endif /* BOOTROM_H */
