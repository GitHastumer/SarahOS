#include "serial.h"

#include "../util.h"

#include <stdarg.h>

const char digitToChar[] = "0123456789abcdef";

void clearBuf(char *buf, int size) {
    for (int i = 0; i < size; ++i) {
        buf[i] = 0;
    }
}

void sendBuf(char *buf, int *buf_i, int size) {
    if (*buf != 0) {
        serial::send(buf);
        clearBuf(buf, size);
        *buf_i = 0;
    }
}

void itoa(int n, int base, bool prefix, char *buf, int *buf_i, int buf_size) {
    *buf_i = buf_size - 2;

    if (n == 0) {
        buf[(*buf_i)--] = '0';
    }
    
    while (n > 0) {
        buf[(*buf_i)--] = digitToChar[n % base];
        n /= base;
    }

    if (prefix)
        if (base == 2) {
            buf[(*buf_i)--] = 'b';
            buf[(*buf_i)--] = '0';
        } else if (base == 7) {
            buf[(*buf_i)--] = '0';
        } else if (base == 16) {
            buf[(*buf_i)--] = 'x';
            buf[(*buf_i)--] = '0';
        }

    ++(*buf_i);

    sendBuf(buf + (*buf_i), buf_i, buf_size - (*buf_i));
    
    *buf_i = 0;
}

void serial::init() {
    uint8_t lineControl = util::inb(portCOM1 + 3);
    lineControl |= 0b01000011; // set dlab bit to 1
    lineControl &= 0b11000011; // set parity to none, 8bit = 1 byte, 1 stop bit
    util::outb(portCOM1, lineControl);

    uint16_t baudrateThing = 115200 / BAUDRATE;
    util::outb(portCOM1, baudrateThing & 0xff); // write baudrate
    util::outb(portCOM1 + 1, (baudrateThing >> 8) & 0xff);

    lineControl = util::inb(portCOM1 + 3);
    lineControl &= 0b10111011; // set dlab bit back to 0
    util::outb(portCOM1, lineControl);  

    serial::printf("\nHello from the kernel! Baud rate: %d\n", BAUDRATE);
    serial::printf("Serial interface initialized.\n");
}

void serial::send(const char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        while (!(util::inb(portCOM1 + 5) & 0b00100000)); // wait for send allow

        util::outb(portCOM1, str[i]);
    }
}

void serial::printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[32];
    int buf_i = 0;
    int n;

    for (int i = 0; i < 32; ++i) {
        buf[i] = 0;
    }

    while (*fmt != 0) {
        if (*fmt == '%') {
            ++fmt;

            switch (*fmt) {
                case 's':
                    sendBuf(buf, &buf_i, 32);
                    serial::send(va_arg(args, char*));

                    break;
                case 'd':
                case 'i':
                    sendBuf(buf, &buf_i, 32);
                    itoa(va_arg(args, int), 10, false, buf, &buf_i, 32);
                    clearBuf(buf, 32);

                    break;
                case 'x':
                    sendBuf(buf, &buf_i, 32);
                    itoa(va_arg(args, int), 16, true, buf, &buf_i, 32);
                    clearBuf(buf, 32);

                    break;
                case 'X': // fuck upper case hex numbers.
                    sendBuf(buf, &buf_i, 32);
                    itoa(va_arg(args, int), 16, false, buf, &buf_i, 32);
                    clearBuf(buf, 32);

                    break;
                default:
                    buf[buf_i++] = *fmt;

                    break;
            }
        } else {
            buf[buf_i++] = *fmt;
        }

        ++fmt;

        if (buf_i == 31) {
            sendBuf(buf, &buf_i, 32);
        }
    }

    sendBuf(buf, &buf_i, 32);
}