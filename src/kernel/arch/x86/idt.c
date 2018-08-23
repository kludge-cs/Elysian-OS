#include <types.h>
#include <idt.h>
#include <string.h>
#include <screen.h>

#ifdef _DEBUG
//#include <panic.h>
#endif

#define IDT_DESC_BITS_16    0x06    //00000110
#define IDT_DESC_BITS_32    0x0E    //00001110
#define IDT_DESC_RING1      0x40    //01000000
#define IDT_DESC_RING2      0x20    //00100000
#define IDT_DESC_RING3      0x60    //01100000
#define IDT_DESC_PRESENT    0x80    //10000000

/* * * * * * * * * * * * * * *
* Type_attr is as follows:    *
*   7  6   5  4  3         0  *
* +---+-----+---+-----------+ *
* | P | DPL | S | Gate Type | *
* +---+-----+---+-----------+ *
*                             *
* P - Segment is present      *
* DPL - Ring number           *
* S - Storage segment         *
* * * * * * * * * * * * * * * */

struct idt_entry
{
	uint16 offset_low;
	uint16 selector;
	uint8  zero; //unused
	uint8  type_attr;
	uint16 offset_high;
} __attribute__((packed));

struct idt_pointer_struct
{
	uint16 limit;
	uint32 base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_pointer_struct idt_pointer;

extern void idt_load();
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

char *exception_msg[] =
{
	"EXCEPTION: Division By Zero - #0",
	"EXCEPTION: Debug - #1",
	"EXCEPTION: Non Maskable Interrupt - #2",
	"EXCEPTION: Breakpoint - #3",
	"EXCEPTION: Into Detected Overflow - #4",
	"EXCEPTION: Out of bounds - #5",
	"EXCEPTION: Invalid opcode - #6",
	"EXCEPTION: No Coprocessor - #7",
	"EXCEPTION: Double fault - #8",
	"EXCEPTION: Coprocessor segment overrun - #9",
	"EXCEPTION: Bad TSS - #10",
	"EXCEPTION: Segment not present - #11",
	"EXCEPTOIN: Stack Fault  - #12",
	"EXCEPTION: General protection fault - #13",
	"EXCEPTION: Page fault - #14",
	"EXCEPTION: Unknown interrupt - #15",
	"EXCEPTION: Coprocessor fault - #16",
	"EXCEPTION: Alignment check - #17",
	"EXCEPTION: Machine check - #18",
	"EXCEPTION: Reserved - #19",
	"EXCEPTION: Reserved - #20",
	"EXCEPTION: Reserved - #21",
	"EXCEPTION: Reserved - #22",
	"EXCEPTION: Reserved - #23",
	"EXCEPTION: Reserved - #24",
	"EXCEPTION: Reserved - #25",
	"EXCEPTION: Reserved - #26",
	"EXCEPTION: Reserved - #27",
	"EXCEPTION: Reserved - #28",
	"EXCEPTION: Reserved - #30",
	"EXCEPTION: Reserved - #31"
};

void idt_add(uint8 num, uint8 flags, uint16 selector, uint32 offset)
{
	idt[num].offset_low = (offset & 0xFFFF);
	idt[num].offset_high = (offset >> 16) & 0xFFFF;
	idt[num].selector = selector;
	idt[num].type_attr = flags;
	idt[num].zero = 0;
}


void install_idt(uint16 selector)
{
	idt_pointer.limit = (sizeof(struct idt_entry) * 256) - 1;
	idt_pointer.base = (uint32)&idt;

	memset(&idt, 0, sizeof(struct idt_entry) * 256);
	idt_add(0,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr0);
	idt_add(1,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr1);
	idt_add(2,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr2);
	idt_add(3,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr3);
	idt_add(4,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr4);
	idt_add(5,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr5);
	idt_add(6,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr6);
	idt_add(7,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr7);
	idt_add(8,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr8);
	idt_add(9,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr9);
	idt_add(10, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr10);
	idt_add(11, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr11);
	idt_add(12, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr12);
	idt_add(13, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr13);
	idt_add(14, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr14);
	idt_add(15, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr15);
	idt_add(16, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr16);
	idt_add(17, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr17);
	idt_add(18, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr18);
	idt_add(19, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr19);
	idt_add(20, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr20);
	idt_add(21, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr21);
	idt_add(22, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr22);
	idt_add(23, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr23);
	idt_add(24, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr24);
	idt_add(25, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr25);
	idt_add(26, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr26);
	idt_add(27, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr27);
	idt_add(28, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr28);
	idt_add(29, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr29);
	idt_add(30, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr30);
	idt_add(31, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr31);

	/* Load */
	asm volatile ("lidt %0" : : "m" (idt_pointer));
}

void __attribute__((__cdecl__)) int_handler(struct regs_struct *regs)
{
	if (regs->int_num > 31)
	{
		clear_screen();
		puts("===============================================================");
		puts("                        KERNEL PANIC!                          ");
		puts("===============================================================");
		puts("Reason: Unknown exception!");
		puts("Debugger unimplemented!");
		puts("System halted.");
		while(1);
	}
	else
	{
		puts(exception_msg[regs->int_num]);
	}
}