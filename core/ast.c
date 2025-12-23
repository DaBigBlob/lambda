#ifndef _src_ast_
#define _src_ast_

#include "ast.h"
#include "need.h"

extern char symeq(const sym* a, const sym* b) {
    if (a->l != b->l) return 'l';
    return
    (__nstr_eq(a->p, b->p, a->l) == 0)
        ? 'e'
        : 'n'
    ;
}

#endif // _src_ast_
