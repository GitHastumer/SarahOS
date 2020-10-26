#if !defined(UTIL_H)
#define UTIL_H

#include <stdint.h>
#include <stddef.h>

namespace util {
    inline void outb(uint16_t port, uint8_t data) {
        asm volatile("outb %0, %1" :: "a" (data), "Nd" (port));
    }

    inline uint8_t inb(uint16_t port) {
        uint8_t data;
        asm volatile("inb %1, %0" : "=a" (data) : "Nd" (port));
        return data;
    }

    size_t strlen(const char *str);
}

#endif // UTIL_H
