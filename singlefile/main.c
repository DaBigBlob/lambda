#define abs stdlib_abs
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef abs

/* automatically complies with BSD-2 license */
__asm__(".ident\t\"\\n\\n\
Lambda (C impl) (BSD-2 License)\\n\
Copyright (c) 2025 Hans S. <hans@0x8.io>\"");

typedef unsigned long ulong;

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

static char symeq(const sym* a, const sym* b) {
    if (a->l != b->l) return 'l';
    return
    (strncmp(a->p, b->p, a->l) == 0)
        ? 'e'
        : 'n'
    ;
}

static exp* new_exp(void) {
    void* e = malloc(sizeof(exp));
    if (e) ((exp*)e)->T = invT;
    return e;
}

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

// stack may overflow
static exp* deep_dupe(exp* e) {
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
            memcpy(w, e, sizeof(exp));
        } break;
    }
    return w;
}

static void del_exp(exp* e) {
    if (e) {
        ((exp*)e)->T = invT;
        free(e);
    }
}

static void deep_del(exp* e) {
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


static char eval(exp** _e) {
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

#define fwrite(...) {fprintf(stdout, __VA_ARGS__); fflush(stdout);}
#define lwrite(p, l) fwrite("%.*s", (int)(l), (p))
#define pwrite(p) fwrite("%s", (p))

static void write_exp(exp* e) {
    if (!e) {
        pwrite("<NULL>")
        return;
    }

    switch (e->T) {
        case (symT): {
            lwrite(e->sym.p, e->sym.l);
        } break;
        case (appT): {
            pwrite("(app ")
            write_exp(e->app.f);
            pwrite(" ")
            write_exp(e->app.x);
            pwrite(")")
        } break;
        case (absT): {
            pwrite("(abs ")
            write_exp(e->abs.n);
            pwrite(" ")
            write_exp(e->abs.b);
            pwrite(")")
        } break;
        default: {
            pwrite("<INVAL>")
        } break;
    }
}

/* use when *p == '(' */
static char* match_b(char* p, char* d) {
    ++p;
    ulong b = 1;
    for (; (p < d) && b; ++p) {
        if (*p == '(') ++b; else
        if (*p == ')') --b;
    }

    return b ? NULL : p;
}

static char get_car(char** _p, char** _d) {
    char* p = *_p;
    char* d = *_d;

    // remove space prefix
    for (; (p < d) && (*p == ' ') ; ++p);

    if (d-p < 1) return 'w';

    if (*p == '(') {
        char* e = match_b(p, d);
        if (!e) return 'i';
        *_p = p;
        *_d = e;
        return 's';
    } else {
        char* e = p;
        for (; (e < d) && (*e != ' ') && (*e != '(') && (*e != ')'); e++);
        *_p = p;
        *_d = e;
        return 'a';
    }

    return 'o';
}
static char parse(exp** _e, char* p, char* d) {
    // pwrite("IN PARSER:`") lwrite(p, d-p) pwrite("`\n");
    char t = get_car(&p, &d);

    if (t == 'a') {
        exp* e = new_exp();
        fil_sym(e, p, d-p);
        *_e = e;
        return 'a';
    }

    if (t == 's') {
        ++p; --d; // trim parenthesis
        char *fp = p, *fd = d;

        expT T;

        if (get_car(&fp, &fd) == 'a') {
            if (fd-fp == 3) {
                #define streq(nm) (strncmp(nm, fp, 3) == 0)
                if (streq("app")) T = appT; else
                if (streq("abs")) T = absT;
                else T = symT;
                #undef streq
            } else T = symT;
        } else T = symT;

        exp* e = new_exp();
        // deal with atomic sexp
        if (T == symT) {
            fil_sym(e, p-1, d-p+2);
            *_e = e;
            return 'A';
        }

        // deal with proper
        char *ap = fd, *ad = d;
        char at = get_car(&ap, &ad);
        if (at != 'a' && at != 's') return '!';

        char r1 = 0, r2 = 0;
        switch (T) {
            case (absT): {
                fil_abs(e, NULL, NULL)
                r1 = parse(&e->abs.n, ap, ad);
                r2 = parse(&e->abs.b, ad, d);
            } break;
            case (appT): {
                fil_app(e, NULL, NULL)
                r1 = parse(&e->app.f, ap, ad);
                r2 = parse(&e->app.x, ad, d);
            } break;
            default: break; // unreachable
        }
        *_e = e;

        return (r1 == '!' || r2 == '!') ? '!' : '^';;
    }

    return '!';
}


int main(int ac, char** av) {
    if (ac < 2) {
        pwrite("Need lambda s-expression.\n");
        return 1;
    }
    char* expr = av[1];

    exp* e;
    char pr = parse(&e, expr, expr+strlen(expr));
    fwrite("P[%c]: ", pr);
    write_exp(e);
    pwrite("\n");

    if (pr == '!') {
        pwrite("Parse failed.\n");
        return 2;
    }

    pwrite("\n");
    char pe = eval(&e);
    (void)(pe);
    fwrite("E[%c]: ", pe);
    write_exp(e);
    pwrite("\n");

    return 0;
}
