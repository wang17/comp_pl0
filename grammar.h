#include "morphology.h"
#include "analysis.h"
#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;

class Grammar
{
    private:
    Analysis ana;
    Morphology mor;
    char *token;
    tokenTYPE tokenType;
    void init(){ nextToken(); memset(errorInfo, 0, sizeof(errorInfo));}
    void nextToken();
    bool keyCheck( char*);
    char errorInfo[40];

    recogTYPE program();        //<程序>   ::= <分程序>.
    recogTYPE subprogram();     //<分程序>     ::= [<常量说明部分>][<变量说明部分>][<过程说明部分>]<语句>
    recogTYPE constState();     //<常量说明部分>  ::= const<常量定义>{,<常量定义>};
    recogTYPE constDef();       //<常量定义>   ::= <标识符>=<无符号整数>
    recogTYPE varState();       //<变量说明部分>  ::= var<标识符>{, <标识符>};
    recogTYPE procedureState(); //<过程说明部分>  ::= <过程首部><分程序>{;<过程说明部分>}
    recogTYPE procedureHead();  //<过程首部>   ::= procedure<标识符>;
    recogTYPE statement();      //<语句>   ::= <赋值语句>|<条件语句>|<当循环语句>|<过程调用语句>|<复合语句>|<读语句>|<写语句>|<空>
    recogTYPE ifState();        //<条件语句>   ::= if<条件>then<语句>
    recogTYPE whileState();     //<当循环语句>     ::= while<条件>do<语句>
    recogTYPE callState();      //<过程调用语句>  ::= call<标识符>
    recogTYPE complexState();   //<复合语句>   ::= begin<语句>{;<语句>}end
    recogTYPE readState();      //<读语句>     ::= read ‘ ( ’<标识符>{, <标识符>} ‘ ) ’
    recogTYPE writeState();     //<写语句>     ::= write ‘ ( ’<表达式>{, <表达式>} ‘ ) ’
    recogTYPE assignState();    //<赋值语句>   ::= <标识符> := <表达式>
    recogTYPE condition();      //<条件>   ::= <表达式><关系运算符><表达式>|odd<表达式>
    recogTYPE expression();     //<表达式>     ::= [+|-]<项>{<加法运算符><项>}
    recogTYPE term();           //<项>     ::= <因子>{<乘法运算符><因子>}
    recogTYPE factor();         //<因子>   ::= <标识符>|<无符号整数>| ‘ ( ’ <表达式> ‘ ) ’
    recogTYPE expectedError( recogTYPE type, char* info );


    public:
    Grammar(){}
    Grammar(char *str):mor(str) { nextToken(); }
    void run();
    void showError();
};
