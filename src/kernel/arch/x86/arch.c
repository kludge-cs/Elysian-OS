#include <screen.h>
#include <gdt.h>
#include <idt.h>

extern void pic_init();

void arch_init()
{
	asm volatile ("xchgw %bx, %bx"); // so we can turn on tracing when we need
	puts("Initializing GDT...");
	install_gdt();
	puts("GDT initialized!");

	puts("Initializing IDT...");
	install_idt(0x08);
	puts("IDT initialized!");

	//puts("Setting up PIC...");
	//pic_init();
	//puts("PIC initialized!");
	
	//asm volatile ("sti");
}