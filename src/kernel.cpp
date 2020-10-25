#include <stdint.h>
#include <stddef.h>

#include "terminal/term.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "pic/pic.h"
#include "keyboard/keyboard.h"

#include "stdlib/string.h"

extern "C" void executeSyscall(void);

extern "C" void kernel_main(void)  {
    term::init();

    term::puts("Terminal initialized.\n");

    gdt::init();

    term::puts("GDT initialized.\n");

    idt::init();

    term::puts("IDT initialized.\n");

    pic::init();

    term::puts("PIC initialized.\n");

    kb::init();

    term::puts("Keyboard initialized.\n");

    enableHardwareInterrupts();

    term::puts("Hardware interrupts enabled.\n");

    term::printf("SarahOS kernel v0.5\n");

    asm volatile("int $0x30");

    // Simple shell
    char input[61];
    char argv[31][31];
    int i = 0;
    int argc = 0;
    int arg_i = 0;

    for (int j = 0; j < 61; ++j) {
        input[j] = 0;
    }

    for (int j = 0; j < 31; ++j) {
        for (int k = 0; k < 31; ++k) {
            argv[j][k] = 0;
        }
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

            // Split command

            for (int j = 0; input[j] != '\0'; ++j) {
                if (input[j] != ' ') {
                    argv[argc][arg_i++] = input[j];
                } else {
                    argc++;
                    arg_i = 0;
                }
            }

            // Handle command
            if (!std::strcmp(argv[0], "help")) {
                term::printf("A list of all available commands:\n");
                term::printf("help - Show this page.\n");
                term::printf("syscall - Execute a syscall\n");
            } else if (!std::strcmp(argv[0], "syscall")) {
                executeSyscall();
            }

            //Reset input

            for (int j = 0; j < 61; ++j) {
                input[j] = 0;
            }

            for (int j = 0; j < 31; ++j) {
                for (int k = 0; k < 31; ++k) {
                    argv[j][k] = 0;
                }
            }

            argc = 0;
            arg_i = 0;

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