#include "term.h"
    
size_t row;
size_t col;
uint8_t color;
volatile uint16_t *buffer;

const char numberChars[] = "0123456789abcdef";
const char capitalChars[] = "0123456789ABCDEF";

inline uint8_t term::entry_color(enum colors fg_color, enum colors bg_color) {
    return fg_color | bg_color << 4;
}

inline uint16_t term::entry(unsigned char ch, uint8_t color) {
    return (uint16_t) ch | (uint16_t) color << 8;
}

int reverseNumber(int n, int base) {
    int reversed_n = 0;

    while (n) {
        reversed_n = reversed_n * base + n % base;
        n /= base;
    }

    return reversed_n;
}

void putCapitalHex(unsigned int n) {
    if (n == 0) {
        term::putc('0');
        return;
    }

    n = reverseNumber(n, 16); // Reverse n to print in the right order. Otherwise 123 would become 321.

    while (n) {
        term::putc(capitalChars[n % 16]);
        n /= 16;
    }
    
    term::putc(capitalChars[n % 16]);
}

void term::init() {
    row = 0;
    col = 0;

    color = entry_color(colors::WHITE, colors::CYAN);

    buffer = (uint16_t*) 0xb8000;

    for (size_t y = 0; y < VGA_HEIGHT; ++y) {
        for (size_t x = 0; x < VGA_WIDTH; ++x) {
            buffer[y * VGA_WIDTH + x] = entry(' ', color);
        }
    }
}

void term::setCursor(int x, int y) {
    uint16_t screenbuf_pos = y * VGA_WIDTH + x;

    util::outb(0x3d4, 14);
    util::outb(0x3d5, screenbuf_pos >> 8);
    util::outb(0x3d4, 15);
    util::outb(0x3d5, screenbuf_pos);
}

void term::putc(char c) {
    if (c == '\0')
        return;

    else if (c == '\b') {
        if (col > 0) {
            col--;

            buffer[row * VGA_WIDTH + col] = entry(' ', color);
        }

    } else if (c != '\n')
        buffer[row * VGA_WIDTH + (col++)] = entry(c, color);

    if (col == VGA_WIDTH || c == '\n') {
        col = 0;
        ++row;

        if (row == VGA_HEIGHT) { // Wrap text for now.
            row = 0;
        }
    }

    setCursor(col, row);
}

void term::putc(char c, int x, int y) {
    if (c == '\0')
        return;
    
    buffer[row * VGA_WIDTH + col] = entry(c, color);
}

void term::puts(const char *str) {
    for (size_t i = 0; str[i] != '\0'; ++i) {
        putc(str[i]);
    }
}

void term::putn(int64_t n) {
    putbasedn(n, 10);
}

void term::putbasedn(int64_t n, int base) {
    if (n < 0) {
        putc('-');
        n = -n;
    }
    
    if (n < 10) {
        putc(numberChars[n % base]);
        return;
    }

    n = reverseNumber(n, base); // Reverse n to print in the right order. Otherwise 123 would become 321.

    while (n) {
        putc(numberChars[n % base]);
        n /= base;
    }
}

void term::putun(uint64_t n) {
    putbasedun(n, 10);
}

void term::putbasedun(uint64_t n, int base) {
    if (n < 10) {
        putc(numberChars[n % base]);
        return;
    }

    n = reverseNumber(n, base); // Reverse n to print in the right order. Otherwise 123 would become 321.

    while (n) {
        putc(numberChars[n % base]);
        n /= base;
    }
}

void term::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            switch (*(++format)) {
                case 'c':
                    term::putc(va_arg(args, int));
                    break;
                case 'd':
                case 'i':
                    term::putn(va_arg(args, int));
                    break;
                case 'e':
                case 'E':
                case 'f':
                case 'g':
                case 'G':
                    term::puts("NOT IMPLEMENTED");
                    break;
                case 'o':
                    term::putbasedn(va_arg(args, int), 7);
                    break;
                case 's':
                    term::puts(va_arg(args, char*));
                    break;
                case 'u':
                    term::putun(va_arg(args, unsigned int));
                    break;
                case 'x':
                    term::putbasedun(va_arg(args, unsigned int), 16);
                    break;
                case 'X':
                    putCapitalHex(va_arg(args, unsigned int));
                    break;
                case 'p':
                    term::putun((uint64_t) va_arg(args, void*));
                    break;
                case 'n':
                    break;
                case '%':
                    term::putc('%');
                    break;
                default:
                    term::putc('%');
                    term::putc(*format);
                    break;
            }
        } else {
            putc(*format);
        }

        format++;
    }
}