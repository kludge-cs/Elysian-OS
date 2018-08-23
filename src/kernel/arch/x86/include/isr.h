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

struct regs_struct
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by pusha */
	unsigned int int_num, err_code;
	unsigned int eip, cs, eflags, useresp, ss;   /* auto-pushed */ 
};

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