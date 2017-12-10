//
// Created by alin on 12/10/17.
//

#include <cstdio>

int main() {
    char a[16];
    FILE* fin = fopen("pt1","r");
    while(!feof(fin)) {
        printf("Entered\n");
        int b = (int) fread(a, 1, 1, fin);
        printf("%c\n",a[0]);
        printf("read %d\n", b);
    }
}