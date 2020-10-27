#if !defined(GDT_H)
#define GDT_H

#include <stdint.h>
#include <stddef.h>

static uint32_t tss[32] = {0, 0, 0x10};

namespace gdt {
    constexpr int ENTRIES = 6;

    void init();

    void setEntry(int i, uint32_t base, uint32_t limit, int32_t flags);
}

extern "C" void setGDT(void*);
extern "C" void reloadSegments(void);
extern "C" void reloadTaskRegister(uint16_t);

#endif // GDT_H
