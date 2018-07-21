#include <screen.h>
#include <gdt.h>
#include <idt.h>
void arch_init()
{
	puts("Initializing GDT...");
	install_gdt();

	puts("Initializing IDT...");
	install_idt(0x8);
}