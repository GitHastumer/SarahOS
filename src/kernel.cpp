#include <stdint.h>
#include <stddef.h>

#include "terminal/term.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "pic/pic.h"
#include "keyboard/keyboard.h"

#include "stdlib/string.h"

extern "C" void kernel_main(void)  {
    term::init();

    gdt::init();

    idt::init();

    pic::init();

    kb::init();

    term::printf("SarahOS kernel v0.5\n");

    // Simple shell
    char input[61];
    int i = 0;

    for (int j = 0; j < 61; ++j) {
        input[j] = 0;
    }

    term::printf("> ");

    while (1) {
        char c = kb::getch();

        if (c == '\b') {
            if (i > 0) {
                input[--i] = '\0';
                term::putc('\b');
            }

            continue;
        } else if (c == '\n') {
            term::putc('\n');

            // Handle command

            if (!std::strcmp(input, "help")) {
                term::printf("A list of all available commands:\n");
                term::printf("help - Show this page.\n");
            }

            //Reset input

            for (int j = 0; j < 61; ++j) {
                input[j] = 0;
            }

            i = 0;

            term::printf("> ");

            continue;
        } else {
            if (i < 59) {
                input[i++] = c;
                term::putc(c);
            }
        }
    }
}