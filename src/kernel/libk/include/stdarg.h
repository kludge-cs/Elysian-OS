#pragma once

/*
 * Taken from libc11
 * see http://libc11.org/stdarg/
 */

__builtin_va_list typedef va_list;

#define va_start(ap, param) __builtin_va_start((ap), (param))

#define va_arg(ap, type)    __builtin_va_arg(ap, type)

#define va_end(ap)          __builtin_va_end((ap))

#define va_copy(dest, src)  __builtin_va_copy((dest), (src))