#include <types.h>
#include <idt.h>
#include <string.h>
#include <screen.h>

#ifdef _DEBUG
//#include <panic.h>
#endif


extern void handler_wrapper();
extern void idt_load();

void idt_add(uint8 num, uint8 flags, uint16 selector, uint32 offset)
{
	idt[num].offset_low = (offset & 0xFFFF);
	idt[num].offset_high = (offset >> 16) & 0xFFFF;
	idt[num].selector = selector;
	idt[num].type_attr = flags;
	idt[num].zero = 0;
}


void install_idt(uint16 selector)
{
	int i;
	idt_pointer.limit = (sizeof(struct idt_entry) * 256) - 1;
	idt_pointer.base = (uint32)&idt;

	memset(&idt, 0, sizeof(struct idt_entry) * 256);
	for (i = 0; i < 256; i++)
		idt_add(i, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &handler_wrapper);

	/* Load */
	asm volatile ("lidt %0" : : "m" (idt_pointer));
}

void int_handler(void)
{
	clear_screen();
	puts("Uncaught exception!");
}