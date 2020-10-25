#include "inthandler.h"

#include "../terminal/term.h"
#include "../pic/pic.h"
#include "../keyboard/keyboard.h"
#include "../util.h"

const char *exceptionNames[] = {
    "Divide by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range",
    "Invalid opcode",
    "Device not available",
    "Double Fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack fault",
    "General protection",
    "Page fault",
    "",
    "x87 Floating point",
    "Alignment check",
    "Machine check",
    "SIMD Floating point",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "Security-sensitive event in host",
    ""
};

#define IRQ_BEGIN 32

#define IRQ(n) IRQ_BEGIN + n

#define SYSCALL_BEGIN IRQ(15) + 1

#define SYSCALL(n) SYSCALL_BEGIN + n

extern "C" inthandler::cpu_state *handle_int(inthandler::cpu_state *cpu) {
    inthandler::cpu_state *newState = cpu;

    if (cpu->intr <= 0x1f) {
        term::printf("%s exception %d, kernel halted. Dumping processor information: \n", exceptionNames[cpu->intr], cpu->intr);

        term::printf("eax=%d ebx=%d ecx=%d edx=%d\n", cpu->eax, cpu->ebx, cpu->ecx, cpu->edx);
        term::printf("esi=%x edi=%x ebp=%x eip=%x\n", cpu->esi, cpu->edi, cpu->ebp, cpu->eip);
        term::printf("cs=%x ss=%x esp=%x eflags=%x\n", cpu->cs, cpu->ss, cpu->esp, cpu->eflags);
        term::printf("intr=%d error=%d\n", cpu->intr, cpu->error);

        while (1) {
            asm volatile("cli; hlt");
        }
    } else {
        if (cpu->intr == IRQ(0)) {
            
        } else if (cpu->intr == IRQ(1)) {
            kb::handleIRQ();
        } else if (cpu->intr < SYSCALL_BEGIN) {
            term::printf("Received interrupt %d.\n", cpu->intr);
        } else if (cpu->intr == SYSCALL(0)) {
            term::printf("SYSCALL 0\n");
        }
        
        if (cpu->intr >= 0x20 && cpu->intr <= 0x2f) { // Send EOI to PIC
            util::outb(0xa0, 0x20);
        }

        util::outb(0x20, 0x20);
    }

    return newState;
}