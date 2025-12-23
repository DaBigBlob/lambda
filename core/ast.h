#ifndef _src_ast_h_
#define _src_ast_h_

#include "pre.h"

#define expT char
enum __expT{
    absT = 'b',
    appT = 'p',
    symT = 'y',
    invT = 'i'
};

typedef struct _sym {
    const char* p;
    ulong l;
} sym;

typedef struct _abs {
    struct _exp* n;
    struct _exp* b;
} abs;

typedef struct _app {
    struct _exp* f;
    struct _exp* x;
} app;

typedef struct _exp {
    union {
        sym sym;
        abs abs;
        app app;
    };
    expT T;
} exp;

extern char symeq(const sym* a, const sym* b);

#define fil_sym(sm, _p, _l)\
{\
    sm->T = symT;\
    sm->sym.p = _p;\
    sm->sym.l = _l;\
}

#define fil_app(ap, _f, _x)\
{\
    ap->T = appT;\
    ap->app.f = _f;\
    ap->app.x = _x;\
}

#define fil_abs(ab, _n, _b)\
{\
    ab->T = absT;\
    ab->abs.n = _n;\
    ab->abs.b = _b;\
}

#endif // _src_ast_h_
