#pragma once
#include <types.h>

#define MBOOT_MAGIC 0x2BADB002

extern uint32 mboot_magic_check;
extern struct mboot_info_s * mboot_info;

struct __attribute__((packed))
mboot_info_s
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
};

struct __attribute__((packed))
mboot_mmap_s
{
	uint32 size;
	uint64 base_addr; 
	uint64 len;
	uint32 type;
};