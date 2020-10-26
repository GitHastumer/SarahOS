#if !defined(GDT_H)
#define GDT_H

#include <stdint.h>
#include <stddef.h>

namespace gdt {
    constexpr int ENTRIES = 6;

    void init();

    void setEntry(int i, uint32_t base, uint32_t limit, int32_t flags);
}

extern "C" void setGDT(void*);
extern "C" void reloadSegments(void);

#endif // GDT_H
