#include <screen.h>
#include <gdt.h>
#include <idt.h>
#include <pic_c.h>


void arch_init()
{
	puts("Initializing GDT...");
	install_gdt();
	puts("GDT initialized!");

	puts("Initializing IDT...");
	install_idt(0x08);
	puts("IDT initialized!");

	puts("Setting up PIC...");
	irq_install(0x08);
	puts("PIC initialized!");
	
	asm volatile ("sti");
}