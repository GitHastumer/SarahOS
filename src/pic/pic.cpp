#include "pic.h"

#include "../util.h"
#include "../serial/serial.h"

void pic::init() {
    util::outb(0x20, 0x11);
    util::outb(0x21, 0x20);
    util::outb(0x21, 0x04);
    util::outb(0x21, 0x01);

    util::outb(0xa0, 0x11);
    util::outb(0xa1, 0x28);
    util::outb(0xa1, 0x02);
    util::outb(0xa1, 0x01);

    util::outb(0x21, 0x0);
    util::outb(0xa1, 0x0);

    serial::printf("Initialized PIC.\n");
}