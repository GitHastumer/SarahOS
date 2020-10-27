#include "multitasking.h"

#include <stdint.h>

#include "../terminal/term.h"
#include "../keyboard/keyboard.h"
#include "../stdlib/string.h"
#include "../serial/serial.h"

static uint8_t stack0[4096];
static uint8_t stack1[4096];
static uint8_t stack2[4096];
static uint8_t userStack0[4096];
static uint8_t userStack1[4096];
static uint8_t userStack2[4096];

static int numTasks = 3;
static inthandler::cpu_state *taskStates[3];

void task0() {
    volatile uint16_t *buf = (uint16_t*) 0xb8000;
    uint16_t j = '-' | (term::WHITE | term::CYAN << 4) << 8;
    
    while (1) {
        // for (int k = 0; k < 40000; ++k);

        // buf[0 * 80 + 79] = j;

        // for (int k = 0; k < 40000; ++k);

        // buf[0 * 80 + 79] = (j & 0xff00) | '/';

        for (int k = 0; k < 40000; ++k);

        term::putc('A');
    }
}

void task1() {
    volatile uint16_t *buf = (uint16_t*) 0xb8000;
    uint16_t j = '|' | (term::WHITE | term::CYAN << 4) << 8;
    
    while (1) {
        // for (int k = 0; k < 40000; ++k);

        // buf[0 * 80 + 79] = j;

        // for (int k = 0; k < 40000; ++k);

        // buf[0 * 80 + 79] = (j & 0xff00) | '/';

        for (int k = 0; k < 40000; ++k);

        term::putc('B');
    }
}

void shell() {

    while (1) {

        for (int k = 0; k < 40000; ++k);
        term::putc('C');
    }

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
                term::printf("help                - Show this page.\n");
                term::printf("bda <offset> <size> - Read the BDA at offset <offset> with the size <size>.\n");
            } else if (!std::strcmp(argv[0], "bda")) {
                int offset = 0;
                int size = 0;

                for (int j = 0; argv[1][j] != '\0'; ++j) {
                    offset = offset * 10;
                    
                    switch (argv[1][j]) { // too lazy for a real conversion function rn.
                        case '0':
                            offset += 0;
                            break;
                        case '1':
                            offset += 1;
                            break;
                        case '2':
                            offset += 2;
                            break;
                        case '3':
                            offset += 3;
                            break;
                        case '4':
                            offset += 4;
                            break;
                        case '5':
                            offset += 5;
                            break;
                        case '6':
                            offset += 6;
                            break;
                        case '7':
                            offset += 7;
                            break;
                        case '8':
                            offset += 8;
                            break;
                        case '9':
                            offset += 9;
                            break;
                        case 'a':
                        case 'A':
                            offset += 10;
                            break;
                        case 'b':
                        case 'B':
                            offset += 11;
                            break;
                        case 'c':
                        case 'C':
                            offset += 12;
                            break;
                        case 'd':
                        case 'D':
                            offset += 13;
                            break;
                        case 'e':
                        case 'E':
                            offset += 14;
                            break;
                        case 'f':
                        case 'F':
                            offset += 15;
                            break;
                    }
                }

                for (int j = 0; argv[2][j] != '\0'; ++j) {
                    size = size * 10 + argv[2][j] - '0';
                }

                for (int j = 0; j < size; ++j) {
                    term::putbasedun(*((uint8_t*) 0x0400 + offset + j), 16);
                    term::putc(' ');
                }

                term::putc('\n');
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

        .intr = 0,
        .error = 0,

        .eip = (uint32_t) entry,
        .cs = 0x18 | 0x03,
        .eflags = 0x202,
        .esp = (uint32_t) userStack + 4096,
        .ss = 0x20 | 0x03
    };

    inthandler::cpu_state *state = (inthandler::cpu_state*) (stack + 4096 - sizeof(newState));
    *state = newState;

    serial::printf("Initialized task %x with stack address %x and user_stack %x.\n", entry, stack, userStack);

    return state;
}

void multitasking::init() {
    currentTask = -1;

    taskStates[0] = initTask(stack0, userStack0, (void*) task0);
    taskStates[1] = initTask(stack1, userStack1, (void*) task1);
    taskStates[2] = initTask(stack2, userStack2, (void*) shell);
}

inthandler::cpu_state *multitasking::schedule(inthandler::cpu_state *cpu) {
    if (currentTask >= 0)
        taskStates[currentTask] = cpu;
    
    currentTask++;
    currentTask %= numTasks;

    cpu = taskStates[currentTask];

    serial::printf("Scheduled task %i.\n", currentTask);

    return cpu;
}