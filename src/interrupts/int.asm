%macro int_stub 1
global int_stub_%1
int_stub_%1:
    push 0
    push %1
    jmp int_common_handler
%endmacro

%macro int_stub_errorcode 1
global int_stub_%1
int_stub_%1:
    push %1
    jmp int_common_handler
%endmacro

int_stub 0
int_stub 1
int_stub 2
int_stub 3
int_stub 4
int_stub 5
int_stub 6
int_stub 7
int_stub_errorcode 8
int_stub 9
int_stub_errorcode 10
int_stub_errorcode 11
int_stub_errorcode 12
int_stub_errorcode 13
int_stub_errorcode 14
int_stub 15
int_stub 16
int_stub_errorcode 17
int_stub 18
int_stub 19
int_stub 20
int_stub 21
int_stub 22
int_stub 23
int_stub 24
int_stub 25
int_stub 26
int_stub 27
int_stub 28
int_stub 29
int_stub 30
int_stub 31
int_stub 32
int_stub 33
int_stub 34
int_stub 35
int_stub 36
int_stub 37
int_stub 38
int_stub 39
int_stub 40
int_stub 41
int_stub 42
int_stub 43
int_stub 44
int_stub 45
int_stub 46
int_stub 47
int_stub 48
int_stub 49
int_stub 50
int_stub 51
int_stub 52
int_stub 53
int_stub 54
int_stub 55
int_stub 56
int_stub 57
int_stub 58
int_stub 59
int_stub 60
int_stub 61
int_stub 62
int_stub 63
int_stub 64
int_stub 65
int_stub 66
int_stub 67
int_stub 68
int_stub 69
int_stub 70
int_stub 71
int_stub 72
int_stub 73
int_stub 74
int_stub 75
int_stub 76
int_stub 77
int_stub 78
int_stub 79
int_stub 80

extern handle_int
int_common_handler:
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax

    push esp
    call handle_int
    mov esp, eax

    mov ax, 0x23
    mov ds, ax
    mov es, ax

    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp

    add esp, 8

    iret