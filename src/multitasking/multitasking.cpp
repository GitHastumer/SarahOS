#include "multitasking.h"

#include <stdint.h>

#include "../terminal/term.h"
#include "../keyboard/keyboard.h"
#include "../stdlib/string.h"

static uint8_t stack0[4096];
static uint8_t stack1[4096];
//static uint8_t stack2[4096];
static uint8_t userStack0[4096];
static uint8_t userStack1[4096];
//static uint8_t userStack2[4096];

static int currentTask = -1;
static int numTasks = 2;
static inthandler::cpu_state *taskStates[2];

void task0() {
    volatile uint16_t *buf = (uint16_t*) 0xb8000;
    uint16_t j = 'A' | (8 | 13 << 4) << 8;

    while (1) {
        asm volatile("cli; hlt");
        for (int i = 0; i < 1000000; ++i);

        buf[20 * 80 + 10] = j;

        term::puts("A");
    }
}

void task1() {
    volatile uint16_t *buf = (uint16_t*) 0xb8000;
    uint16_t j = 'B' | (8 | 13 << 4) << 8;
    
    while (1) {
    asm("cli; hlt");
        for (int i = 0; i < 1000000; ++i);

        buf[20 * 80 + 10] = j;

        //term::puts("B");
    }
}

void shell() {
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

inthandler::cpu_state *multitasking::initTask(uint8_t *stack, uint8_t *userStack, void *entry) {
    inthandler::cpu_state newState = {
        .eax = 0,
        .ebx = 0,
        .ecx = 0,
        .edx = 0,
        .esi = 0,
        .edi = 0,
        .ebp = 0,
        .eip = (uint32_t) entry,

        .cs = 0x18 | 0x03,

        .eflags = 0x202,
        .esp = (uint32_t) userStack + 4096,
        .ss = 0x20 | 0x03
    };

    inthandler::cpu_state *state = (inthandler::cpu_state*) (stack + 4096 - sizeof(newState));
    *state = newState;

    return state;
}

void multitasking::init() {
    taskStates[0] = initTask(stack0, userStack0, (void*) task0);
    taskStates[1] = initTask(stack1, userStack1, (void*) task1);
    //taskStates[2] = initTask(stack2, userStack2, (void*) shell);
}

inthandler::cpu_state *multitasking::schedule(inthandler::cpu_state *cpu) {
    if (currentTask >= 0)
        taskStates[currentTask] = cpu;
    
    currentTask++;
    currentTask %= numTasks;

    cpu = taskStates[currentTask];

    return cpu;
}