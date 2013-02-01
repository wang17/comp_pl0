#define T_STRUCT_CHAR_LENGTH 20

enum tokenTYPE{ERROR, IDENTI, NUM, RELATIONSIGN, ADDSIGN, MULTISIGN, SIGN, KEY, END};
enum recogTYPE{SUCCESS, FAIL, NOTEXIST};
enum OPT{LIT, OPR, LOD, STO, CAL, INT, JMP, JPC, RED, WRT};

struct returnType 
{ 
    char *pos; tokenTYPE type; 
    returnType(){}
    returnType(char *p, tokenTYPE t)      
    {
        pos = p; type = t;
    }
};

struct pStruct
{
    int i, arg1, arg2;
    OPT opt;
    pStruct(){}
    pStruct(int ii, OPT o, int a1, int a2)
    { i = ii; opt = o; arg1 = a1; arg2 = a2; }
};

struct tStruct
{
    char proName[T_STRUCT_CHAR_LENGTH];
    int pos, int level;
    tStruct() { memset(proName, 0, sizeof(proName)); }
    tStruct(char *c, int pos)
    {
        strcpy();
        
};

struct varStruct
{
    char name[T_STRUCT_CHAR_LENGTH]; int level;
    varStruct() { memset( name, 0, sizeof(name)); int level = 0; }
    varStruct( char * c, int l )
    {
        strcpy(name, c);
        level = l;
    }
}

typedef sStruct int;
