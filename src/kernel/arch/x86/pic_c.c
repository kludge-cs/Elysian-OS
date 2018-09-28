#include <types.h>

#include <_pic.h>
#include <ports.h>
#include <_idt.h>



void irq_remove(int irq);

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void pic_init();

/* Array of func pointers */
static void *irq_routines[16] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void irq_add(int irq, void (*handler)(struct regs_struct *regs))
{
	irq_routines[irq] = handler;
}

void irq_remove(int irq)
{
	irq_routines[irq] = 0;
}

void __attribute__((__cdecl__))
irq_handler_main
(struct regs_struct *regs)
{
	void (*handler)(struct regs_struct *regs);
	uint8 inum;
	inum = (uint8) regs->int_num;
	handler = irq_routines[inum - 32];
	
	if (handler)
		handler(regs);

	if (inum >= 40)
		port_out(0xA0, 0x20); //send EOI to slave
	
	port_out(0x20, 0x20); //send EOI to master
}

void irq_install(uint16 selector)
{
	pic_init();
	
	/*   idt_add(uint8 num, uint8 flags, uint16 selector, uint32 offset);   */
	idt_add(32, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq0);
	idt_add(33, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq1);
	idt_add(34, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq2);
	idt_add(35, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq3);
	idt_add(36, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq4);
	idt_add(37, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq5);
	idt_add(38, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq6);
	idt_add(39, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq7);
	idt_add(40, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq8);
	idt_add(41, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq9);
	idt_add(42, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq10);
	idt_add(43, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq11);
	idt_add(44, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq12);
	idt_add(45, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq13);
	idt_add(46, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq14);
	idt_add(47, IDT_DESC_PRESENT | IDT_DESC_BITS_32, selector, (uint32) irq15);
}