#include "idt.h"

#include "../terminal/term.h"
#include "../serial/serial.h"

static uint64_t _idt[idt::ENTRIES];

void handler(void) {
    term::printf("Non-implemented interrupt caught.\n");
}

void idt::setEntry(int id, uint64_t offset, uint16_t selector, uint8_t type_attr) {
    _idt[id] = offset & 0xffffll;
    _idt[id] |= (selector & 0xffffll) << 16;
    _idt[id] |= (type_attr & 0xffll) << 40;
    _idt[id] |= ((offset >> 16) & 0xffffll) << 48;

    serial::printf("IDT Entry %d: Offset %x, selector %x, Attributes %x\n", id, offset, selector, type_attr);
}

void idt::init() {
    setEntry(0, (uint64_t) int_stub_0, 0x08, 0xe | 0x80 | 0x0);
    setEntry(1, (uint64_t) int_stub_1, 0x08, 0xe | 0x80 | 0x0);
    setEntry(2, (uint64_t) int_stub_2, 0x08, 0xe | 0x80 | 0x0);
    setEntry(3, (uint64_t) int_stub_3, 0x08, 0xe | 0x80 | 0x0);
    setEntry(4, (uint64_t) int_stub_4, 0x08, 0xe | 0x80 | 0x0);
    setEntry(5, (uint64_t) int_stub_5, 0x08, 0xe | 0x80 | 0x0);
    setEntry(6, (uint64_t) int_stub_6, 0x08, 0xe | 0x80 | 0x0);
    setEntry(7, (uint64_t) int_stub_7, 0x08, 0xe | 0x80 | 0x0);
    setEntry(8, (uint64_t) int_stub_8, 0x08, 0xe | 0x80 | 0x0);
    setEntry(9, (uint64_t) int_stub_9, 0x08, 0xe | 0x80 | 0x0);
    setEntry(10, (uint64_t) int_stub_10, 0x08, 0xe | 0x80 | 0x0);
    setEntry(11, (uint64_t) int_stub_11, 0x08, 0xe | 0x80 | 0x0);
    setEntry(12, (uint64_t) int_stub_12, 0x08, 0xe | 0x80 | 0x0);
    setEntry(13, (uint64_t) int_stub_13, 0x08, 0xe | 0x80 | 0x0);
    setEntry(14, (uint64_t) int_stub_14, 0x08, 0xe | 0x80 | 0x0);
    setEntry(15, (uint64_t) int_stub_15, 0x08, 0xe | 0x80 | 0x0);
    setEntry(16, (uint64_t) int_stub_16, 0x08, 0xe | 0x80 | 0x0);
    setEntry(17, (uint64_t) int_stub_17, 0x08, 0xe | 0x80 | 0x0);
    setEntry(18, (uint64_t) int_stub_18, 0x08, 0xe | 0x80 | 0x0);
    setEntry(19, (uint64_t) int_stub_19, 0x08, 0xe | 0x80 | 0x0);
    setEntry(20, (uint64_t) int_stub_20, 0x08, 0xe | 0x80 | 0x0);
    setEntry(21, (uint64_t) int_stub_21, 0x08, 0xe | 0x80 | 0x0);
    setEntry(22, (uint64_t) int_stub_22, 0x08, 0xe | 0x80 | 0x0);
    setEntry(23, (uint64_t) int_stub_23, 0x08, 0xe | 0x80 | 0x0);
    setEntry(24, (uint64_t) int_stub_24, 0x08, 0xe | 0x80 | 0x0);
    setEntry(25, (uint64_t) int_stub_25, 0x08, 0xe | 0x80 | 0x0);
    setEntry(26, (uint64_t) int_stub_26, 0x08, 0xe | 0x80 | 0x0);
    setEntry(27, (uint64_t) int_stub_27, 0x08, 0xe | 0x80 | 0x0);
    setEntry(28, (uint64_t) int_stub_28, 0x08, 0xe | 0x80 | 0x0);
    setEntry(29, (uint64_t) int_stub_29, 0x08, 0xe | 0x80 | 0x0);
    setEntry(30, (uint64_t) int_stub_30, 0x08, 0xe | 0x80 | 0x0);
    setEntry(31, (uint64_t) int_stub_31, 0x08, 0xe | 0x80 | 0x0);
    setEntry(32, (uint64_t) int_stub_32, 0x08, 0xe | 0x80 | 0x0);
    setEntry(33, (uint64_t) int_stub_33, 0x08, 0xe | 0x80 | 0x0);
    setEntry(34, (uint64_t) int_stub_34, 0x08, 0xe | 0x80 | 0x0);
    setEntry(35, (uint64_t) int_stub_35, 0x08, 0xe | 0x80 | 0x0);
    setEntry(36, (uint64_t) int_stub_36, 0x08, 0xe | 0x80 | 0x0);
    setEntry(37, (uint64_t) int_stub_37, 0x08, 0xe | 0x80 | 0x0);
    setEntry(38, (uint64_t) int_stub_38, 0x08, 0xe | 0x80 | 0x0);
    setEntry(39, (uint64_t) int_stub_39, 0x08, 0xe | 0x80 | 0x0);
    setEntry(40, (uint64_t) int_stub_40, 0x08, 0xe | 0x80 | 0x0);
    setEntry(41, (uint64_t) int_stub_41, 0x08, 0xe | 0x80 | 0x0);
    setEntry(42, (uint64_t) int_stub_42, 0x08, 0xe | 0x80 | 0x0);
    setEntry(43, (uint64_t) int_stub_43, 0x08, 0xe | 0x80 | 0x0);
    setEntry(44, (uint64_t) int_stub_44, 0x08, 0xe | 0x80 | 0x0);
    setEntry(45, (uint64_t) int_stub_45, 0x08, 0xe | 0x80 | 0x0);
    setEntry(46, (uint64_t) int_stub_46, 0x08, 0xe | 0x80 | 0x0);
    setEntry(47, (uint64_t) int_stub_47, 0x08, 0xe | 0x80 | 0x0);
    setEntry(48, (uint64_t) int_stub_48, 0x08, 0xe | 0x80 | 0x0);
    setEntry(49, (uint64_t) int_stub_49, 0x08, 0xe | 0x80 | 0x0);
    setEntry(50, (uint64_t) int_stub_50, 0x08, 0xe | 0x80 | 0x0);
    setEntry(51, (uint64_t) int_stub_51, 0x08, 0xe | 0x80 | 0x0);
    setEntry(52, (uint64_t) int_stub_52, 0x08, 0xe | 0x80 | 0x0);
    setEntry(53, (uint64_t) int_stub_53, 0x08, 0xe | 0x80 | 0x0);
    setEntry(54, (uint64_t) int_stub_54, 0x08, 0xe | 0x80 | 0x0);
    setEntry(55, (uint64_t) int_stub_55, 0x08, 0xe | 0x80 | 0x0);
    setEntry(56, (uint64_t) int_stub_56, 0x08, 0xe | 0x80 | 0x0);
    setEntry(57, (uint64_t) int_stub_57, 0x08, 0xe | 0x80 | 0x0);
    setEntry(58, (uint64_t) int_stub_58, 0x08, 0xe | 0x80 | 0x0);
    setEntry(59, (uint64_t) int_stub_59, 0x08, 0xe | 0x80 | 0x0);
    setEntry(60, (uint64_t) int_stub_60, 0x08, 0xe | 0x80 | 0x0);

    for (int i = 61; i < 256; ++i)
        setEntry(i, (uint64_t) handler, 0x08, 0xe | 0x80 | 0x0); // interrupt gate, present flag, ring 0

    struct idt_ptr_struct {
        uint16_t limit;
        void *ptr;
    } __attribute__((packed));

    struct idt_ptr_struct idtp {
        idtp.limit = idt::ENTRIES * 8 - 1,
        idtp.ptr = _idt
    };

    setIDT(&idtp);

    serial::printf("Set the IDT.\n");
}