#if !defined(IDT_H)
#define IDT_H

#include <stdint.h>
#include <stddef.h>

namespace idt {
    constexpr int ENTRIES = 256;

    void init();

    void setEntry(int id, uint64_t offset, uint16_t selector, uint8_t type_attr);
}

extern "C" void setIDT(void*);

extern "C" void int_stub_0(void);
extern "C" void int_stub_1(void);
extern "C" void int_stub_2(void);
extern "C" void int_stub_3(void);
extern "C" void int_stub_4(void);
extern "C" void int_stub_5(void);
extern "C" void int_stub_6(void);
extern "C" void int_stub_7(void);
extern "C" void int_stub_8(void);
extern "C" void int_stub_9(void);
extern "C" void int_stub_10(void);
extern "C" void int_stub_11(void);
extern "C" void int_stub_12(void);
extern "C" void int_stub_13(void);
extern "C" void int_stub_14(void);
extern "C" void int_stub_15(void);
extern "C" void int_stub_16(void);
extern "C" void int_stub_17(void);
extern "C" void int_stub_18(void);
extern "C" void int_stub_19(void);
extern "C" void int_stub_20(void);
extern "C" void int_stub_21(void);
extern "C" void int_stub_22(void);
extern "C" void int_stub_23(void);
extern "C" void int_stub_24(void);
extern "C" void int_stub_25(void);
extern "C" void int_stub_26(void);
extern "C" void int_stub_27(void);
extern "C" void int_stub_28(void);
extern "C" void int_stub_29(void);
extern "C" void int_stub_30(void);
extern "C" void int_stub_31(void);
extern "C" void int_stub_32(void);
extern "C" void int_stub_33(void);
extern "C" void int_stub_34(void);
extern "C" void int_stub_35(void);
extern "C" void int_stub_36(void);
extern "C" void int_stub_37(void);
extern "C" void int_stub_38(void);
extern "C" void int_stub_39(void);
extern "C" void int_stub_40(void);
extern "C" void int_stub_41(void);
extern "C" void int_stub_42(void);
extern "C" void int_stub_43(void);
extern "C" void int_stub_44(void);
extern "C" void int_stub_45(void);
extern "C" void int_stub_46(void);
extern "C" void int_stub_47(void);
extern "C" void int_stub_48(void);
extern "C" void int_stub_49(void);
extern "C" void int_stub_50(void);
extern "C" void int_stub_51(void);
extern "C" void int_stub_52(void);
extern "C" void int_stub_53(void);
extern "C" void int_stub_54(void);
extern "C" void int_stub_55(void);
extern "C" void int_stub_56(void);
extern "C" void int_stub_57(void);
extern "C" void int_stub_58(void);
extern "C" void int_stub_59(void);
extern "C" void int_stub_60(void);

#endif // IDT_H
