#include "morphology.h"
#include "assert.h"
#include <cstring>
#include <cstdio>


char Morphology::add_sign[MAXSIGNNUM][3] = {"+", "-"};
char Morphology::relation_sign[MAXSIGNNUM][3] = { "=", ">=", "<=", "<>", "<", ">"};
char Morphology::multi_sign[MAXSIGNNUM][3] = {"*" ,"/"};
char Morphology::sign_table[MAXSIGNNUM][3] = {":=", ",", "(", ")", ".", ";"};
char Morphology::key_table[KEYLENGTH][12] = { "const", "var", "procedure", "if", "then", "odd", "while", "do", "begin", "end", "call", "read", "write", };

char Morphology::num() 
{
//<数字>  ::= 0|1|2|3…...8|9
//成功返回int数字，否则返回-1
    char t = *pointer;
    if ( t >= '0' && t <= '9' )
    {
        move();
        return t;
    }
    else return 0;
}

char Morphology::word() 
//<字母>::= a|b|c|d…..x|y|z  
//成功指针后移，返回字符，否则返回0
{
    char t = *pointer;
    if (t >= 'a' && t <= 'z')
    {
        move();
        return t;
    }
    else return 0;
}


returnType Morphology::unsign()
//<无符号整数>  ::= <数字>{<数字>} 
//如果存在unsign，就返回token首指针，否则返回NULL
{
    int res = 0, t;
    bool success = false;
    initIdenti();
    while ( t = num() )
    {
        success = true;
        tempIdentiAppend(t);
    }
    if (success)
        return returnType(tempIdenti, NUM);
    else 
        return returnType(NULL, ERROR);
}


returnType Morphology::identi()
// <标识符>     ::= <字母>{<字母>|<数字>} 
{
    char temp;
    initIdenti();
    if ( temp = word() )
    {
        tempIdentiAppend(temp);
        while ( (temp = word()) || (temp = num()) )
        {
            tempIdentiAppend(temp);
        }
        return returnType(tempIdenti, IDENTI);
    }
    else return returnType(NULL, ERROR);
}


void Morphology::initIdenti() 
//初始化临时存储token的指针
{ 
    identiPointer = tempIdenti; 
    memset(tempIdenti, 0, sizeof(tempIdenti)); 
} 


void Morphology::tempIdentiAppend(char temp)
//token添加一个元素
{
    *identiPointer = temp;
    identiPointer++;
    //cout << identiPointer <<" " << tempIdenti << endl;
    assert( identiPointer < tempIdenti + 100);
}


char *Morphology::sign_judge(char table[][3])
{
    for ( int i = 0; i < MAXSIGNNUM; ++i)
    {
        int len = strlen(table[i]);
        bool check = true;
        for ( int j = 0; j < len; ++j )
            if (*(pointer+j) != table[i][j])
                check = false;
        if ( len > 0 && check )
        {
            for ( int j = 0; j < len; ++j )
            {
                tempIdentiAppend(*(pointer));
                move();
            }
            return tempIdenti;
        }
    }
    return NULL;
}


returnType Morphology::sign()
//判断符号
{
    initIdenti();
    char * res;
    if (res = sign_judge(sign_table))       return returnType(res, SIGN);
    if (res = sign_judge(multi_sign))       return returnType(res, MULTISIGN);
    if (res = sign_judge(add_sign))         return returnType(res, ADDSIGN);
    if (res = sign_judge(relation_sign))    return returnType(res, RELATIONSIGN);
    return returnType(NULL, ERROR);
}


returnType Morphology::nextToken()
{
    returnType re;

    removeSpace();
    last_col = col; last_row = row;
    if ((*pointer) == 0)
        return returnType(NULL, END);     //指向文件尾指针

    re = identi();                      //判断标示符
    if (re.type == ERROR ) re = unsign();   //无符号整数
    if (re.type == ERROR ) re = sign();     //符号
    if (re.type == ERROR )
    {
        //TODO no token to recognize
        move();
        return returnType(pointer, ERROR);
    }
    if (re.type == IDENTI && keyCheck(re.pos))
        re.type = KEY;

    printf("token: %s %d\n", re.pos, re.type);
    return re;

}


bool Morphology::keyCheck(char *ch)
{
    for ( int i = 0; i < KEYLENGTH; ++i ) 
        if (strcmp(ch, key_table[i]) == 0)
            return true;
    return false;
}


void Morphology::removeSpace()
//取出多余空行和空格
{
    while (*pointer == '\n' || *pointer == '\t' || *pointer == ' ') 
    {
        if (*pointer == '\n')
        {
            row += 1;
            col = 0;
            memset(line, 0, sizeof(line));
            linePtr = line;
        }
        move();
    }
}


void Morphology::move()
{
    *(linePtr++) = *pointer;
    pointer++;
    col++;
}

