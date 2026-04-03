/**
 * @file bootrom_main.c
 * @brief QL25 BootROM 主逻辑
 *
 * 本文件是 BootROM 的全部 C 代码。编译后与 bootrom_entry.S 链接，
 * 总大小 <= 16KB，固化到主核（Core 1）ROM。
 *
 * 注意：
 *   - 不使用可写全局变量（ROM 不可写，SRAM 中无 .bss/.data 段）
 *   - 所有状态通过局部变量（栈）或寄存器传递
 *   - 不依赖 SDK 驱动，全部寄存器级操作
 *   - 不调用 printf / malloc 等 C 库函数
 */

#include "bootrom.h"

/* ========================================================================
 * 寄存器访问宏
 * ======================================================================== */

#define REG32(addr)         (*(volatile uint32_t *)(addr))
#define REG_WRITE(base, ofs, val)   REG32((base) + (ofs)) = (val)
#define REG_READ(base, ofs)         REG32((base) + (ofs))
#define REG_SET(base, ofs, bits)    REG32((base) + (ofs)) |= (bits)
#define REG_CLR(base, ofs, bits)    REG32((base) + (ofs)) &= ~(bits)

/* 内存屏障 */
#define FENCE()     __asm volatile("fence" ::: "memory")
#define FENCE_I()   __asm volatile("fence.i" ::: "memory")

/* ========================================================================
 * CRC32（Nibble 查表法，表仅 64 字节，省 ROM 空间）
 * ======================================================================== */

static const uint32_t crc32_nibble_table[16] = {
    0x00000000, 0x1DB71064, 0x3B6E20C8, 0x26D930AC,
    0x76DC4190, 0x6B6B51F4, 0x4DB26158, 0x5005713C,
    0xEDB88320, 0xF00F9344, 0xD6D6A3E8, 0xCB61B38C,
    0x9B64C2B0, 0x86D3D2D4, 0xA00AE278, 0xBDBDF21C
};

uint32_t bootrom_crc32(const void *data, uint32_t len)
{
    const uint8_t *p = (const uint8_t *)data;
    uint32_t crc = 0xFFFFFFFF;

    for (uint32_t i = 0; i < len; i++) {
        crc = crc32_nibble_table[(crc ^ p[i]) & 0x0F] ^ (crc >> 4);
        crc = crc32_nibble_table[(crc ^ (p[i] >> 4)) & 0x0F] ^ (crc >> 4);
    }
    return crc ^ 0xFFFFFFFF;
}

/* ========================================================================
 * 硬件操作：核释放控制
 * ======================================================================== */

void bootrom_hold_core0_reset(void)
{
    /* 确保从核（Core 0）保持在复位状态 */
    REG_SET(MISC_BASE, MISC_CTRL5_OFS, MISC_CTRL5_CORE0_STOP);
}

static void bootrom_release_core0(void)
{
    /* 释放从核（Core 0） */
    REG_CLR(MISC_BASE, MISC_CTRL5_OFS, MISC_CTRL5_CORE0_STOP);
}

/* ========================================================================
 * 硬件操作：USART0（寄存器级，最小实现）
 * ======================================================================== */

/*
 * TODO: USART 寄存器布局需要根据 QL25 实际硬件确认。
 *       以下使用 SiFive 风格 UART 寄存器布局作为参考模板。
 *       如果 QL25 的 USART 寄存器不同，需要替换此处实现。
 */

void bootrom_uart_init(uint32_t baudrate)
{
    /*
     * 最小 USART0 初始化：
     * 1. 设置波特率分频
     * 2. 使能 TX/RX
     * 3. 8N1 格式
     *
     * TODO: 需要知道系统时钟频率来计算分频值
     *       div = (sys_clk / baudrate) - 1
     */
    uint32_t sys_clk = 8000000UL;   /* TODO: 实际系统时钟频率 */
    uint32_t div = (sys_clk / baudrate) - 1;

    REG_WRITE(USART0_BASE, USART_DIV_OFS, div);
    REG_WRITE(USART0_BASE, USART_TXCTRL_OFS, 0x1);    /* 使能 TX */
    REG_WRITE(USART0_BASE, USART_RXCTRL_OFS, 0x1);    /* 使能 RX */
}

void bootrom_uart_putc(char c)
{
    /* 等待 TX FIFO 有空位 */
    while (REG_READ(USART0_BASE, USART_TXDATA_OFS) & USART_TXDATA_FULL)
        ;
    REG_WRITE(USART0_BASE, USART_TXDATA_OFS, (uint32_t)c);
}

void bootrom_uart_puts(const char *s)
{
    while (*s) {
        bootrom_uart_putc(*s++);
    }
}

int bootrom_uart_getc(uint32_t timeout_cycles)
{
    uint32_t count = 0;
    while (count < timeout_cycles) {
        uint32_t val = REG_READ(USART0_BASE, USART_RXDATA_OFS);
        if (!(val & USART_RXDATA_EMPTY)) {
            return (int)(val & 0xFF);
        }
        count++;
    }
    return -1;  /* 超时 */
}

void bootrom_uart_send(const uint8_t *data, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        bootrom_uart_putc((char)data[i]);
    }
}

int bootrom_uart_recv(uint8_t *buf, uint32_t len, uint32_t timeout_cycles)
{
    for (uint32_t i = 0; i < len; i++) {
        int c = bootrom_uart_getc(timeout_cycles);
        if (c < 0) {
            return -1;  /* 超时 */
        }
        buf[i] = (uint8_t)c;
    }
    return 0;
}

/* ========================================================================
 * 硬件操作：QSPI XIP0 Flash 读取
 * ======================================================================== */

/*
 * 对于 Flash XIP 模式：
 *   如果 QSPI XIP0 硬件默认已配置好（某些 SoC 上电即可 XIP），
 *   则 Flash 内容可以直接通过地址 0x20000000 读取，无需额外初始化。
 *
 * 对于非 XIP 模式（需要手动 SPI 命令读取）：
 *   TODO: 实现 QSPI 寄存器级初始化和 SPI Flash 读命令。
 *
 * 当前实现假设 XIP 硬件可直接访问 Flash。
 * 如果不行，需要补充 bootrom_qspi_init() 和 bootrom_flash_read()。
 */

static const void *bootrom_flash_ptr(uint32_t offset)
{
    return (const void *)(FLASH_XIP_BASE + offset);
}

static void bootrom_flash_read(uint32_t offset, void *dst, uint32_t len)
{
    /*
     * XIP 模式下直接 memcpy 即可。
     * 非 XIP 模式需要通过 QSPI 命令读取，TODO 补充。
     */
    const uint8_t *src = (const uint8_t *)(FLASH_XIP_BASE + offset);
    uint8_t *d = (uint8_t *)dst;
    for (uint32_t i = 0; i < len; i++) {
        d[i] = src[i];
    }
}

/* ========================================================================
 * 镜像头验证
 * ======================================================================== */

static int bootrom_validate_header(const image_header_t *hdr)
{
    /* 检查 magic */
    if (hdr->magic != IMAGE_MAGIC) {
        return BOOTROM_ERR_BAD_MAGIC;
    }

    /* 检查头部 CRC（计算时 hdr_crc32 字段视为 0） */
    image_header_t tmp;
    const uint8_t *src = (const uint8_t *)hdr;
    uint8_t *dst = (uint8_t *)&tmp;
    for (uint32_t i = 0; i < sizeof(tmp); i++) {
        dst[i] = src[i];
    }
    tmp.hdr_crc32 = 0;
    uint32_t calc_crc = bootrom_crc32(&tmp, sizeof(tmp));
    if (calc_crc != hdr->hdr_crc32) {
        return BOOTROM_ERR_BAD_CRC;
    }

    /* 检查镜像大小合理性 */
    if (hdr->image_size == 0 || hdr->image_size > SLOT_MAX_SIZE) {
        return BOOTROM_ERR_TOO_LARGE;
    }

    /* 检查入口地址非零 */
    if (hdr->entry_point == 0) {
        return BOOTROM_ERR_BAD_ENTRY;
    }

    return BOOTROM_OK;
}

/* ========================================================================
 * 镜像加载与跳转
 * ======================================================================== */

static int bootrom_load_and_verify(uint32_t flash_offset)
{
    /* 读取镜像头 */
    const image_header_t *hdr =
        (const image_header_t *)bootrom_flash_ptr(flash_offset);

    /* 验证头部 */
    int ret = bootrom_validate_header(hdr);
    if (ret != BOOTROM_OK) {
        return ret;
    }

    /* payload 在头部之后 */
    uint32_t payload_offset = flash_offset + IMAGE_HDR_SIZE;
    const void *payload = bootrom_flash_ptr(payload_offset);

    /* 验证 payload CRC */
    uint32_t calc_crc = bootrom_crc32(payload, hdr->image_size);
    if (calc_crc != hdr->image_crc32) {
        return BOOTROM_ERR_BAD_CRC;
    }

    /* 加载镜像 */
    if (hdr->boot_mode == BOOT_MODE_ILM) {
        /* 搬运到目标地址（通常是 Core 0 ILM 或 Core 1 外部 SRAM） */
        if (hdr->image_size > CORE0_ILM_SIZE) {
            return BOOTROM_ERR_TOO_LARGE;
        }
        uint8_t *dst = (uint8_t *)hdr->load_addr;
        const uint8_t *src = (const uint8_t *)payload;
        for (uint32_t i = 0; i < hdr->image_size; i++) {
            dst[i] = src[i];
        }
        FENCE();
        FENCE_I();
    }
    /* BOOT_MODE_XIP: 不需要搬运，直接从 Flash XIP 执行 */

    /* 跳转到应用入口 */
    bootrom_jump_to_app(hdr->entry_point);

    /* 不应到达这里 */
    return BOOTROM_ERR_BAD_ENTRY;
}

/* ========================================================================
 * Boot Pin 读取
 * ======================================================================== */

static uint32_t bootrom_read_boot_pins(void)
{
    /*
     * 读取 2 个 boot pin 的电平，组合成 2-bit 模式值。
     *
     * 硬件要求：
     *   - Boot pin 上电时由外部电阻/跳线决定电平
     *   - GPIO 默认为输入模式（大多数 SoC 复位后 GPIO 默认输入）
     *   - 如果 GPIO 需要先配置为输入模式，在此处添加配置代码
     *
     * 返回值: 0~3
     *   0 = Flash 正常启动
     *   1 = UART 下载
     *   2 = JTAG 调试
     *   3 = 预留
     */
    uint32_t gpio_val = REG_READ(BOOT_PIN_GPIO_BASE, LGPIO_IVAL_OFS);

    uint32_t pin0 = (gpio_val >> BOOT_PIN0_BIT) & 0x1;
    uint32_t pin1 = (gpio_val >> BOOT_PIN1_BIT) & 0x1;

    return (pin1 << 1) | pin0;
}

/* ========================================================================
 * 启动模式检测（Boot Pin + Flash 自动 fallback）
 * ======================================================================== */

int bootrom_detect_boot_mode(void)
{
    /*
     * 三级检测机制：
     *
     * 第一级：读 boot pin（硬件强制选择）
     *   - 01 → 强制 UART 下载（不管 Flash 有没有镜像）
     *   - 10 → 强制 JTAG 调试
     *   - 11 → 预留
     *   - 00 → 进入第二级
     *
     * 第二级：UART 命令行菜单（开发调试用）
     *   - boot pin = 00 时，UART 上等 1~2 秒
     *   - 收到空格键 → 进入交互菜单，用户选择 Flash/UART/JTAG
     *   - 超时无输入 → 进入第三级
     *   （好处：开发阶段不用拨开关，串口敲空格即可选模式）
     *
     * 第三级：自动检测 Flash
     *   - Flash 有有效镜像 → Flash 正常启动
     *   - Flash 无有效镜像 → 自动 fallback 到 UART 下载
     *   （好处：空片首次上电不需要拨开关就能进 UART 下载）
     */

    uint32_t pin_mode = bootrom_read_boot_pins();

    /* 第一级：Boot pin 非 00，直接按 pin 选择模式 */
    if (pin_mode == BOOT_MODE_UART_DL) {
        return BOOT_MODE_UART_DL;
    }
    if (pin_mode == BOOT_MODE_JTAG_DEBUG) {
        return BOOT_MODE_JTAG_DEBUG;
    }
    if (pin_mode == BOOT_MODE_RESERVED) {
        return BOOT_MODE_RESERVED;
    }

    /* 第二级：UART 命令行菜单（仅 boot pin = 00 时） */
    if (BOOTROM_MENU_TIMEOUT_CYCLES > 0) {
        int menu_result = bootrom_uart_boot_menu();
        if (menu_result >= 0) {
            return menu_result;  /* 用户通过菜单选择了模式 */
        }
        /* menu_result < 0 表示超时，无人干预，继续第三级 */
    }

    /* 第三级：自动检测 Flash 有效性 */
    const image_header_t *hdr_a =
        (const image_header_t *)bootrom_flash_ptr(SLOT_A_OFFSET);
    const image_header_t *hdr_b =
        (const image_header_t *)bootrom_flash_ptr(SLOT_B_OFFSET);

    if (hdr_a->magic == IMAGE_MAGIC || hdr_b->magic == IMAGE_MAGIC) {
        return BOOT_MODE_FLASH;
    }

    /* Flash 中无有效镜像，自动 fallback 到 UART（空片友好） */
    return BOOT_MODE_UART_DL;
}

/* ========================================================================
 * Shell 工具函数：十六进制输入/输出
 * ======================================================================== */

/* 输出单个十六进制 nibble */
static void shell_put_nibble(uint32_t n)
{
    n &= 0xF;
    bootrom_uart_putc((char)(n < 10 ? '0' + n : 'a' + n - 10));
}

/* 输出 32-bit 十六进制值（固定 8 位，带 0x 前缀） */
static void shell_put_hex32(uint32_t val)
{
    bootrom_uart_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        shell_put_nibble(val >> i);
    }
}

/* 输出 8-bit 十六进制值（固定 2 位） */
static void shell_put_hex8(uint8_t val)
{
    shell_put_nibble(val >> 4);
    shell_put_nibble(val);
}

/* 输出十进制无符号整数 */
static void shell_put_dec(uint32_t val)
{
    char buf[10];
    int pos = 0;

    if (val == 0) {
        bootrom_uart_putc('0');
        return;
    }
    while (val > 0) {
        buf[pos++] = '0' + (char)(val % 10);
        val /= 10;
    }
    while (pos > 0) {
        bootrom_uart_putc(buf[--pos]);
    }
}

/*
 * 解析十六进制字符串，支持 "0x" 前缀（可选）。
 * 成功返回 0，*out 写入解析结果；失败返回 -1。
 */
static int shell_parse_hex(const char *s, uint32_t *out)
{
    uint32_t val = 0;
    int digits = 0;

    /* 跳过 0x / 0X 前缀 */
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        s += 2;
    }

    while (*s) {
        char c = *s++;
        uint32_t nibble;
        if (c >= '0' && c <= '9')      nibble = (uint32_t)(c - '0');
        else if (c >= 'a' && c <= 'f') nibble = (uint32_t)(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') nibble = (uint32_t)(c - 'A' + 10);
        else return -1;
        val = (val << 4) | nibble;
        digits++;
        if (digits > 8) return -1;  /* 超过 32-bit */
    }

    if (digits == 0) return -1;
    *out = val;
    return 0;
}

/* ========================================================================
 * Shell 工具函数：行输入
 * ======================================================================== */

#define SHELL_LINE_MAX  64  /* 命令行缓冲区（栈上分配） */
#define SHELL_ARGS_MAX  4   /* 最多 4 个参数（命令 + 3 个参数） */

/*
 * 从 UART 读取一行（回车结束），支持退格。
 * 返回行长度（不含 '\0'），buf 以 '\0' 结尾。
 * 无超时——Shell 模式下用户已在交互，永久等待输入。
 */
static int shell_readline(char *buf, int maxlen)
{
    int pos = 0;

    for (;;) {
        int c = bootrom_uart_getc(0xFFFFFFFF);
        if (c < 0) continue;

        if (c == '\r' || c == '\n') {
            bootrom_uart_puts("\r\n");
            buf[pos] = '\0';
            return pos;
        }

        if (c == '\b' || c == 0x7F) {
            /* 退格 */
            if (pos > 0) {
                pos--;
                bootrom_uart_puts("\b \b");
            }
            continue;
        }

        if (c >= 0x20 && pos < maxlen - 1) {
            buf[pos++] = (char)c;
            bootrom_uart_putc((char)c);  /* 回显 */
        }
    }
}

/*
 * 将命令行按空格拆分为 argv[]。
 * 返回 argc（参数个数）。原始 buf 会被修改（空格替换为 '\0'）。
 */
static int shell_tokenize(char *buf, char *argv[], int max_args)
{
    int argc = 0;
    char *p = buf;

    while (*p && argc < max_args) {
        /* 跳过前导空格 */
        while (*p == ' ') p++;
        if (*p == '\0') break;

        argv[argc++] = p;

        /* 找到参数结尾 */
        while (*p && *p != ' ') p++;
        if (*p) *p++ = '\0';
    }
    return argc;
}

/*
 * 简单字符串比较（不依赖 libc）。
 * 返回 0 表示相等。
 */
static int shell_streq(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a++ != *b++) return 1;
    }
    return (*a != *b) ? 1 : 0;
}

/* ========================================================================
 * Shell 命令：md — Memory Dump
 * ======================================================================== */

/*
 * md <addr> [len]
 *
 * 以十六进制 dump 内存，默认 64 字节。
 * 输出格式（类似 U-Boot）：
 *   0x30060000: 4a544147 00000000 00000000 00000000  JTAG............
 */
static void shell_cmd_md(int argc, char *argv[])
{
    if (argc < 2) {
        bootrom_uart_puts("Usage: md <addr> [len]\r\n");
        return;
    }

    uint32_t addr;
    if (shell_parse_hex(argv[1], &addr) < 0) {
        bootrom_uart_puts("Bad address\r\n");
        return;
    }

    /* 地址对齐到 4 字节 */
    addr &= ~0x3UL;

    uint32_t len = 64;  /* 默认 64 字节 */
    if (argc >= 3) {
        if (shell_parse_hex(argv[2], &len) < 0) {
            bootrom_uart_puts("Bad length\r\n");
            return;
        }
    }
    /* 限制单次 dump 上限 256 字节，防止刷屏 */
    if (len > 256) len = 256;

    uint32_t end = addr + len;

    while (addr < end) {
        /* 行首地址 */
        shell_put_hex32(addr);
        bootrom_uart_puts(": ");

        /* 每行 16 字节（4 个 32-bit word） */
        uint8_t line_bytes[16];
        uint32_t line_len = (end - addr >= 16) ? 16 : (end - addr);

        /* 读取并输出 hex word */
        for (uint32_t i = 0; i < 16; i += 4) {
            if (i < line_len) {
                uint32_t word = REG32(addr + i);
                /* 存储到 line_bytes 用于后面 ASCII 输出 */
                line_bytes[i + 0] = (uint8_t)(word & 0xFF);
                line_bytes[i + 1] = (uint8_t)((word >> 8) & 0xFF);
                line_bytes[i + 2] = (uint8_t)((word >> 16) & 0xFF);
                line_bytes[i + 3] = (uint8_t)((word >> 24) & 0xFF);
                shell_put_hex8(line_bytes[i + 3]);
                shell_put_hex8(line_bytes[i + 2]);
                shell_put_hex8(line_bytes[i + 1]);
                shell_put_hex8(line_bytes[i + 0]);
            } else {
                bootrom_uart_puts("        ");
                line_bytes[i + 0] = 0;
                line_bytes[i + 1] = 0;
                line_bytes[i + 2] = 0;
                line_bytes[i + 3] = 0;
            }
            bootrom_uart_putc(' ');
        }

        /* ASCII 列 */
        bootrom_uart_putc(' ');
        for (uint32_t i = 0; i < line_len; i++) {
            char c = (char)line_bytes[i];
            bootrom_uart_putc((c >= 0x20 && c <= 0x7E) ? c : '.');
        }

        bootrom_uart_puts("\r\n");
        addr += 16;
    }
}

/* ========================================================================
 * Shell 命令：mw — Memory Write
 * ======================================================================== */

/*
 * mw <addr> <value>
 *
 * 向指定地址写入 32-bit 值。
 * 地址必须 4 字节对齐。
 */
static void shell_cmd_mw(int argc, char *argv[])
{
    if (argc < 3) {
        bootrom_uart_puts("Usage: mw <addr> <value>\r\n");
        return;
    }

    uint32_t addr, val;
    if (shell_parse_hex(argv[1], &addr) < 0) {
        bootrom_uart_puts("Bad address\r\n");
        return;
    }
    if (shell_parse_hex(argv[2], &val) < 0) {
        bootrom_uart_puts("Bad value\r\n");
        return;
    }

    if (addr & 0x3) {
        bootrom_uart_puts("Address must be 4-byte aligned\r\n");
        return;
    }

    REG32(addr) = val;
    FENCE();

    /* 回读验证 */
    uint32_t readback = REG32(addr);
    shell_put_hex32(addr);
    bootrom_uart_puts(" <- ");
    shell_put_hex32(val);
    if (readback != val) {
        bootrom_uart_puts(" (readback: ");
        shell_put_hex32(readback);
        bootrom_uart_putc(')');
    }
    bootrom_uart_puts("\r\n");
}

/* ========================================================================
 * Shell 命令：info — 显示分区表和镜像信息
 * ======================================================================== */

static void shell_show_image_info(const char *label, uint32_t flash_offset)
{
    const image_header_t *hdr =
        (const image_header_t *)bootrom_flash_ptr(flash_offset);

    bootrom_uart_puts(label);

    if (hdr->magic != IMAGE_MAGIC) {
        bootrom_uart_puts(" [EMPTY/INVALID]\r\n");
        return;
    }

    bootrom_uart_puts("\r\n");

    bootrom_uart_puts("  magic:    ");
    shell_put_hex32(hdr->magic);
    bootrom_uart_puts("\r\n");

    bootrom_uart_puts("  size:     ");
    shell_put_dec(hdr->image_size);
    bootrom_uart_puts(" bytes\r\n");

    bootrom_uart_puts("  entry:    ");
    shell_put_hex32(hdr->entry_point);
    bootrom_uart_puts("\r\n");

    bootrom_uart_puts("  load:     ");
    shell_put_hex32(hdr->load_addr);
    bootrom_uart_puts("\r\n");

    bootrom_uart_puts("  mode:     ");
    bootrom_uart_puts(hdr->boot_mode == BOOT_MODE_XIP ? "XIP" : "ILM");
    bootrom_uart_puts("\r\n");

    bootrom_uart_puts("  version:  ");
    shell_put_dec(hdr->version);
    bootrom_uart_puts("\r\n");

    bootrom_uart_puts("  crc_hdr:  ");
    shell_put_hex32(hdr->hdr_crc32);
    bootrom_uart_puts("\r\n");

    bootrom_uart_puts("  crc_img:  ");
    shell_put_hex32(hdr->image_crc32);

    /* 在线验证 payload CRC */
    uint32_t payload_offset = flash_offset + IMAGE_HDR_SIZE;
    uint32_t calc = bootrom_crc32(bootrom_flash_ptr(payload_offset),
                                  hdr->image_size);
    if (calc == hdr->image_crc32) {
        bootrom_uart_puts(" [OK]");
    } else {
        bootrom_uart_puts(" [BAD, calc=");
        shell_put_hex32(calc);
        bootrom_uart_putc(']');
    }
    bootrom_uart_puts("\r\n");
}

static void shell_cmd_info(void)
{
    bootrom_uart_puts("=== Flash Partition Info ===\r\n");

    /* 分区表 */
    const partition_table_t *pt =
        (const partition_table_t *)bootrom_flash_ptr(0);

    bootrom_uart_puts("Partition table: ");
    if (pt->magic == PARTITION_TABLE_MAGIC) {
        bootrom_uart_puts("VALID\r\n");
        bootrom_uart_puts("  active:   Slot ");
        bootrom_uart_putc(pt->active_slot == SLOT_B ? 'B' : 'A');
        bootrom_uart_puts("\r\n");
        bootrom_uart_puts("  boot_cnt: ");
        shell_put_dec(pt->boot_count);
        bootrom_uart_puts("\r\n");
        bootrom_uart_puts("  rollback: ");
        shell_put_dec(pt->rollback_limit);
        bootrom_uart_puts("\r\n");
    } else {
        bootrom_uart_puts("NOT FOUND (magic=");
        shell_put_hex32(pt->magic);
        bootrom_uart_puts(")\r\n");
    }

    /* Slot A */
    shell_show_image_info("Slot A @0x400:", SLOT_A_OFFSET);

    /* Slot B */
    shell_show_image_info("Slot B @0x100000:", SLOT_B_OFFSET);
}

/* ========================================================================
 * Shell 命令：go — 跳转到指定地址执行
 * ======================================================================== */

/*
 * go <addr>
 *
 * 跳转到指定地址执行，不返回。
 * 执行前会做 fence + fence.i + 关中断，与正常启动跳转流程一致。
 */
static void shell_cmd_go(int argc, char *argv[])
{
    if (argc < 2) {
        bootrom_uart_puts("Usage: go <addr>\r\n");
        return;
    }

    uint32_t addr;
    if (shell_parse_hex(argv[1], &addr) < 0) {
        bootrom_uart_puts("Bad address\r\n");
        return;
    }

    bootrom_uart_puts("Jump to ");
    shell_put_hex32(addr);
    bootrom_uart_puts("...\r\n");

    bootrom_jump_to_app(addr);
}

/* ========================================================================
 * BootROM Shell（交互式命令行）
 * ======================================================================== */

static void shell_print_help(void)
{
    bootrom_uart_puts("Commands:\r\n");
    bootrom_uart_puts("  md <addr> [len]   - memory dump (hex, default 64B)\r\n");
    bootrom_uart_puts("  mw <addr> <val>   - memory write (32-bit)\r\n");
    bootrom_uart_puts("  info              - show partition table & images\r\n");
    bootrom_uart_puts("  go <addr>         - jump to address\r\n");
    bootrom_uart_puts("  boot flash        - boot from Flash\r\n");
    bootrom_uart_puts("  boot uart         - enter UART download\r\n");
    bootrom_uart_puts("  boot jtag         - enter JTAG debug mode\r\n");
    bootrom_uart_puts("  help              - show this help\r\n");
}

/*
 * BootROM Shell 主循环。
 *
 * boot pin = 00 时，UART 上等待空格键进入。
 * 进入后提供交互式命令行，用户可以：
 *   - 查看/修改内存和寄存器
 *   - 查看 Flash 分区和镜像信息
 *   - 手动选择启动模式
 *   - 跳转到任意地址执行
 *
 * 返回值：
 *   >= 0: 用户通过 "boot" 命令选择了启动模式
 *   < 0:  超时未进入 Shell（直接返回，继续自动启动流程）
 */
int bootrom_uart_boot_menu(void)
{
    bootrom_uart_puts("Press SPACE to enter shell...\r\n");

    /* 等待空格键，超时则直接返回 */
    int c = bootrom_uart_getc(BOOTROM_MENU_TIMEOUT_CYCLES);
    if (c < 0 || (char)c != BOOTROM_MENU_KEY) {
        return -1;  /* 超时或非空格键，不进入 Shell */
    }

    /* 进入 Shell */
    bootrom_uart_puts("\r\n");
    bootrom_uart_puts("=== QL25 BootROM Shell ===\r\n");
    shell_print_help();

    char line[SHELL_LINE_MAX];
    char *argv[SHELL_ARGS_MAX];

    for (;;) {
        bootrom_uart_puts("QL25> ");
        int len = shell_readline(line, SHELL_LINE_MAX);
        if (len == 0) continue;

        int argc = shell_tokenize(line, argv, SHELL_ARGS_MAX);
        if (argc == 0) continue;

        if (shell_streq(argv[0], "md") == 0) {
            shell_cmd_md(argc, argv);
        } else if (shell_streq(argv[0], "mw") == 0) {
            shell_cmd_mw(argc, argv);
        } else if (shell_streq(argv[0], "info") == 0) {
            shell_cmd_info();
        } else if (shell_streq(argv[0], "go") == 0) {
            shell_cmd_go(argc, argv);
        } else if (shell_streq(argv[0], "boot") == 0) {
            if (argc < 2) {
                bootrom_uart_puts("Usage: boot flash|uart|jtag\r\n");
            } else if (shell_streq(argv[1], "flash") == 0) {
                return BOOT_MODE_FLASH;
            } else if (shell_streq(argv[1], "uart") == 0) {
                return BOOT_MODE_UART_DL;
            } else if (shell_streq(argv[1], "jtag") == 0) {
                return BOOT_MODE_JTAG_DEBUG;
            } else {
                bootrom_uart_puts("Unknown: boot flash|uart|jtag\r\n");
            }
        } else if (shell_streq(argv[0], "help") == 0) {
            shell_print_help();
        } else {
            bootrom_uart_puts("Unknown command. Type 'help'\r\n");
        }
    }
}

/* ========================================================================
 * JTAG 调试模式
 * ======================================================================== */

void bootrom_enter_jtag_mode(void)
{
    /*
     * JTAG 调试模式：BootROM 做完最小初始化后停住。
     *
     * 此时 CPU 状态干净：
     *   - 中断已关闭
     *   - SP 指向 SRAM3 顶部
     *   - FPU 已使能
     *   - Core 0 保持复位
     *   - USART0 已初始化（可用于调试输出）
     *
     * 调试器（OpenOCD）可以：
     *   1. halt CPU
     *   2. 通过 JTAG 直接写内存（ILM/SRAM/Flash）
     *   3. 设置 PC 到目标地址
     *   4. resume 执行
     *
     * 或者使用 GDB load 命令加载 ELF 后 continue。
     *
     * JTAG 模式特殊标记：在 SRAM3 起始写入标记字，
     * 方便调试器脚本检测当前处于 JTAG 等待状态。
     */

    /* 在 SRAM3 起始写入标记，供调试器检测 */
    REG32(SRAM3_BASE) = 0x4A544147;  /* "JTAG" ASCII */

    bootrom_uart_puts("JTAG mode: waiting for debugger...\r\n");
    bootrom_uart_puts("Connect OpenOCD, halt, load, continue.\r\n");

    /*
     * 死循环 + WFI
     * JTAG 调试器可以随时 halt 此循环，然后接管 CPU
     */
    for (;;) {
        __asm volatile("wfi");
    }
}

/* ========================================================================
 * Flash 启动（支持 A/B 分区回滚）
 * ======================================================================== */

int bootrom_boot_from_flash(void)
{
    /* 读分区表 */
    const partition_table_t *pt =
        (const partition_table_t *)bootrom_flash_ptr(0);

    uint32_t try_slot_offset;
    uint32_t fallback_offset;

    if (pt->magic == PARTITION_TABLE_MAGIC) {
        /* 分区表有效，按 active_slot 选择 */
        if (pt->active_slot == SLOT_B) {
            try_slot_offset = pt->slot_b_offset;
            fallback_offset = pt->slot_a_offset;
        } else {
            try_slot_offset = pt->slot_a_offset;
            fallback_offset = pt->slot_b_offset;
        }
    } else {
        /* 分区表无效，直接从 Slot A 启动（首次烧录场景） */
        try_slot_offset = SLOT_A_OFFSET;
        fallback_offset = SLOT_B_OFFSET;
    }

    /* 尝试主分区 */
    int ret = bootrom_load_and_verify(try_slot_offset);
    if (ret == BOOTROM_OK) {
        return BOOTROM_OK;  /* 不会到达，已跳转 */
    }

    bootrom_uart_puts("SLOT1 fail, try fallback\r\n");

    /* 主分区失败，尝试备份分区 */
    ret = bootrom_load_and_verify(fallback_offset);
    if (ret == BOOTROM_OK) {
        return BOOTROM_OK;
    }

    /* 两个分区都失败 */
    return BOOTROM_ERR_NO_IMAGE;
}

/* ========================================================================
 * UART 下载启动
 * ======================================================================== */

int bootrom_boot_from_uart(void)
{
    /*
     * UART 下载协议（115200 8N1）：
     *
     * 1. BootROM → Host: 发送同步标记 "QL25BOOT\n"
     * 2. Host → BootROM: 64 字节镜像头
     * 3. BootROM 验证头部，发送 ACK/NAK
     * 4. Host → BootROM: image_size 字节 payload
     * 5. BootROM 验证 CRC，发送 ACK/NAK
     * 6. 验证通过则跳转到入口
     */

    uint32_t timeout = 0x00FFFFFF;  /* 超时计数 */

    /* 循环发送同步标记，等待主机连接 */
    for (int attempt = 0; attempt < 100; attempt++) {
        bootrom_uart_puts(UART_SYNC_MARKER);

        /* 等待主机响应（发送头部的第一个字节） */
        int c = bootrom_uart_getc(timeout);
        if (c >= 0) {
            /* 收到响应，开始接收头部 */
            image_header_t hdr;
            uint8_t *hdr_buf = (uint8_t *)&hdr;
            hdr_buf[0] = (uint8_t)c;

            /* 接收剩余 63 字节 */
            if (bootrom_uart_recv(&hdr_buf[1], IMAGE_HDR_SIZE - 1, timeout) < 0) {
                bootrom_uart_putc(UART_NAK);
                continue;
            }

            /* 验证头部 */
            int ret = bootrom_validate_header(&hdr);
            if (ret != BOOTROM_OK) {
                bootrom_uart_putc(UART_NAK);
                continue;
            }

            /* 头部有效，发送 ACK */
            bootrom_uart_putc(UART_ACK);

            /* 接收 payload 到目标地址 */
            uint8_t *load_dst = (uint8_t *)hdr.load_addr;
            if (bootrom_uart_recv(load_dst, hdr.image_size, timeout) < 0) {
                bootrom_uart_putc(UART_NAK);
                continue;
            }

            /* 验证 payload CRC */
            uint32_t calc_crc = bootrom_crc32(load_dst, hdr.image_size);
            if (calc_crc != hdr.image_crc32) {
                bootrom_uart_putc(UART_NAK);
                continue;
            }

            /* 全部验证通过 */
            bootrom_uart_putc(UART_ACK);
            bootrom_uart_puts("OK, jump\r\n");

            FENCE();
            FENCE_I();

            bootrom_jump_to_app(hdr.entry_point);
            /* 不应到达 */
        }
    }

    return BOOTROM_ERR_UART_TIMEOUT;
}

/* ========================================================================
 * 跳转到应用入口
 * ======================================================================== */

void bootrom_jump_to_app(uint32_t entry_point)
{
    /*
     * 跳转前清理：
     * 1. fence.i 刷新 I-Cache（确保新搬运的代码可见）
     * 2. 关中断（应用 startup 会重新配置）
     * 3. 跳转（不返回）
     */
    typedef void (*entry_fn_t)(void);

    FENCE();
    FENCE_I();

    /* 关闭 BootROM 阶段的中断配置，让应用 startup 重新初始化 */
    __asm volatile("csrc mstatus, %0" :: "r"(0x8));  /* MIE = 0 */

    entry_fn_t entry = (entry_fn_t)entry_point;
    entry();

    /* 不应到达 */
    for (;;) {
        __asm volatile("wfi");
    }
}

/* ========================================================================
 * 致命错误处理
 * ======================================================================== */

void bootrom_fatal(uint32_t error_code)
{
    /* 通过 UART 输出错误码，方便调试 */
    bootrom_uart_puts("BOOTROM FATAL: ");

    /* 简单的十进制输出 */
    char buf[4];
    buf[0] = '0' + (char)(error_code / 10);
    buf[1] = '0' + (char)(error_code % 10);
    buf[2] = '\r';
    buf[3] = '\n';
    bootrom_uart_send((const uint8_t *)buf, 4);

    /* 死循环，等待 JTAG 调试器接入 */
    for (;;) {
        __asm volatile("wfi");
    }
}

/* ========================================================================
 * BootROM 主入口（从 bootrom_entry.S 调用）
 * ======================================================================== */

void bootrom_main(void)
{
    /*
     * 整体流程：
     *
     * 1. 保持 Core 0（从核）复位
     * 2. 初始化 USART0（调试输出 + UART 下载）
     * 3. 三级启动模式检测：
     *    第一级：boot pin 硬件强制选择（01=UART, 10=JTAG, 11=Reserved）
     *    第二级：boot pin=00 时，UART 等待空格键进入交互菜单（1~2秒窗口）
     *    第三级：无人干预则自动检测 Flash，无效镜像 fallback UART
     * 4. 按模式分派：
     *    - Flash：读分区表 → A/B 验证 → 加载 → 跳转（失败 fallback UART）
     *    - UART：等待主机 → 接收镜像 → 验证 → 跳转
     *    - JTAG：最小初始化后 WFI 停住，等调试器接入
     *    - Reserved：安全兜底到 UART 下载
     * 5. 全部失败：fatal 死循环，等 JTAG
     */

    /* Step 1: 确保从核保持复位 */
    bootrom_hold_core0_reset();

    /* Step 2: 初始化 USART0（不论哪种模式都需要，用于调试输出） */
    bootrom_uart_init(UART_BAUDRATE);
    bootrom_uart_puts("\r\nQL25 BootROM v1.0\r\n");

    /* Step 3: 检测启动模式 */
    int mode = bootrom_detect_boot_mode();

    int ret;

    switch (mode) {
    case BOOT_MODE_FLASH:
        /* Flash 正常启动（量产默认） */
        bootrom_uart_puts("Flash boot...\r\n");
        ret = bootrom_boot_from_flash();

        /* Flash 启动失败，自动 fallback 到 UART 下载 */
        bootrom_uart_puts("Flash fail, UART mode\r\n");
        ret = bootrom_boot_from_uart();
        bootrom_fatal((uint32_t)ret);
        break;

    case BOOT_MODE_UART_DL:
        /* UART 下载模式（boot pin 选择） */
        bootrom_uart_puts("UART download mode\r\n");
        ret = bootrom_boot_from_uart();
        bootrom_fatal((uint32_t)ret);
        break;

    case BOOT_MODE_JTAG_DEBUG:
        /* JTAG 调试模式：最小初始化后停住，等调试器接入 */
        bootrom_enter_jtag_mode();
        /* 不应到达 */
        break;

    default:
        /* BOOT_MODE_RESERVED 或非法值：进入 UART 下载作为安全兜底 */
        bootrom_uart_puts("Reserved mode, UART fallback\r\n");
        ret = bootrom_boot_from_uart();
        bootrom_fatal((uint32_t)ret);
        break;
    }

    /* 不应到达 */
    bootrom_fatal(BOOTROM_ERR_NO_IMAGE);
}
