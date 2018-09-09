typedef signed char   int8;
typedef unsigned char uint8;

typedef short          int16;
typedef unsigned short uint16;

typedef int      int32;
typedef unsigned uint32;

typedef long long int          int64;
typedef unsigned long long int uint64;

struct regs_struct
{
	unsigned int gs, fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
	unsigned int int_num, err_code;
	unsigned int eip, cs, eflags, useresp, ss; /* Auto-pushed */ 
};