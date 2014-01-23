#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSCALL_NUM 10
#include <type.h>

#define DECL_SYSCALL0(fn)                   int syscall_##fn();
#define DECL_SYSCALL1(fn,p1)                int syscall_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2)             int syscall_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3)          int syscall_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4)       int syscall_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5)    int syscall_##fn(p1,p2,p3,p4,p5);

#define DEF_SYSCALL0(fn, num)\
int syscall_##fn()       \
{\
    int a;\
    __asm__ volatile("int $0x30" : "=a" (a) : "0" (num));\
    return a;\
}

#define DEF_SYSCALL1(fn, num, P1)\
int syscall_##fn(P1 p1)\
{\
     int a;\
     __asm__ volatile("int $0x30" : "=a" (a) : "0" (num), "b" ((u32int)p1));\
     return a;\
}

#define DEF_SYSCALL2(fn, num, P1, P2)\
int syscall_##fn(P1 p1, P2 p2)\
{\
     int a;\
     __asm__ volatile("int $0x30" : "=a" (a) : "0" (num), "b" ((u32int)p1), "c" ((u32int)p2));\
     return a;\
}

#define DEF_SYSCALL3(fn, num, P1, P2, P3)\
int syscall_##fn(P1 p1, P2 p2, P3 p3)\
{\
     int a;\
     __asm__ volatile("int $0x30" : "=a" (a) : "0" (num), "b" ((u32int)p1), "c" ((u32int)p2), "d"((u32int)p3));\
     return a;\
}

#define DEF_SYSCALL4(fn, num, P1, P2, P3, P4)\
int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4)\
{\
     int a;\
     __asm__ volatile("int $0x30" : "=a" (a) : "0" (num), "b" ((u32int)p1), "c" ((u32int)p2), "d"((u32int)p3), "S"((u32int)p4));\
     return a;\
}


#define DEF_SYSCALL5(fn, num, P1, P2, P3, P4, P5)\
int syscall_##fn(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)\
{\
     int a;\
     __asm__ volatile("int $0x30" : "=a" (a) : "a" (num), "b" ((u32int)p1), "c" ((u32int)p2), "d"((u32int)p3), "S"((u32int)p4), "D"(u32int)p5);\
     return a;\
}

void syscall_handler(u32int in, registers_t *reg);
void init_syscall();


DECL_SYSCALL1(monitor_puts, char *);
DECL_SYSCALL0(fork);
DECL_SYSCALL0(pause);



#endif
