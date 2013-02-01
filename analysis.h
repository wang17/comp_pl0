#include "struct.h"

#define ALENGTH 100
#define MAIN begin

class Analysis
{
    public:
    void init()
    { 
        int proLevel = 0; 
        setProcedure("MAIN");
        pStructPtr = P;
        sStructPtr = S;
        tStructPtr = T;

    }
    void setBegin();
    void setProcedure(char *);
    void opt(char *);
    void run();
    void identiArg(char *);
    void numArg(char *);
    void setTempVar(char *);
    void assign();
    void write();
    void read(char *);
    void ifStart();
    void ifEnd();
    void whileStart();
    void whileEnd();
    void whileLoop();
    void call(char *);
    void setConst(char *, char *);
    void addVar(char *);

    private:
    pStruct P[ALENGTH]; pStruct *pStructPtr;//指令集
    sStruct S[ALENGTH]; sStruct *sStructPtr;//数据堆栈
    tStruct T[ALENGTH]; tStruct *tStructPtr;//存储每个函数指令坐标下标
   // iStruct I[ALENGTH]; //

    varStruct constArr[ALENTH]; varStruct *constStructPtr;
    varStruct vartArr[ALENTH];  varStruct *varStructPtr;

    pStruct *jmpStack[10]; pStruct **jmpStackPtr;


};
