#include <types.h>
#include <gdt.h>

/* * * * * * * * * * * * * * * * * * * * *
* GDT access field:                       *
*   7  6   5   4  3    0                  *
* +---+-----+----+------+                 *
* | P | DPL | DT | Type |                 *
* +---+-----+----+------+                 *
* P - Segment is present                  *
* DPL - Ring number                       *
* DT - Descriptor type                    *
* Type - Type                             *
* * * * * * * * * * * * * * * * * * * * * *

* * * * * * * * * * * * * * * * * * * * * *
* GDT highlimit_flags field:              *
*   7   6   5   4  3          0           *
* +---+---+---+---+------------+          *
* | G | D | 0 | A | Limit-high |          *
* +---+---+---+---+------------+          *
* G - Granularity (0 for byte, 1 for 4KB) *
* D - Operand size                        *
* A - Available for system                *
* * * * * * * * * * * * * * * * * * * * * */

extern void flush_gdt(); //defined in boot.asm

struct gdt_ptr_struct
{
   uint16 limit;               // The upper 16 bits of all selector limits.
   uint32 base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed));

struct gdt_entry
{
	uint16 limit_low;
	uint16 base_low;

	uint8  base_middle;
	uint8  access;
	uint8  highlimit_flags;
	
	uint8  base_high; 
} __attribute__((packed));

#define GDT_CODE 0x9A
#define GDT_DATA 0x92

#define GDT_GRANULARITY 1 << 3
#define GDT_OPSIZE      1 << 2
#define GDT_AVAILABLE   1

struct gdt_entry gdt[3];
struct gdt_ptr_struct gdt_pointer;

void gdt_add(int num, uint32 base, uint32 limit, uint8 access, uint8 flags)
{
	/* Set base address */
	gdt[num].base_low    = (base & 0xFFFF);
	gdt[num].base_middle = ((base >> 16) & 0xFF);
	gdt[num].base_high   = ((base >> 24) & 0xFF);

	/* Set limits */
	gdt[num].limit_low = (limit & 0xFFFF);

	/* Set granularity and top 4 bits of limit */
	gdt[num].highlimit_flags = ((limit >> 16) & 0x0F) | ((flags << 4) & 0xF0);

	/* Set access */
	gdt[num].access = access;
}

void install_gdt()
{
	gdt_pointer.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gdt_pointer.base = (uint32)&gdt;

	/* NULL */
	gdt_add(0, 0, 0, 0, 0);

	/* Let's cover everything, for now */
	gdt_add(1, 0, 0xFFFFF, GDT_CODE, GDT_GRANULARITY | GDT_OPSIZE);
	gdt_add(2, 0, 0xFFFFF, GDT_DATA, GDT_GRANULARITY | GDT_OPSIZE);

	/* Flush and update */
	flush_gdt();
}