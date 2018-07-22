#include <screen.h>
#include <gdt.h>
#include <idt.h>

extern void pic_init();

void arch_init()
{
	puts("Initializing GDT...");
	install_gdt();

	puts("Initializing IDT...");
	install_idt(0x8);
	pic_init();
	
	asm volatile ("sti");
}