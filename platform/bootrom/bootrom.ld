/**
 * QL25 BootROM 链接脚本
 *
 * BootROM 固化在主核（Core 1）16KB ROM 中。
 * 代码 + 只读数据 <= 16KB，无可写段（.data / .bss）。
 * 临时栈在片上 SRAM3 顶部，由汇编入口直接设置 SP。
 *
 * TODO: BOOTROM_ORIGIN 需要根据 SoC 实际 ROM 映射地址修改。
 */

MEMORY
{
    /* BootROM: 16KB 只读 ROM */
    rom (rx)  : ORIGIN = 0x00000000, LENGTH = 16K

    /*
     * 片上 SRAM3: 用于 BootROM 临时栈
     * 不在此分配段，只是让链接器知道这块内存存在。
     * 栈由 bootrom_entry.S 直接设 SP = 0x30070000。
     */
    sram (rw) : ORIGIN = 0x30060000, LENGTH = 64K
}

ENTRY(_bootrom_start)

SECTIONS
{
    /* 向量表放在 ROM 起始 */
    .bootrom_vtable : ALIGN(64)
    {
        KEEP(*(.text.bootrom_vtable))
    } >rom

    /* BootROM 入口代码 */
    .bootrom_entry :
    {
        *(.text.bootrom_entry)
    } >rom

    /* BootROM C 代码 + 其他文本 */
    .text :
    {
        *(.text .text.*)
        *(.rodata .rodata.*)
    } >rom

    /* 确保没有可写段被链接进来 */
    .data :
    {
        _data_start = .;
        *(.data .data.*)
        _data_end = .;
    } >rom

    .bss (NOLOAD) :
    {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    } >sram

    /* ROM 大小检查 */
    _bootrom_end = .;
    ASSERT(SIZEOF(.bootrom_vtable) + SIZEOF(.bootrom_entry) + SIZEOF(.text) + SIZEOF(.data) <= 16K,
           "BootROM exceeds 16KB ROM limit!")

    /* 确认没有可写全局变量（.data 应该为空） */
    ASSERT(_data_end == _data_start,
           "BootROM must not have writable .data section! Use const or stack variables only.")
    ASSERT(_bss_end == _bss_start,
           "BootROM must not have writable .bss section! Use const or stack variables only.")

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
        *(.note.*)
    }
}
