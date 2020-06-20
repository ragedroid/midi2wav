#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char*argv[]) {
    char* s = argv[1];
    char* p = s;
    while (*p) {
        p++;
    }
    while (*p != '/') {
        p--;
    }
    p++;
    printf("%s\n",p);
    return 0;
}