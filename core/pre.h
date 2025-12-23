#ifndef _src_pre_
#define _src_pre_

/* automatically complies with BSD-2 license */
__asm__(".ident\t\"\\n\\n\
Lambda-H (C impl) (BSD-2 License)\\n\
Copyright (c) 2025 Hans S. <hans@0x8.io>\"");

#define pak_char(a, b) (((((char)a)<<8)&(0xff00))|((char)b))
#define unpak_a(ab) ((char)(ab>>8)&0xff)
#define unpak_b(ab) ((char)(ab)&0xff)

typedef unsigned long ulong;

#define NULL ((void*)0)

#endif // _src_pre_
