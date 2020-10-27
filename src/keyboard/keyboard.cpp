#include "keyboard.h"

#include "../terminal/term.h"
#include "../util.h"
#include "../serial/serial.h"

char scancodeToAscii[] = {
    0,
    0,
    '1',
    '2',

    '3',
    '4',
    '5',
    '6',

    '7',
    '8',
    '9',
    '0',

    '-',
    '=',
    '\b',
    '\t',

    'q',
    'w',
    'e',
    'r',

    't',
    'y',
    'u',
    'i',

    'o',
    'p',
    '[',
    ']',

    '\n',
    0,
    'a',
    's',

    'd',
    'f',
    'g',
    'h',

    'j',
    'k',
    'l',
    ';',

    '\'',
    '`',
    0,
    '\\',

    'z',
    'x',
    'c',
    'v',

    'b',
    'n',
    'm',
    ',',

    '.',
    '/',
    0,
    '*',

    0,
    ' ',
    0,
    0,

    0,
    0,
    0,
    0,

    0,
    0,
    0,
    0,

    0,
    0,
    0,
    '7',

    '8',
    '9',
    '-',
    '4',

    '5',
    '6',
    '+',
    '1',

    '2',
    '3',
    '0',
    '.',

    0,
    0,
    0,
    0,

    0
};

char scancodeToAsciiUpper[] = {
    0,
    0,
    '!',
    '@',

    '#',
    '$',
    '%',
    '^',

    '&',
    '*',
    '(',
    ')',

    '_',
    '+',
    '\b',
    '\t',

    'Q',
    'W',
    'E',
    'R',

    'T',
    'Y',
    'U',
    'I',

    'O',
    'P',
    '{',
    '}',

    '\n',
    0,
    'A',
    'S',

    'D',
    'F',
    'G',
    'H',

    'J',
    'K',
    'L',
    ':',

    '\"',
    '~',
    0,
    '|',

    'Z',
    'X',
    'C',
    'V',

    'B',
    'N',
    'M',
    '<',

    '>',
    '?',
    0,
    '*',

    0,
    ' ',
    0,
    0,

    0,
    0,
    0,
    0,

    0,
    0,
    0,
    0,

    0,
    0,
    0,
    '7',

    '8',
    '9',
    '-',
    '4',

    '5',
    '6',
    '+',
    '1',

    '2',
    '3',
    '0',
    '.',

    0,
    0,
    0,
    0,

    0
};

kb::kb_state_t kb_state;

void kb::command(uint8_t cmd) {
    while (util::inb(0x64) & 2); // Wait for the command buffer to be emptied
    
    util::outb(0x60, cmd);
}

void kb::command(uint8_t cmd, uint8_t data) {
    while (util::inb(0x64) & 2);

    util::outb(0x60, cmd);
    util::outb(0x60, data);
}

void kb::handleIRQ() {
    uint8_t scancode = util::inb(0x60);

    kb_state.scan_buf[kb_state.curr_pos] = scancode;

    if (kb_state.curr_pos == 99) {
        kb_state.curr_pos = 0;
    } else {
        ++kb_state.curr_pos;
    }

    ++kb_state.buffered_keys;
}

int kb::getbufferedkeys() {
    return kb_state.buffered_keys;
}

char kb::getch() {
    char c;

    do {
        c = getch_noblock();
    } while (c == '\0');

    return c;
}

char kb::getch_noblock() {
    if (kb_state.buffered_keys == 0)
        return 0;
    
    kb_state.buffered_keys -= 1;
    kb_state.curr_pos -= 1;

    uint8_t scancode = kb_state.scan_buf[kb_state.curr_pos];
    bool down = !(scancode & 0x80); // true = pressed, false = released
    scancode &= 0b01111111; // remove down/up flag

    if (down) {
        switch (scancode) {
            case 0x2a: // left shift
            case 0x36: // right shift
                kb_state.curr_flags |= SHIFT;
                return 0;
            case 0x3a: // caps
                kb_state.curr_flags ^= CAPS;
                return 0;
            default:
                if (((kb_state.curr_flags & SHIFT) == 0)
                    || (((kb_state.curr_flags & SHIFT) == SHIFT) && (kb_state.curr_flags & CAPS) == CAPS))
                    return scancodeToAscii[scancode];
                else
                    return scancodeToAsciiUpper[scancode];
        }
    } else {
        switch (scancode) {
            case 0x2a: // left shift
            case 0x36: // right shift
                kb_state.curr_flags &= (0xff ^ SHIFT);
                return 0;
            default:
                return 0;
        }
    }
}

void kb::init() {
    util::outb(0x64, 0xaa);

    int ret = util::inb(0x60);
    if (ret != 0x55) {
        term::printf("The keyboard returned %d instead of 85. What happened there?\n");
        serial::printf("The keyboard returned %d instead of 85. What happened there?\n");
        return;
    }

    while (util::inb(0x64) & 0x1) { // Clear the keyboard buffer
        util::inb(0x60);
    }

    command(0xf4);
}