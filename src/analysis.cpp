#include "analysis.h"
#include "struct.h"
#include <iostream>

char Analysis::opt_sign[SIGNSIZE][4] = {
    "NUL","NUL","+","-",
    "*","/","ODD", "NUL",
    "=","<>","<",">=",
    ">","<=" };


pStruct *Analysis::setProcedure(char *token)
{
    tStruct * tmp = currentFather;
    if (tmp != NULL)
    for ( tStruct * sons = tmp->son; sons != NULL; sons = sons->bro)
    {
        fflush(stdout);
        if (strcmp(sons->name, token) == 0)
        {
            return NULL;
        }
    }
    
    proLevel++;
    currentFather = tAppend(token, proLevel, currentFather);
    jmpStackPush(pAppend(JMP, 0, 0)); //第二个0为默认值，后面会修改
    return pStructPtr - 1;
}


tStruct *Analysis::tAppend( char * t, int l, tStruct *fa)
{
    *tStructPtr = tStruct(t, tStructPtr, fa);
    tStructPtr++;
    return tStructPtr-1;
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


pStruct *Analysis::pAppend(OPT o, int arg1, int arg2 )
{
    (*pStructPtr) = pStruct( o, arg1, arg2 );
    pStructPtr++;
    return (pStructPtr-1);
}

void Analysis::setBegin()
{
    //修改指令集JMP的跳转位置
    pStruct* p = jmpStackPop(); 
    p->arg2 = getPLen();

    //获得实际pcode对应代码行数
    currentFather->pcode = getPLen();

    //添加INT指令，过程深度-1
    pAppend(INT, 0, 3+(currentFather->vars));
   // currentFather = currentFathher->father;
    proLevel--;
}

void Analysis::opt(char *token)
{
    for (int i = 0; i < SIGNSIZE; ++i )
    {
        if (strcmp(token, opt_sign[i]) == 0)
        {
            *(optStackPtr++) = i;
            break;
        }
    }
}


void Analysis::run()
{ 
    pAppend(OPR, 0, *(--optStackPtr)); 
}


bool Analysis::identiArg(char *token)
{
    tStruct* t = currentFather;
    int cnt = 0;
    while (t != NULL)
    {
        for (int i = 0; i < (t->vars); ++i )
        {
            varStruct *v = t->varSt;
            if (strcmp((v+i)->name, token) == 0)
            { pAppend(LOD, cnt, i+3); return true;}
        }
        for (int i = 0; i < (t->consts); ++i )
        {
            constStruct *v = (t->constSt)+i;
            if (strcmp(v->name, token) == 0)
            { pAppend(LIT, 0, ((t->constSt)+i)->value);return true;}
        }
        cnt++;
        t = t->father;
    }
    return false;
}


bool Analysis::endProce()
{
    currentFather = currentFather-> father;
    pAppend(OPR, 0, 0);
}


bool Analysis::numArg(char *token)
{
    int res = 0;
    while (*token)
    {
        res = res * 10 + (*token) - '0';
        token++;
    }
    pAppend(LIT, 0, res);
    return true;
}


bool Analysis::setTempVar(char *token)
{
    tStruct* t = currentFather;
    int cnt = 0;
    while (t != NULL)
    {
        for (int i = 0; i < (t->vars); ++i )
            if (strcmp(((t->varSt)+i)->name, token) == 0)
            { 
                currentVarLevel = cnt;
                currentVarOffset = i+3;
                return true;
            }
        cnt++;
        t = t->father;
    }
    return false;
}


void Analysis::assign() { pAppend(STO, currentVarLevel, currentVarOffset); }


void Analysis::write() { pAppend(WRT, 0, 0); }


bool Analysis::read(char *ch)
{
    tStruct* t = currentFather;
    int cnt = 0;
    while (t != NULL)
    {
        for (int i = 0; i < (t->vars); ++i )
            if (strcmp(((t->varSt)+i)->name, ch) == 0)
            {
                pAppend(RED, cnt, i+3);
                return true;
            }
        cnt++;
        t = t->father;
    }
    return false;
}

    
void Analysis::jpcStart()
{ *(jmpStackPtr++) = pAppend(JPC, 0, 0); }


void Analysis::jpcEnd()
{ (*(--jmpStackPtr))->arg2 = getPLen(); }


void Analysis::setWhileLoop()
{ *(whileLoopPtr++) = getPLen(); }


void Analysis::whileLoop()
{ pAppend(JMP, 0, *(--whileLoopPtr));}


bool Analysis::checkExist(char *token)
{
    tStruct *t = currentFather;
    for ( int i = 0; i < (t->vars); ++i )
    {
        char * p = ((t->varSt)+i)->name;
        if (strcmp(p, token) == 0)
            return false;
    }
    for ( int i = 0; i < (t->consts); ++i )
    {
        char * p = ((t->constSt)+i)->name;
        if (strcmp(p, token) == 0)
            return false;
    }
    return true;
}

bool Analysis::setConst(char *token, char *value)
{
    if (!checkExist(token)) return false;      

    int res = 0;
    while ((*value) > 0)
    {
        res = res*10+(*value) - '0';
        value++;
    }
    if (currentFather->constSt == NULL)
        currentFather->constSt = constStructPtr;
    (currentFather->consts)++;
    
    *(constStructPtr++) = constStruct(token, currentFather, proLevel, currentFather->consts, res);
    return true;
}


bool Analysis::addVar(char *token)
{
    if (!checkExist(token)) return false;
    if (currentFather->varSt == NULL)
        currentFather->varSt = varStructPtr;
    (currentFather->vars)++;
    *(varStructPtr++) = varStruct(token, currentFather, proLevel, currentFather->vars);
    return true;
}


int Analysis::getPLen() { return (pStructPtr - P)+1; }



bool Analysis::call(char *token)
{
    tStruct *t = currentFather;
    
    int l = 0;
    while ( t != NULL )
    {
        for ( tStruct * tmp = t->son; tmp != NULL; tmp = tmp->bro)
        {
            if (strcmp(tmp->name, token) == 0)
            {
                pAppend(CAL, l, tmp->pcode);
                return true;
            }
        }
        t = t->father;
        l++;
    }

    return false;
}


void Analysis::show()
{
    int cnt = 0;
    for (pStruct *i = P; i != pStructPtr; ++i )
    {
        cnt++;
        printf( "%d %s %d %d\n", cnt, command[i->opt], i->arg1, i->arg2);
    }
}
    
char Analysis::command[10][4] = { "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "RED", "WRT" };
