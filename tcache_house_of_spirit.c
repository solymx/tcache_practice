#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, const char* argv[]) {
    size_t fake_chunk_and_more[64];
    memset(fake_chunk_and_more, 'A', sizeof(fake_chunk_and_more));
    printf("stack buf: %p\n", (void *)fake_chunk_and_more);
    getchar();
    
    char* fake_chunk = (char * )fake_chunk_and_more;
    *(long *)(fake_chunk + sizeof(long)) = 0x110; // set size = 0x110
    printf("set fake size\n");
    getchar();

    *(long *)(fake_chunk + 0x110 + sizeof(long)) = 0x40;  // set pre_inused 
    printf("set pre_inused\n");
    getchar();

    char *mem = fake_chunk + 2*sizeof(long);
    free(mem);
    printf("free chunk\n");
    getchar();

    void *mem2 = malloc(0x100);
    printf("malloc(0x100) returned: %p\n", mem2);
    getchar();

    return 0;
}

/*
    origin: https://hk.saowen.com/a/2475e2d4e78a88754e3e64989016def09408a0d50b71bfd328559e27539c1d2f

    environ: Ubuntu 17.10 / glibc 2.26

*/