#include <types.h>
#include <_idt.h>
void irq_add (int irq, void (*handler)(struct regs_s *regs));
void irq_remove (int irq);
void irq_install (uint16_t selector);

extern void pic_init ();