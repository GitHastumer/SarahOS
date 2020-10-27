#include <stdint.h>
#include <stddef.h>

#include "terminal/term.h"
#include "gdt/gdt.h"
#include "interrupts/idt.h"
#include "pic/pic.h"
#include "keyboard/keyboard.h"
#include "multitasking/multitasking.h"
#include "serial/serial.h"

#include "stdlib/string.h"

extern "C" void executeSyscall(void);

extern "C" void kernel_main(void)  {
    term::init();

    term::puts("Terminal initialized.\n");

    serial::init();

    term::puts("Serial interface initialized.\n");

    gdt::init();

    term::puts("GDT initialized.\n");

    idt::init();

    term::puts("IDT initialized.\n");

    pic::init();

    term::puts("PIC initialized.\n");

    kb::init();

    term::puts("Keyboard initialized.\n");

    multitasking::init();

    term::puts("Multitasking initialized.\n");

    serial::printf("Enabling hardware interrupts... ");

    term::puts("Enabling hardware interrupts... ");

    enableHardwareInterrupts();

    serial::printf("Done.\n");

    term::puts("Done.\n");

    term::printf("SarahOS kernel v0.5\n");

    while (1);
}