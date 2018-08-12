#include <stdio.h>
#include <stdlib.h>
 
/*
    copy from https://0x00sec.org/t/heap-safari-thread-local-caching/5054
    PoC on tcache's internals regarding fast chunks (0x20 - 0x80).
    environ: Ubuntu 17.10, glibc-2.26
*/
int main(void) {
 
    void* a = malloc(0x20);
    void* b = malloc(0x20);
    void* c = malloc(0x20);
    void* d = malloc(0x20);
    void* e = malloc(0x20);
    void* f = malloc(0x20);
    void* g = malloc(0x20);
    void* h = malloc(0x20);
    void* i = malloc(0x20);
    void* j = malloc(0x20);
    void* k = malloc(0x20);

    fprintf(stderr, "a malloc(0x20): %p\n", a);
    fprintf(stderr, "b malloc(0x20): %p\n", b);
    fprintf(stderr, "c malloc(0x20): %p\n", c);
    fprintf(stderr, "d malloc(0x20): %p\n", d);
    fprintf(stderr, "e malloc(0x20): %p\n", e);
    fprintf(stderr, "f malloc(0x20): %p\n", f);
    fprintf(stderr, "g malloc(0x20): %p\n", g);
    fprintf(stderr, "h malloc(0x20): %p\n", h);
    fprintf(stderr, "i malloc(0x20): %p\n", i);
    fprintf(stderr, "j malloc(0x20): %p\n", j);
    fprintf(stderr, "k malloc(0x20): %p\n", k);

    /* Fill in the tcache for size 0x20. */
    free(a);
    free(b);
    free(c);
    free(d);
    free(e);
    free(f);
    free(g);
    /* Place the rest in the corresponding fastbin list. */
    free(h);
    free(i);
    free(j);
    free(k);
 
    /* Allocate the chunks out of the tcache->entries[idx]. */
    void* p1 = malloc(0x20);
    void* p2 = malloc(0x20);
    void* p3 = malloc(0x20);
    void* p4 = malloc(0x20);
    void* p5 = malloc(0x20);
    void* p6 = malloc(0x20);
    void* p7 = malloc(0x20);
    /*
        Retrieve chunk from fastbin.
        The rest of the chunks (h, i, j, k) will be allocated
        out of their fastbin and will be placed back into tcache->entries[idx].
    */
    void* p8 = malloc(0x20);
    void* p9 = malloc(0x20);

    fprintf(stderr, "p1/g malloc(0x20): %p/%p\n", p1,g);
    fprintf(stderr, "p2/f malloc(0x20): %p/%p\n", p2,f);
    fprintf(stderr, "p3/e malloc(0x20): %p/%p\n", p3,e);
    fprintf(stderr, "p4/d malloc(0x20): %p/%p\n", p4,d);
    fprintf(stderr, "p5/c malloc(0x20): %p/%p\n", p5,c);
    fprintf(stderr, "p6/b malloc(0x20): %p/%p\n", p6,b);
    fprintf(stderr, "p7/a malloc(0x20): %p/%p\n", p7,a);
    fprintf(stderr, "p8/k malloc(0x20): %p/%p\n", p8,k);
    fprintf(stderr, "p9/h malloc(0x20): %p/%p\n", p9,h);

 
    puts("Done!");
 
    /*
        Yes, I know. I should've free'd the latest allocations
        but that would require me to write down the rest of the
        alphabet. Sue me.
    */
    return 0;
}


/*
一開始放到 tcache 由頭到尾:
    g
    f
    e
    d
    c
    b
    a

之後放到 fastbin 由頭到尾:
    k
    j
    i
    h

之後 malloc() 前七個由 tcache 中拿， p1 ~ p7 分別為:
    g
    f
    e
    d
    c
    b
    a

之後因為 tcache 空了，所以 p8 會拿到 fastbin 第一個也就是 k

而取到 k 的當下，他會把 fastbin 剩餘的 chunk 填到 tcache 中直到填滿，由頭到尾:
    h
    i
    j

所以在下一次 malloc 也就是 p9 會再從 tcache 中取出來，也就是 h




*/