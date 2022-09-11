#include <stdio.h>
int main(int argc, char* argv[])
{
    FILE* fptr;
    fptr=fopen(argv[1], "r")
    {
        printf("error");
    }
    char cp;
    while((cp=fgetc(fptr))!=EOF)
    {
        printf("%c",cp);
    }
    fclose(fptr);
}