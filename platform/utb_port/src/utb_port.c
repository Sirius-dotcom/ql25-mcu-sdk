#include "utb_port.h"

#include <stdio.h>
#include <stdlib.h>

void *utb_port_malloc(size_t size)
{
    return malloc(size);
}

void utb_port_free(void *ptr)
{
    free(ptr);
}

int utb_port_vprintf(const char *fmt, va_list ap)
{
    return vprintf(fmt, ap);
}

int utb_port_printf(const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = utb_port_vprintf(fmt, ap);
    va_end(ap);

    return ret;
}
