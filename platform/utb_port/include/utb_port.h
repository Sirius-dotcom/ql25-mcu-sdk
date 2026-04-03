#ifndef UTB_PORT_H
#define UTB_PORT_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define UTB_OK                  0
#define UTB_ERR                 (-1)
#define UTB_ERR_PARAM           (-2)
#define UTB_ERR_NOMEM           (-3)
#define UTB_ERR_TIMEOUT         (-4)
#define UTB_ERR_UNSUPPORTED     (-5)
#define UTB_ERR_BUSY            (-6)

void *utb_port_malloc(size_t size);
void utb_port_free(void *ptr);
int utb_port_printf(const char *fmt, ...);
int utb_port_vprintf(const char *fmt, va_list ap);

#define UTB_MALLOC(sz)          utb_port_malloc(sz)
#define UTB_FREE(ptr)           utb_port_free(ptr)
#define UTB_PRINT(...)          utb_port_printf(__VA_ARGS__)

#ifndef UTB_ARRAY_SIZE
#define UTB_ARRAY_SIZE(a)       (sizeof(a) / sizeof((a)[0]))
#endif

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define UTB_HTON16(x)           (x)
#define UTB_HTON32(x)           (x)
#else
#define UTB_HTON16(x)           ((uint16_t)((((uint16_t)(x)) << 8) | (((uint16_t)(x)) >> 8)))
#define UTB_HTON32(x)           ( \
    ((((uint32_t)(x)) & 0x000000FFu) << 24) | \
    ((((uint32_t)(x)) & 0x0000FF00u) << 8)  | \
    ((((uint32_t)(x)) & 0x00FF0000u) >> 8)  | \
    ((((uint32_t)(x)) & 0xFF000000u) >> 24))
#endif

#define UTB_NTOH16(x)           UTB_HTON16(x)
#define UTB_NTOH32(x)           UTB_HTON32(x)

#ifdef __cplusplus
}
#endif

#endif /* UTB_PORT_H */
