#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char ch;
    FILE* src, * tgt;
    char srcf[] = "readhere.txt";
    char tgtf[] = "writehere.txt";
    src = fopen(srcf, "r");
    tgt = fopen(tgtf, "w");

    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, tgt);
    }
    printf("files copied!");
    fclose(src);
    fclose(tgt);
    return 0;
}