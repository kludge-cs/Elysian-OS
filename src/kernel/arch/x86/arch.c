#include <types.h>
#include <screen.h>
#include <_gdt.h>
#include <_idt.h>
#include <_pic.h>
#include <time.h>
#include <console.h>
#include <_multiboot.h>


int arch_init()
{
	/*
	extern int *multiboot_magic_check;
	//extern struct multiboot_info_struct multiboot_info;
	*/
	/* Init screen */
	screen_init();
	clear_screen();

	/*if (multiboot_magic_check != 0x2BADB002)
	{
		puts("Invalid multiboot magic!");
		return -1;
	}*/

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

	puts("Setting up PIT...");
	timer_install(100);
	puts("PIT initialized!");
	return 0;
}