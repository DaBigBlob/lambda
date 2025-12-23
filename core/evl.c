#ifndef _src_evl_
#define _src_evl_

#include "evl.h"
#include "mem.h"

static char replace(const exp* n, exp** _b, exp* x) {
    exp* b = *_b;

    switch (b->T) {
        case (symT): {
            if (symeq(&n->sym, &b->sym) == 'e') {
                del_exp(b); // delete body (never used again)
                // this may happen any number of time
                *_b = deep_dupe(x);    // \n.n x
                return 'i';
            } else {
                // *_b = b;    // \n.b x
                return 'b';
            }
        } break;
        case (absT): {
            if (symeq(&n->sym, &b->abs.n->sym) == 'e') {
                // *_b = b;    // \n.(\n.b2) x
                return 'd';
            } else {
                // *_b = b;    // \n.(\m.b2) x
                return replace(n, &b->abs.b, x);
            }
        } break;
        case (appT): {
            // *_b = b;    // f x
            char rf = replace(n, &b->app.f, x);
            char rx = replace(n, &b->app.x, x);
            return (rf == 'i' || rx == 'i') ? 'i' : '^';
        } break;
        default: break;
    }

    return 'o'; // unexpected
}


extern char eval(exp** _e) {
    exp* e = *_e;

    if (e->T != appT) return 'a'; // not application -> no eval
    eval(&e->app.f); // need -> lazy eval
    exp* f = e->app.f;
    exp *x = e->app.x;

    if (f->T != absT) return 'f'; // f is not abstraction -> no eval
    exp *n = f->abs.n;
    exp *b = f->abs.b;

    if (n->T != symT) return 'n'; // n is not symbol (lambda calc single var arg)

    // have: n, b, x
    char ret = replace(n, &b, x);
    deep_del(x); // deep copied => safe to delete
    del_exp(f); // delete abs
    del_exp(n);
    del_exp(e); // delete app
    *_e = b;

    return ret;
}

#endif // _src_evl_
