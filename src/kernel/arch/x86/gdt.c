#include <types.h>
#include <gdt.h>

/* * * * * * * * * * * * * * * * * * * * *
* GDT access field:                       *
*   7  6   5   4  3    0                  *
* +---+-----+----+------+                 *
* | P | DPL | DT | Type |                 *
* +---+-----+----+------+                 *
* P - segment is present                  *
* DPL - Ring number                       *
* DT - Descriptor type                    *
* Type - Type                             *
*******************************************
* GDT granularity field:                  *
*   7   6   5   4  3      0               *
* +---+---+---+---+--------+              *
* | G | D | 0 | A | Length |              *
* +---+---+---+---+--------+              *
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
	uint8  granularity;
	uint8  base_high; 
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr_struct gdt_pointer;

void gdt_add(int num, uint64 base, uint64 limit, uint8 access, uint8 granularity)
{
	/* Set base address */
	gdt[num].base_low    = (base & 0xFFFF); //upper 16 bits
	gdt[num].base_middle = ((base >> 16) & 0xFF); //the 8 bits after the 16
	gdt[num].base_high   = ((base >> 24) & 0xFF); //16+8=24, is the 8 bits after that

	/* Set limits */
	gdt[num].limit_low = (limit & 0xFFFF); //upper 16 again

	/* Set granularity */
	gdt[num].granularity =  ((limit >> 16) & 0x0F);
	
	gdt[num].granularity |= (granularity & 0xF0);

	/* Set access */
	gdt[num].access = access;
}

void install_gdt()
{
	gdt_pointer.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gdt_pointer.base = (uint32)&gdt;

	/* NULL */
	gdt_add(0, 0, 0, 0, 0);
	
	/* Let's cover everything, at least for now */
	/* Code */
	
	gdt_add(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	
	/* Data */
	gdt_add(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	/* Flush and update */
	flush_gdt();
}