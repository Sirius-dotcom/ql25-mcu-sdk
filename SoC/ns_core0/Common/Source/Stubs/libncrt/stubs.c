#include <stdint.h>
#include <time.h>
#include <nuclei_sdk_hal.h>

int metal_tty_putc(int c)
{
    if (c == '\n') {
        USART_SendData(SOC_DEBUG_USART, '\r');
    }
    USART_SendData(SOC_DEBUG_USART, (uint8_t)c);
    return 0;
}

int metal_tty_getc(void)
{
    int c = (int)USART_ReceiveData(SOC_DEBUG_USART);
    return c;
}

void exit(int fd)
{
    while (1) {
        __WFI();
    }
}

__WEAK clock_t clock(void)
{
    uint64_t cycles = __get_rv_cycle();
    clock_t clktm;

    clktm = (cycles * CLOCKS_PER_SEC) / SystemCoreClock;
    return clktm;
}

__WEAK void __libc_fini_array(void)
{
}

extern void __SEGGER_RTL_init_heap(void *ptr, size_t size);
extern char __heap_start[];
extern char __heap_end[];

__WEAK void __libc_init_array(void)
{
    size_t heapsz = (size_t)__heap_end - (size_t)__heap_start;
    __SEGGER_RTL_init_heap((void *)__heap_start, heapsz);
}

__WEAK void __SEGGER_RTL_init_heap(void *ptr, size_t size)
{

}
