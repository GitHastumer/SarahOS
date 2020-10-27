#if !defined(SERIAL_H)
#define SERIAL_H

#include <stdint.h>

namespace serial {
    constexpr int BAUDRATE = 9600;
    constexpr uint16_t portCOM1 = 0x3f8;

    void init();
    void send(const char *str);
    void printf(const char *fmt, ...);
}

#endif // SERIAL_H
