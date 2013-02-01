#include "grammar.h"
#include <cstdio>

char ch[2000];
char c;
int main(int argc, char *argv[])
{
    FILE *fp;
    if ( (fp = fopen(argv[1], "r")) == NULL )
    {
        printf( "can't open.");
        return 0;
    }
    else
    {
        char *pointer = ch;
        while ( (c = fgetc(fp)) != EOF )
        {
            *pointer = c;
            pointer++;
        }
        Grammar g(ch);
        g.run();

    }
}
