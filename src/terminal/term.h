#if !defined(TERM_H)
#define TERM_H

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#include "../util.h"

namespace term {
    constexpr size_t VGA_WIDTH = 80;
    constexpr size_t VGA_HEIGHT = 25;

    enum colors {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        BROWN = 6,
        LIGHT_GRAY = 7,
        DARK_GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        ORANGE = 14,
        WHITE = 15
    };

    inline uint8_t entry_color(enum colors fg_color, enum colors bg_color);
    inline uint16_t entry(unsigned char ch, uint8_t color);

    void init();

    void setCursor(int x, int y);

    void putc(char c);
    void puts(const char *str);

    void putn(int64_t n);
    void putbasedn(int64_t n, int base);
    
    void putun(uint64_t n);
    void putbasedun(uint64_t n, int base);

    void printf(const char *format, ...);
}

#endif // TERM_H
