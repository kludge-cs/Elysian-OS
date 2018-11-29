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
#include <string.h>


int arch_init (void)
{
	struct mboot_mmap_s * mmap;

	screen_init();

	color_printf(Magenta, White, " Booting Elysium... (Name may change later)                                     \n");

	if (mboot_magic_check == MBOOT_MAGIC)
		color_printf(Light_Green, Black, "Booted from a multiboot-compliant bootloader\n");
	else
		panic("Multiboot magic is not correct: %x", (uint32_t) mboot_magic_check);
	/**/

	if ((unsigned int)mboot_info > VIRT(0x100000))
	{
		panic("Multiboot struct is too high: %x!", (uint32_t) PHYS(mboot_info));
	}

	if ((mboot_info->flags & (1<<6)) == 0)
	{
		panic("No multiboot memory map available");
	}


	printf("Multiboot info:");
	set_colors(Light_Grey, Black);
	printf("Lower conventional memory: 0x%x KB",   (uint32_t) mboot_info->mem_lower);
	printf("Upper conventional memory: 0x%x KB\n", (uint32_t) mboot_info->mem_upper);

	color_printf(White, Black, "Memory map:");
	mboot_info->mmap_addr += VIRT_BASE;
	mmap = (struct mboot_mmap_s *) mboot_info->mmap_addr;

	puts("  Addr                 Length               Type");

	for
		(
			;
			(uint32_t) mmap < mboot_info->mmap_addr + mboot_info->mmap_len;
			mmap = mmap + mmap->size + sizeof(mmap->size)
		)
	{
		printf
		(
			"| 0x%y | 0x%y | 0x%x |",
			(uint64_t) mmap->addr,
			(uint64_t) mmap->len,
			(uint32_t) mmap->type
		);

		putch('\n');
	}

	set_colors(White, Black);

	puts("Initializing GDT...");
	install_gdt();
	color_printf(Light_Green, Black, "GDT initialized!\n");

	puts("Initializing IDT...");
	install_idt(0x08);
	color_printf(Light_Green, Black, "IDT initialized!\n");

	puts("Setting up PIC...");
	irq_install(0x08);
	color_printf(Light_Green, Black, "PIC initialized!\n");

	__asm__ volatile ("sti");

	puts("Setting up PIT...");
	timer_install(100);
	color_printf(Light_Green, Black, "PIT initialized!\n");
	return 0;
}