#include "gdt.h"

#include "../terminal/term.h"
#include "../interrupts/inthandler.h"

#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS     0x09

#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0   0x00
#define GDT_FLAG_RING3   0x60
#define GDT_FLAG_PRESENT 0x80

#define GDT_FLAG_4K_GRAN 0x800
#define GDT_FLAG_32_BIT  0x400

static uint64_t _gdt[gdt::ENTRIES];

void gdt::setEntry(int i, uint32_t base, uint32_t limit, int32_t flags) {
    _gdt[i] = limit & 0xffffll;
    _gdt[i] |= (base & 0xffffffll) << 16;
    _gdt[i] |= (flags & 0xffll) << 40;
    _gdt[i] |= ((limit >> 16) & 0xfll) << 48;
    _gdt[i] |= ((flags >> 8) & 0xffll) << 52;
    _gdt[i] |= ((base >> 24) & 0xffll) << 56;
}

void gdt::init() {
    setEntry(0, 0, 0, 0);
    
    setEntry(1, 0, 0xfffff,
        GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT);

    setEntry(2, 0, 0xfffff,
        GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT);

    setEntry(3, 0, 0xfffff,
        GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_CODESEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3);

    setEntry(4, 0, 0xfffff,
        GDT_FLAG_SEGMENT | GDT_FLAG_32_BIT | GDT_FLAG_DATASEG | GDT_FLAG_4K_GRAN | GDT_FLAG_PRESENT | GDT_FLAG_RING3);

    setEntry(5, (uint32_t) inthandler::tss, sizeof(inthandler::tss),
        GDT_FLAG_TSS | GDT_FLAG_PRESENT | GDT_FLAG_RING3);

    struct gdt_ptr_struct {
        uint16_t limit;
        void *ptr;
    } __attribute__((packed));

    struct gdt_ptr_struct gdtp = {
        gdtp.limit = gdt::ENTRIES * 8 - 1,
        gdtp.ptr = _gdt
    };

    setGDT(&gdtp);
    reloadSegments();

    asm volatile("ltr %%ax" :: "a" (5 << 3));
}