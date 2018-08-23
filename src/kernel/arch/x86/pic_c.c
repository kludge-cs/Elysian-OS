#include <pic_c.h>
#include <screen.h>
#include <string.h>
#include <types.h>
#include <ports.h>


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
void *irq_routines[16] =
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

void __attribute__((__cdecl__)) irq_handler_main(struct regs_struct *regs)
{
	void (*handler)(struct regs_struct *regs);
	uint8 inum;
	inum = (uint8) regs->int_num;

	if (inum >= 40)
	{
		puts("IRQ: # >= 40");
		port_out(0xA0, 0x20); //send EOI to slave
	}
	
	handler = irq_routines[inum - 32];
	if (handler)
		handler(regs);
	port_out(0x20, 0x20); //send EOI to master
}