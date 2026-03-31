/* See LICENSE of license details. */
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include "nuclei_sdk_hal.h"

#undef getchar

int getchar(void)
{
    int dat;

    dat = (int)USART_ReceiveData(SOC_DEBUG_USART);
#ifdef UART_AUTO_ECHO
    USART_SendData(SOC_DEBUG_USART, (uint8_t)dat);
#endif
    return dat;
}

__WEAK ssize_t _read(int fd, void* ptr, size_t len)
{
    if (fd != STDIN_FILENO) {
        return -1;
    }
    ssize_t cnt = 0;
    uint8_t* readbuf = (uint8_t*)ptr;
    for (cnt = 0; cnt < len; cnt ++) {
        readbuf[cnt] = getchar();
        /* Return partial buffer if we get EOL */
        if (readbuf[cnt] == '\n') {
            return cnt;
        }
    }

    return cnt;
}
