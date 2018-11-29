#pragma once
#include <types.h>

void * memcpy( void * s1, const void * s2, int64_t n );

void * memset( void * dest, int32_t val, int64_t size );

void * memsetw(uint16_t * dest, uint16_t val, int64_t n );

char * reverse(char * str, uint32_t n);

char * tohex(uint32_t val, char * buf);