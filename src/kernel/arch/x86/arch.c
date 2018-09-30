#include <types.h>
#include <screen.h>
#include <_gdt.h>
#include <_idt.h>
#include <_pic.h>
#include <time.h>
#include <console.h>
#include <_multiboot.h>


int arch_init (void)
{
	screen_init();
	clear_screen();

	color_puts(" Booting Project Mira... (Name will be changed later) \n", Magenta, White);

	puts("Initializing GDT...");
	install_gdt();
	color_puts("GDT initialized!\n", Light_Green, Black);

	puts("Initializing IDT...");
	install_idt(0x08);
	color_puts("IDT initialized!\n", Light_Green, Black);

	puts("Setting up PIC...");
	irq_install(0x08);
	color_puts("PIC initialized!\n", Light_Green, Black);

	asm volatile ("sti");

	puts("Setting up PIT...");
	timer_install(100);
	color_puts("PIT initialized!\n", Light_Green, Black);
	return 0;
}