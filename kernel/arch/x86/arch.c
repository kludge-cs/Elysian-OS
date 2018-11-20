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
	char * buf = "--------";

	screen_init();

	color_puts(Magenta, White, " Booting Elysium... (Name may change later)                                     \n");

	if (mboot_magic_check == MBOOT_MAGIC)
		color_puts(Light_Green, Black, "Booted from a multiboot-compliant bootloader\n");
	else
		panic("Multiboot magic is not correct: %", mboot_magic_check);

	if ((unsigned int)mboot_info > VIRT(0x100000))
	{
		panic("Multiboot struct is too high: %!", PHYS(mboot_info));
	}

	if ((mboot_info->flags & (1<<6)) == 0)
	{
		panic("No multiboot memory map available");
	}

	puts("Multiboot info:");
	color_puts(Light_Grey, Black, "Lower conventional memory: 0x% KB",   tohex(mboot_info->mem_lower, buf));
	color_puts(Light_Grey, Black, "Upper conventional memory: 0x% KB\n", tohex(mboot_info->mem_upper, buf));

	puts("Initializing GDT...");
	install_gdt();
	color_puts(Light_Green, Black, "GDT initialized!\n");

	puts("Initializing IDT...");
	install_idt(0x08);
	color_puts(Light_Green, Black, "IDT initialized!\n");

	puts("Setting up PIC...");
	irq_install(0x08);
	color_puts(Light_Green, Black, "PIC initialized!\n");

	__asm__ volatile ("sti");

	puts("Setting up PIT...");
	timer_install(100);
	color_puts(Light_Green, Black, "PIT initialized!\n");
	return 0;
}