#ifndef _src_need_
#define _src_need_

#include "pre.h"

extern void* __mem_alloc(ulong n);
extern void __mem_free(void* p);
extern void __mem_copy(void* d, const void* s, ulong n);

extern int __nstr_eq(const char* a, const char* b, ulong l);

#endif // _src_need_
