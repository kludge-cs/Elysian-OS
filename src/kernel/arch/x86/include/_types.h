#pragma once

struct regs_struct
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
	unsigned int int_num, err_code;
	unsigned int eip, cs, eflags, useresp, ss; /* Auto-pushed */ 
};