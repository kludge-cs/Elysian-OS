#pragma once
#define VIRT_BASE 0xC0000000
#define VIRT(addr) ((addr) + VIRT_BASE)
#define PHYS(addr) ((addr) - VIRT_BASE)