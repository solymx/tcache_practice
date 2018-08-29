#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
int main(int argc, const char* argv[]) {
    char *mem = malloc(0x48);               
    char *sentry = malloc(0x18);            
    memset(sentry, 'b', 0x10);              
    printf("mem: %p, sentry: %p\n",mem, sentry);
    printf("sentry content: %s\n", sentry);
    *(long* )(mem - sizeof(long)) = 0x110;  
    printf("set fake size to mem...\n");
    getchar();

    printf("free mem with fake size\n");
    free(mem);
    getchar();

    char *mem2 = malloc(0x100);  
    memset(mem2, 'A', 0x100);
    printf("mem2: %p\n", mem2);
    printf("sentry content: %s\n", sentry);
    getchar();
    return 0;
}

/*
    origin from: https://hk.saowen.com/a/2475e2d4e78a88754e3e64989016def09408a0d50b71bfd328559e27539c1d2f
    environ: Ubuntu 17.10 / glibc 2.26



*/




