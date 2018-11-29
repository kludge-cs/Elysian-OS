#include <paging.h>
#include <types.h>
#include <vmem.h>

#define TLB_FLUSH(addr) __asm__ volatile ("invlpg (%0)" :: "r" (virt) : "memory")

int8_t map_page_large (void * virt, struct pd_entry_s pd_entry)
{
	if (page_dir[(uint32_t)virt >> 22].present)
		return -1;
	/**/

	page_dir[(uint32_t)virt >> 22] = pd_entry;
	TLB_FLUSH(virt);
	return 0;
}