#ifndef _parse_need_
#define _parse_need_

#include "../core/pre.h"

extern void __write_nstr(const char* p, ulong n);
extern void __write_cstr(const char* p);

extern int __nstr_eq(const char* a, const char* b, ulong l);

#endif // _parse_need_
