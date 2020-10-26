#if !defined(KEYBOARD_H)
#define KEYBOARD_H

#include <stdint.h>
#include <stddef.h>

namespace kb {
    typedef struct {
        uint8_t scan_buf[100];
        int curr_pos = 0;
        uint8_t curr_flags = 0;
        int buffered_keys = 0;
    } kb_state_t;

    enum kb_flags {
        CAPS = 1,
        SHIFT = 2,
        NUMLOCK = 4,
        SCROLLLOCK = 8
    };

    void init();

    void command(uint8_t cmd);
    void command(uint8_t cmd, uint8_t data);

    void handleIRQ();

    char getch();
    char getch_noblock();
    
    int getbufferedkeys();
}

#endif // KEYBOARD_H
