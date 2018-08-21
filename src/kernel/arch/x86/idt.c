#include <types.h>
#include <idt.h>
#include <string.h>
#include <screen.h>

#ifdef _DEBUG
//#include <panic.h>
#endif

void idt_add(uint8 num, uint8 flags, uint16 selector, uint32 offset)
{
	idt[num].offset_high = (offset & 0xFFFF);
	idt[num].offset_low = (offset >> 16) & 0xFFFF;
	idt[num].selector = selector;
	idt[num].type_attr = flags;
	idt[num].zero = 0;
}

extern void default_handler(void);

void install_idt(uint16 selector)
{
	int i;
	idt_pointer.limit = (sizeof(struct idt_entry) * 256) - 1;
	idt_pointer.base = (uint32)&idt;
	
	for (i = 0; i < 256; i++)
		idt_add(i, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &default_handler);

	/* Load */
	asm volatile ("lidt %0" :: "m"(idt_pointer));
}