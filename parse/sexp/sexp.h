#ifndef _parse_sexp_h_
#define _parse_sexp_h_

#include "../../core/ast.h"

extern void write_exp(exp* e);

extern char parse(exp** _e, char* p, char* d);

#endif // _parse_sexp_h_
