#if !defined(INTHANDLER_H)
#define INTHANDLER_H

#include <stdint.h>
#include <stddef.h>

namespace inthandler {
    struct cpu_state {
        uint32_t   eax;
        uint32_t   ebx;
        uint32_t   ecx;
        uint32_t   edx;
        uint32_t   esi;
        uint32_t   edi;
        uint32_t   ebp;

        uint32_t   intr;
        uint32_t   error;

        uint32_t   eip;
        uint32_t   cs;
        uint32_t   eflags;
        uint32_t   esp;
        uint32_t   ss;
    };

    static uint32_t tss[32] = {0, 0, 0x10};
}

extern "C" inthandler::cpu_state *handle_int(struct inthandler::cpu_state *cpu);

#endif // INTHANDLER_H
