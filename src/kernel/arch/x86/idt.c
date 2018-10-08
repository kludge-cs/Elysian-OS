#include <types.h>
#include <_idt.h>
#include <strings.h>
#include <console.h>
#include <screen.h>
#include <panic.h>

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
	uint8  zero;
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
	"EXCEPTION: Reserved - #29",
	"EXCEPTION: Reserved - #30",
	"EXCEPTION: Reserved - #31"
};

void idt_add (uint8 num, uint8 flags, uint16 selector, uint32 offset)
{
	idt[num].offset_low = (offset & 0xFFFF);
	idt[num].offset_high = (offset >> 16) & 0xFFFF;
	idt[num].selector = selector;
	idt[num].type_attr = flags;
	idt[num].zero = 0;
}


void install_idt (uint16 selector)
{
	idt_pointer.limit = (sizeof(struct idt_entry) * 256) - 1;
	idt_pointer.base = (uint32)&idt;

	memset(&idt, 0, sizeof(struct idt_entry) * 256);
	
#define FILL_GATE(n) idt_add(n,  IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) &isr##n);
	FILL_GATE(0)
	FILL_GATE(1)
	FILL_GATE(2)
	FILL_GATE(3)
	FILL_GATE(4)
	FILL_GATE(5)
	FILL_GATE(6)
	FILL_GATE(7)
	FILL_GATE(8)
	FILL_GATE(9)
	FILL_GATE(10)
	FILL_GATE(11)
	FILL_GATE(12)
	FILL_GATE(13)
	FILL_GATE(14)
	FILL_GATE(15)
	FILL_GATE(16)
	FILL_GATE(17)
	FILL_GATE(18)
	FILL_GATE(19)
	FILL_GATE(20)
	FILL_GATE(21)
	FILL_GATE(22)
	FILL_GATE(23)
	FILL_GATE(24)
	FILL_GATE(25)
	FILL_GATE(26)
	FILL_GATE(27)
	FILL_GATE(28)
	FILL_GATE(29)
	FILL_GATE(30)
	FILL_GATE(31)
#undef fillgate

	/* Load */
	__asm__ volatile ("lidt %0" : : "m" (idt_pointer));
}

__attribute__((__cdecl__))
void int_handler (struct regs_s *regs)
{
	char buf1[9] = "--------";
	char buf2[9] = "--------";
	char buf3[9] = "--------";
	char buf4[9] = "--------";

	puts("\nEXCEPTION!\nRegisters:");
	puts("GS:  0x%     FS:  0x%     ES:  0x%     DS:  0x%\n", tohex(regs->gs, buf1), tohex(regs->fs, buf2), tohex(regs->es, buf3), tohex(regs->ds, buf4));

	puts("EDI: 0x%     ESI: 0x%", tohex(regs->edi, buf1), tohex(regs->esi, buf2));
	puts("EBP: 0x%     ESP: 0x%\n", tohex(regs->ebp, buf1), tohex(regs->esp, buf2));

	puts("EAX: 0x%     EBX: 0x%\nECX: 0x%     EDX: 0x%", tohex(regs->eax, buf1), tohex(regs->ebx, buf2), tohex(regs->ecx, buf3), tohex(regs->edx, buf4));

	puts("EIP: 0x%     ESP: 0x% (user)\n", tohex(regs->eip, buf1), tohex(regs->useresp, buf2));
	puts("EFLAGS: 0x%  CS:  0x%      SS:  0x%\n", tohex(regs->eflags, buf1), tohex(regs->cs, buf2), tohex(regs->ss, buf3));

	puts("INTERRUPT NUMBER: 0x%", tohex(regs->int_num, buf1));
	if (regs->int_num < 32)
	{
		puts(exception_msg[regs->int_num]);
		panic("Excpetion!");
	}
}