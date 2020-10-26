#if !defined(PIC_H)
#define PIC_H

#include <stdint.h>
#include <stddef.h>

namespace pic {
    void init();
}

extern "C" void enableHardwareInterrupts(void);

#endif // PIC_H
