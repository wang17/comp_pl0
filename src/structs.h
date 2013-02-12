#include <cstring>

enum tokenTYPE{ERROR, IDENTI, NUM, RELATIONSIGN, ADDSIGN, MULTISIGN, SIGN, KEY, END};
enum recogTYPE{SUCCESS, FAIL, NOTEXIST};

struct returnType 
{ 
    char *pos; tokenTYPE type; 
    returnType(){}
    returnType(char *p, tokenTYPE t)      
    {
        pos = p; type = t;
    }
};

