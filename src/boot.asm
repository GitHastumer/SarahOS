; constants for multiboot header
MBALIGN equ 1 << 0 ; align loaded modules on page boundaries
MEMINFO equ 1 << 1 ; provide memory map
FLAGS   equ MBALIGN | MEMINFO ; multiboot 'flag' field
MAGIC   equ 0x1badb002 ; magic number
CHECKSUM equ -(MAGIC + FLAGS) ; checksum to prove we're multiboot

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text

global setGDT
setGDT:
    mov eax, [esp + 4]
    lgdt [eax]
    ret

global reloadSegments
reloadSegments:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.rl
.rl:
    ret

global setIDT
setIDT:
    mov eax, [esp + 4]
    lidt [eax]
    ret

global enableHardwareInterrupts
enableHardwareInterrupts:
    sti
    ret

global executeSyscall
executeSyscall:
    int 0x30
    ret

global _start:function (_start.end - _start)
_start:
    mov esp, stack_top

    extern kernel_main
    call kernel_main
    cli
.hang:
    hlt
    jmp .hang
.end: