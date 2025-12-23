#ifndef _parse_sexp_
#define _parse_sexp_

#include "sexp.h"
#include "../../core/mem.h"
#include "../need.h"

#define lwrite(p, l) __write_nstr(p, l);
#define pwrite(p) __write_cstr(p);

extern void write_exp(exp* e) {
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

extern char parse(exp** _e, char* p, char* d) {
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
                #define streq(nm) (__nstr_eq(nm, fp, 3) == 0)
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

#endif // _parse_sexp_
