#include <types.h>
#include <idt.h>
#include <string.h>

void idt_add(uint8 num, uint32 offset, uint16 selector, uint8 flags)
{
	idt[num].offset_high = (offset & 0xFFFF);
	idt[num].offset_low = (offset >> 16) & 0xFFFF;
	idt[num].selector = selector;
	idt[num].type_attr = flags;
	idt[num].zero = 0;
}

void install_idt()
{
	idt_pointer.limit = (sizeof(struct idt_entry) * 256) - 1;
	idt_pointer.base = (uint32)&idt;

	/* Zero it out */
	memset(&idt, 0, sizeof(struct idt_entry) * 256);

	/* Load */
	asm volatile ("lidt idt_pointer");
}