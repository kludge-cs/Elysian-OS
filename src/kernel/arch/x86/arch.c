#include <_gdt.h>
#include <_idt.h>
#include <_pic.h>
#include <_multiboot.h>

#include <types.h>
#include <time.h>
#include <vmem.h>

#include <panic.h>
#include <console.h>
#include <screen.h>
#include <strings.h>


int arch_init (void)
{
	screen_init();

	color_puts(" Booting Elysian... (Name may change later)                                     \n", Magenta, White);

	if (multiboot_magic_check == MBOOT_MAGIC)
		color_puts("Booted from a multiboot-compliant bootloader\n", Light_Green, Black);
	else
		panic("Multiboot magic is not correct:", multiboot_magic_check);

	mboot_info = (struct mboot_info_s *) VIRT(multiboot_info_ptr);

	puts("Initializing GDT...");
	install_gdt();
	color_puts("GDT initialized!\n", Light_Green, Black);

	puts("Initializing IDT...");
	install_idt(0x08);
	color_puts("IDT initialized!\n", Light_Green, Black);

	puts("Setting up PIC...");
	irq_install(0x08);
	color_puts("PIC initialized!\n", Light_Green, Black);

	__asm__ volatile ("sti");

	puts("Setting up PIT...");
	timer_install(100);
	color_puts("PIT initialized!\n", Light_Green, Black);
	return 0;
}