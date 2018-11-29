#pragma once
#include <types.h>

#define MBOOT_MAGIC 0x2BADB002

extern uint32_t mboot_magic_check;
extern struct mboot_info_s * mboot_info;

struct __attribute__((packed))
mboot_info_s
{
	/* Required */
	uint32_t flags;

	/* Set if flags[0] is set */
	uint32_t mem_upper;
	uint32_t mem_lower;

	/* Set if flags[1] is set */
	uint32_t boot_device;
	/* Set if flags[2] is set */
	uint32_t cmdline;
	
	/* Boot modules, set if flags[3] is set */
	uint32_t modules_count;
	uint32_t modules_addr;

	/* ELF/aout symboles, set if flags[4] or flags[5] is set */
	uint32_t symbols[4];

	/* Memory map, set if flags[6] is set */
	uint32_t mmap_len;
	uint32_t mmap_addr;

	/* Set if flags[7] is set */
	uint32_t drives_len;
	uint32_t drives_addr;

	/* Set if flags[8] is set */
	uint32_t config_table;
	/* Set if flags[9] is set */
	uint32_t bootloader_name;
	/* Set if flags[10] is set */
	uint32_t apm_table;

	/* Set if flags[11] is set */
	uint32_t vbe_ctrl_info;
	uint32_t vbe_mode_info;
	uint32_t vbe_mode;
	uint32_t vbe_inferface_segment;
	uint32_t vbe_interface_off;
	uint32_t vbe_interface_len;

	/* Framebuffer, set if flags[12] is set */
	uint32_t fb_addr;
	uint32_t fb_pitch;
	uint32_t fb_width;
	uint32_t fb_height;
	uint32_t fb_bpp;
	uint32_t fb_type;
	uint32_t color_info;
};

struct __attribute__((packed))
mboot_mmap_s
{
	uint32_t size;
	uint64_t addr;
	uint64_t len;
	uint32_t type;
};