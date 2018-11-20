#include <types.h>
#include <time.h>
#include <ports.h>
#include <_pic.h>
#include <console.h>

volatile uint32 _ticks;
uint32 _freq;

/* Command register:
	+-----------+-----------+------+-----+
	| Counter # | R/W Order | Mode | BCD |
	+-----------+-----------+------+-----+

	Counter #: 0-2
	R/W Order: Read/Write mode, 1 is LSB, 2 is MSB, and 3 is LSB then MSB

	Mode:
		0 - 'Interrupt on terminal count'
		1 - 'Hardware Retriggerable one shot'
		2 - Rate generator
		3 - Square wave
		4 - Software strobe
		5 - Hardware strobe

	BCD:
		0 - 1x 16 bit counter
		1 - 4x BCD counters

	00_11_011_0 - Counter 0, LSB then MSB, Square wave generator, 1x 16 bit counter
*/

void set_interval(uint32 hz)
{
	uint32 num;
	_freq = hz;
	num = (uint32)1193180 / hz;
	port_out(0x43, 0x36); /* Set command register: 0x36=0b00110110 */
	port_out(0x40, num & 0xFF);    /* Set low byte */
	port_out(0x40, num >> 8);      /* Set high byte */
}

void timer_handler(struct regs_s *regs)
{
	_ticks++;
	/* puts("Tick"); */
}

void timer_install(uint32 hz)
{
	set_interval(hz);
	irq_add(0, timer_handler);
}

void delay(uint32 amount)
{
	uint32 eticks;
	eticks = amount + _ticks;
	while(_ticks < eticks)
	{
		__asm__ volatile ("hlt");
	}
}