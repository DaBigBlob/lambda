#ifndef _src_mem_h_
#define _src_mem_h_

#include "ast.h"

extern exp* new_exp(void);

extern exp* deep_dupe(exp* e);

extern void del_exp(exp* e);

extern void deep_del(exp* e);

#endif // _src_mem_h_
