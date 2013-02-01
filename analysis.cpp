#include "analysis.h"


pStruct *Analysis::setProcedure(char *token)
{
    proLevel++;
    tAppend(token, getPLen(), proLevel);
    int pLen = getPLen();
    jmpStackPush(pAppend(JMP, 0, 0)); //第二个0为默认值，后面会修改
}


void Analysis::jmpStackPush(pStruct *p)
{
    *jmpStackPtr = p;
    jmpStackPtr++;
}

pStruct* Analysis::jmpStackPop()
{
    jmpStackPtr--;
    return *jmpStackPtr;
}

pStruct *pAppend(OPT o, int arg1, int arg2 )
{
    *pStructPtr = pStruct( o, arg1, arg2 );
    pStructPtr++;
    return (pStructPtr-1);
}

void Analysis::setBegin()
{
    //修改指令集JMP的跳转位置
    pStruct* p = jmpStackPop(); 
    p->arg2 = getPLen()+1;

    //添加INT指令，过程深度-1
    pAppend(INT, 0, 3+localVarCount());
    proLevel--;
}


tStruct *getProJmp()
{
    tStruct *t = tStructPtr;
    while ( t->level != proLevel && t >= T )
        t--;
    assert(t >= T);
    return t;
}
int localVarCount()
{
    int count = 0;
    for ( tStruct *ptr = tStructPtr-1; ptr >= T; --ptr)
    {
        if (ptr->level != proLevel )
            return count;
        count++;
    }
}


void Analysis::opt(char *);
void Analysis::run();
void Analysis::identiArg(char *);
void Analysis::numArg(char *);
void Analysis::setTempVar(char *);
void Analysis::assign();
void Analysis::write();
void Analysis::read(char *);
void Analysis::jmpStart();
void Analysis::jmpEnd();
void Analysis::whileLoop();
void Analysis::call(char *);
void Analysis::setConst(char *, char *);
void Analysis::addVar(char *);


void Analysis::getPLen() { return (pStructPtr - P); }


    

