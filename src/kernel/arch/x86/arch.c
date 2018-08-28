#include <screen.h>
#include <gdt.h>
#include <idt.h>
#include <pic_c.h>
#include <time.h>
#include <util.h>

struct multiboot_info_struct
{
	/* Required */
	uint32 flags;

	/* Set if flags[0] is set */
	uint32 mem_upper;
	uint32 mem_lower;

	/* Set if flags[1] is set */
	uint32 boot_device;
	/* Set if flags[2] is set */
	uint32 cmdline;
	
	/* Boot modules, set if flags[3] is set */
	uint32 modules_count;
	uint32 modules_addr;

	/* ELF/aout symboles, set if flags[4] or flags[5] is set */
	uint32 symbols[4];

	/* Memory map, set if flags[6] is set */
	uint32 mmap_len;
	uint32 mmap_addr;

	/* Set if flags[7] is set */
	uint32 drives_len;
	uint32 drives_addr;

	/* Set if flags[8] is set */
	uint32 config_table;
	/* Set if flags[9] is set */
	uint32 bootloader_name;
	/* Set if flags[10] is set */
	uint32 apm_table;

	/* Set if flags[11] is set */
	uint32 vbe_ctrl_info;
	uint32 vbe_mode_info;
	uint32 vbe_mode;
	uint32 vbe_inferface_segment;
	uint32 vbe_interface_off;
	uint32 vbe_interface_len;

	/* Framebuffer, set if flags[12] is set */
	uint32 fb_addr;
	uint32 fb_pitch;
	uint32 fb_width;
	uint32 fb_height;
	uint32 fb_bpp;
	uint32 fb_type;
	uint32 color_info;
} __attribute__((packed));


int arch_init(uint32 multiboot_magic, uint32 multiboot_addr)
{
	struct multiboot_info_struct *multiboot_info;
	
	/* Init screen */
	screen_init();
	clear_screen();

	if (multiboot_magic != 0x2BADB002)
	{
		puts("Invalid multiboot magic!");
		return -1;
	}

	multiboot_info = (struct multiboot_info_struct *) multiboot_addr;

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