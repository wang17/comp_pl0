#include "struct.h"
#include <iostream>
#include <cstring>

#define ALENGTH 100
#define SIGNSIZE 20             
#define T_STRUCT_CHAR_LENGTH 20 //T/P两个桟上线
#define OPTMAX  100             //pcode数组上限
#define WHILESIZE 10            //while 循环最大深度
#define COMMAND 10              //指令种类数

using namespace std;

enum OPT{LIT, OPR, LOD, STO, CAL, INT, JMP, JPC, RED, WRT}; 
struct varStruct;
struct tStruct;
struct constStruct;

struct pStruct
{
    int i, arg1, arg2;
    OPT opt;
    pStruct(){}
    pStruct(OPT o, int a1, int a2)
    { opt = o; arg1 = a1; arg2 = a2; }
}; // pCode 类型

struct varStruct
{
    char name[T_STRUCT_CHAR_LENGTH]; tStruct*proce;
    int offset, level, value;
    varStruct() { memset( name, 0, sizeof(name));}
    varStruct( char * c, tStruct* pro, int l, int o )
    {
        strcpy(name, c);
        proce = pro;
        level = l; offset = o;
    }
};  //变量符号表

struct tStruct
{
    char name[T_STRUCT_CHAR_LENGTH];
    int level, vars, consts; //深度 直接变量数 直接常量数
    varStruct *varSt;       //对应变量首指针
    constStruct *constSt;   //常量首指针
    tStruct *father, *son, *bro;    //父亲、第一个儿子、下一个兄弟级别的过程指针
    int pcode;                      //实际代码对应的行数
    tStruct() { memset(name, 0, sizeof(name)); }
    tStruct(char *c, tStruct *th, tStruct* fa )
    {
        son = bro = NULL;
        strcpy(name, c);
        vars = 0; 
        consts = 0;
        father = fa;
        constSt = NULL;
        varSt = NULL;
        
        if (father == NULL ) return;
        if (father->son == NULL )
            father->son = th;
        else
        {
            tStruct *tmp = father->son;
            while (tmp->bro != NULL)
                tmp = tmp->bro;
            tmp->bro = th;
        }

    }
};//过程结构体

struct constStruct
{
    char name[T_STRUCT_CHAR_LENGTH]; 
    tStruct*proce; //所在过程的地址
    int offset, level, value;
    constStruct(){ memset(name, 0, sizeof(name)); }
    constStruct( char *c, tStruct*pro, int l, int o, int val)
    {
        strcpy(name, c);
        proce = pro;
        level = l; offset = o;
        value = val;
    }

};


class Analysis
{
    public:
    Analysis(){init();}
    void init()
    { 
        int proLevel = 0; 
        currentFather = NULL;
        pStructPtr = P;
        tStructPtr = T;
        jmpStackPtr = jmpStack;
        optStackPtr = optStack;
        whileLoopPtr = whileLoopArr;
        constStructPtr = constArr;
        varStructPtr = varArr;
    }
    void show();                    //输出pcode
    void setBegin();                //过程的主语句开始
    pStruct* setProcedure(char *);  //定义一次过程头
    void opt(char *);               //添加一个操作符
    void run();                     //一次opr操作
    bool identiArg(char *);         //变量/常量参数
    bool numArg(char *);            //常数参数
    bool setTempVar(char *);        //设置临时变量用于赋值
    void assign();                  //赋值
    void write();                   //写表达式
    bool read(char *);              //读变量
    void jpcStart();                //if/while跳转语句标记
    void jpcEnd();                  //if/while跳转目标
    void whileLoop();               //while循环结束位置
    void setWhileLoop();            //设置循环回复开始位置
    bool call(char *);              //调用函数
    bool setConst(char *, char *);  //定义常量
    bool addVar(char *);            //定义变量
    bool endProce();                //函数结束

    private:
    bool checkExist(char *token);
    int proLevel;
    tStruct *currentFather;
    pStruct P[ALENGTH];                 pStruct *pStructPtr;//指令集
    tStruct T[ALENGTH];                 tStruct *tStructPtr;//存储每个函数指令坐标下标
    pStruct *jmpStack[WHILESIZE];       pStruct **jmpStackPtr; //条件跳转桟
    constStruct constArr[ALENGTH];      constStruct *constStructPtr;//常量数组
    varStruct varArr[ALENGTH];          varStruct *varStructPtr;    //变量数租
    int whileLoopArr[WHILESIZE];              int *whileLoopPtr;    //while循环指针
    int optStack[OPTMAX];                  int *optStackPtr;        //操作符桟

    static char opt_sign[SIGNSIZE][4];
    static char command[COMMAND][4];
    int getPLen();
    tStruct *tAppend(char *, int l, tStruct *);
    pStruct *pAppend(OPT, int arg1, int arg2);
    void jmpStackPush(pStruct *p);
    pStruct* jmpStackPop();

    int currentVarLevel, currentVarOffset, currentConstOffset;
};
