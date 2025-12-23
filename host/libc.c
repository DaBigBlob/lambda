
#include "../core/need.h"
#include "../parse/need.h"
#include "../bin/cli/need.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern void* __mem_alloc(ulong n) {
    return malloc(n);
}

extern void __mem_free(void* p) {
    free(p);
}

extern void __mem_copy(void* d, const void* s, ulong n) {
    memcpy(d, s, n);
}


extern int __nstr_eq(const char* a, const char* b, ulong l) {
    return strncmp(a, b, l);
}

extern void __write_nstr(const char* p, ulong n) {
    for (ulong i=0; i < n; ) {
        long r = write(STDOUT_FILENO, p + i, n - i);
        if (r > 0) {
            i += r;
            continue;
        }
        if (r == 0) break; // EOF
    }
}
extern void __write_cstr(const char* p) {
    fputs(p, stdout);
}

extern void __read_nstr(char* p, ulong n) {
    for (ulong i=0; i < n; ) {
        long r = read(STDIN_FILENO, p + i, n - i);
        if (r > 0) {
            i += r;
            continue;
        }
        if (r == 0) break; // EOF
    }
}
