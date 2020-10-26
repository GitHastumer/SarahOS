#if !defined(MULTITASKING_H)
#define MULTITASKING_H

#include "../interrupts/inthandler.h"

namespace multitasking {
    inthandler::cpu_state *initTask(uint8_t *stack, uint8_t *userStack, void *entry);
    inthandler::cpu_state *schedule(inthandler::cpu_state *cpu);

    void init();
}

#endif // MULTITASKING_H
