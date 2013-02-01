#ifndef _STRUCTS_ 
    #define _STRUCTS_
    #include "structs.h"
#endif

#define MAXIDENTI 100   //标示符最大长度
#define MAXSIGNNUM 10  //最大符号数量
#define MAXLINE 100
#define KEYLENGTH 13
#define NULL 0
#include <cstdio>
#include <cstring>

class Morphology
{
    private:
    int row, col;
    static char add_sign[MAXSIGNNUM][3];
    static char relation_sign[MAXSIGNNUM][3];
    static char multi_sign[MAXSIGNNUM][3];
    static char sign_table[MAXSIGNNUM][3];
    static char key_table[KEYLENGTH][12];
    char *pointer;
    char tempIdenti[MAXIDENTI]; char *identiPointer;//临时存储字符串
    char *sign_judge(char x[][3]);
    char num();
    char word();
    bool keyCheck(char *ch);
    void move();
    void tempIdentiAppend(char temp);
    void initIdenti();

    returnType identi();
    returnType unsign();
    returnType sign();

    void removeSpace();
    void init()
    {
        last_row = row = 1; last_col = col = 1;
        pointer = NULL;
        memset( line, 0, sizeof(line)); linePtr = line;
        initIdenti();
    }


    public:
    int last_row, last_col;
    char line[MAXLINE]; char *linePtr;
    Morphology() { init(); }
    Morphology(char *str) { init(); setSourceString(str); }

    void setSourceString(char * str) { pointer = str; }
    returnType nextToken();


};
