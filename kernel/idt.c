#include <lib/types.h>
#include <lib/memory.h>
#include <kernel/idt.h>

struct idt_entry idt[IDT_TABLE_SIZE];
struct idt_ptr idtPointer;

void idt_set_gate(unsigned char index, uint32_t base, uint16_t selector, uint8_t flags)
{

    idt[index].baseLow = (base & 0xFFFF);
    idt[index].baseHigh = (base >> 16) & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].flags = flags;

}

void idt_init()
{

    idtPointer.base = (uint32_t)&idt;
    idtPointer.limit = (sizeof (struct idt_entry) * IDT_TABLE_SIZE) - 1;

    memory_set(&idt, 0, sizeof (struct idt_entry) * IDT_TABLE_SIZE);

    idt_flush(&idtPointer);

}

