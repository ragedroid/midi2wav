#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (int argc, char*argv[]) {
    char* s = argv[1];
    char* p = s;
    int length=0;
    while (*p) {
        p++;
        length++;
    }
    while (*p != '/') {
        p--;
    }
    p++;
    printf("%s\n",p);

    char ss[length];
    strcpy(ss,s);

    char *sp;
	sp = ss;
    while (*sp) {
        sp++;
    }
    while (*sp != '.') {
        sp--;
    }
    sp++;
	
	*sp = 'w';
	*(sp+1) = 'a';
	*(sp+2) = 'v';
    
    printf("%s\n",ss);
    printf("%s\n",s);
    return 0;
}