#include <screen.h>
#include <gdt.h>
#include <idt.h>

extern void pic_init();

void arch_init()
{
	puts("Initializing GDT...");
	install_gdt();
	puts("GDT initialized!");

	//puts("Initializing IDT...");
	//install_idt(0x8);
	//puts("IDT initialized!");

	//puts("Setting up PIC...");
	//pic_init();
	//puts("PIC initialized!");
	
	//asm volatile ("sti");
	//asm volatile ("int $0");
}