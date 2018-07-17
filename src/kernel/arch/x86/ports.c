unsigned char port_in(unsigned short port)
{
	unsigned char result;
	/*
		GCC inline assembly syntax:
			asm
			(
				code
				: output operands (output operand constraints are prefixed with =)
				: input operands
				: list of clobbered registers
			);
	*/
	
	asm volatile
	(
		"in %1, %0"
		: "=a" (result) // %1, a for EAX, AL, AX
		: "d" (port) // %0, N means a constant of 0-255, d for EDX, DX, DL
	);
	
	return result;
}

void port_out(unsigned short port, unsigned char data)
{
	asm volatile
	(
		"out %1, %0"
		: /* no output */
		: "d" (port) /* %1 */,
		  "a" (data) /* %0 */
	)
}