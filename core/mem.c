#ifndef _src_mem_
#define _src_mem_

#include "mem.h"
#include "need.h"

// TODO: arena alloc

extern exp* new_exp(void) {
    void* e = __mem_alloc(sizeof(exp));
    if (e) ((exp*)e)->T = invT;
    return e;
}

// stack may overflow
extern exp* deep_dupe(exp* e) {
    exp* w = new_exp();
    switch (e->T) {
        case(appT): {
            w->T = appT;
            w->app.f = deep_dupe(e->app.f);
            w->app.x = deep_dupe(e->app.x);
        } break;
        case(absT): {
            w->T = absT;
            w->abs.b = deep_dupe(e->abs.b);
            w->abs.n = deep_dupe(e->abs.n);
        } break;
        default: {
            __mem_copy(w, e, sizeof(exp));
        } break;
    }
    return w;
}

extern void del_exp(exp* e) {
    if (e) {
        ((exp*)e)->T = invT;
        __mem_free(e);
    }
}

extern void deep_del(exp* e) {
    switch (e->T) {
        case(appT): {
            deep_del(e->app.f);
            deep_del(e->app.x);

        } break;
        case(absT): {
            deep_del(e->abs.b);
            deep_del(e->abs.n);
        } break;
        default: break;
    }
    del_exp(e);
}

#endif // _src_mem_
