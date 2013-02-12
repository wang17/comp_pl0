#include "grammar.h"
#include <cstring>

recogTYPE Grammar::program()
{
    recogTYPE re;
    ana.setProcedure("MAIN");
    if ( (re = subprogram()) == SUCCESS )
    {
        if ( keyCheck(".") )
            return SUCCESS;
        else return FAIL;
    }
    return re;
}


recogTYPE Grammar::subprogram()
{
    recogTYPE re;
    if ( (re = constState()) == FAIL ) return re;
    if ( (re = varState())  == FAIL ) return re;
    if ( (re = procedureState()) == FAIL ) return re;

    ana.setBegin();
    if ( (re = statement()) != SUCCESS ) return FAIL;
    ana.endProce();
    return SUCCESS;
}


recogTYPE Grammar::procedureHead()
{
    if (keyCheck("procedure"))
    {
        if ( tokenType == IDENTI)
        {
            pStruct* t = ana.setProcedure(token);
            if (t == NULL)
                return dupliDef("函数", token);
            nextToken();
            if (keyCheck(";"))
                return SUCCESS;
            else
                return expectedError(NOTEXIST, "';'");
        }
        else
            return expectedError(NOTEXIST, "标识符");
    }
    else return NOTEXIST;
}


recogTYPE Grammar::procedureState()
{
    recogTYPE res;
    if ( (res = procedureHead()) != SUCCESS) //no head
        return res;
    else //success
    {
        if ( (res=subprogram()) == SUCCESS )
        {

            if (keyCheck(";"))
            {
                while ((res=procedureState()) == SUCCESS);
                if (res == FAIL)
                    return FAIL;
                else 
                {
                    return SUCCESS;
                }
            }
            else
                return expectedError(FAIL, "';'");
        }
        else
            return expectedError(res, "分程序");
    }
}


recogTYPE Grammar::statement()
{
    
    recogTYPE res;
    if ( (res = ifState())      != NOTEXIST ) return res;
    if ( (res = whileState())   != NOTEXIST ) return res;
    if ( (res = callState())    != NOTEXIST ) return res;
    if ( (res = complexState()) != NOTEXIST ) return res;
    if ( (res = readState())    != NOTEXIST ) return res;
    if ( (res = writeState())   != NOTEXIST ) return res;
    if ( (res = assignState())  != NOTEXIST ) return res;
    
    return SUCCESS;
}


recogTYPE Grammar::condition()
{
    recogTYPE re;
    if (keyCheck("odd") )
    {
        if ((re = expression()) == SUCCESS)
        {
            ana.opt("odd");
            ana.run();
            return SUCCESS;
        }
        else
            return expectedError(re,"表达式");
    }
    else if ((re = expression() ) == SUCCESS)
    {
        if ( tokenType == RELATIONSIGN )
        {
            ana.opt(token);
            nextToken();
            if ((re = expression()) == SUCCESS)
            {
                ana.run();
                return SUCCESS;
            }
            else
                return expectedError(re,"表达式");
        }
        else
            return expectedError(NOTEXIST,"关系运算符");
    }
    else
    {
        return NOTEXIST;
    }
}


recogTYPE Grammar::term()
{
    recogTYPE re;
    if ( (re = factor()) == SUCCESS)
    {
        while ( tokenType == MULTISIGN )
        {
            ana.opt(token);
            nextToken();
            if ((re=factor()) != SUCCESS)
                return expectedError(re,"因子");
            ana.run();
        }
        return SUCCESS;
    }
    else return NOTEXIST;
}


recogTYPE Grammar::factor()
{
    recogTYPE re;
    if ( tokenType == IDENTI )
    {
        ana.identiArg(token);
        nextToken();
        return SUCCESS;
    }
    else if ( tokenType == NUM )
    {
        ana.numArg(token);
        nextToken();
        return SUCCESS;
    }
    else if ( keyCheck("(") )
    {
        if ( (re = expression()) == SUCCESS )
        {
            //set Var

            if ( keyCheck(")") ) return SUCCESS;
            else
                return expectedError(NOTEXIST,"')'");
        }
        else
            return expectedError(re,"表达式");
    }
    else
        return NOTEXIST;
}



recogTYPE Grammar::expression()
{
    bool exist = false;
    recogTYPE re; 
    if ( tokenType == ADDSIGN ) 
    {
        exist = true;
        ana.numArg("0");
        ana.opt(token);
        nextToken();
    }
    
    if ( (re = term()) == SUCCESS )
    {
        //setVar
        //ana.run();
        while ( tokenType == ADDSIGN )
        {
            ana.opt(token);
            nextToken();
            if ( (re = term()) != SUCCESS )
                return expectedError(re, "项");
            ana.run();
        }
        return SUCCESS;
    }

    if (exist)
        return expectedError( NOTEXIST, "表达式");
    else 
    {
        return NOTEXIST;
    }
}


void Grammar::showError()
{
    if ((*errorInfo) != 0)
    {
        printf( "错误:在%d行，%d列，有一个错误:%s\n %s\n", mor.last_row, mor.last_col,errorInfo, mor.line );
        for ( int i = 0; i < mor.last_col-1; ++i )
            printf( " ");
        printf( "^\n");
    }
}


recogTYPE Grammar::expectedError( recogTYPE type, char *info )
{
    if ( type == NOTEXIST )
    {
        strcpy(errorInfo, "缺少一个");
        strcpy(errorInfo+12, info);
    }
    return FAIL;
}


recogTYPE Grammar::assignState()
{
    recogTYPE re;
    if ( tokenType == IDENTI )
    {
        if (!ana.setTempVar(token))
            return expectedError(NOTEXIST, "已定义的变量");
        nextToken();
        if ( keyCheck(":=") )
        {
            if ( (re=expression()) == SUCCESS )
            {
                ana.assign();
                return SUCCESS;
            }
            else
                return expectedError(re, "表达式");
        }
        else
            return expectedError(NOTEXIST, ":=");
    }
    else return NOTEXIST;
}
recogTYPE Grammar::complexState()
{
    recogTYPE re;
    if (keyCheck("begin"))
    {
        if ((re = statement()) == SUCCESS)
        {
            while (keyCheck(";"))
            {
                if ( (re=statement()) != SUCCESS )
                    return expectedError(re, "语句");
                // call func
            }
            if ( keyCheck("end") )
            {
                return SUCCESS;
            }
            else
                return expectedError(NOTEXIST, "'end'");
                
        }
        else
            return expectedError(re, "语句");
    }
    else
        return NOTEXIST;
}
recogTYPE Grammar::writeState()
{
    recogTYPE re;
    if (keyCheck("write"))
    {
        if (keyCheck("("))
        {
            if (( re=expression()) == SUCCESS)
            {
                ana.write();
                while ( keyCheck(",") ) 
                {
                    if ((re=expression()) != SUCCESS)
                        return expectedError( re, "表达式");
                    ana.write();
                }
                if (keyCheck(")"))
                    return SUCCESS;
                else
                    return expectedError( NOTEXIST, "')'");
            }
            else
                return expectedError( re, "表达式");
        }
        else
            return expectedError( NOTEXIST, "'('");
    }
    else return NOTEXIST;
}

recogTYPE Grammar::varNotExist( char * token )
{
    strcpy(errorInfo, "变量不存在:");
    strcat(errorInfo, token );
    return FAIL;
}
recogTYPE Grammar::readState()
{
    if (keyCheck("read"))
    {
        if (keyCheck("("))
        {
            if (tokenType == IDENTI)
            {
                if (!ana.read(token))
                    return varNotExist( token );
                nextToken();
                while ( keyCheck(",") ) 
                {
                    if (tokenType != IDENTI)
                        return expectedError( NOTEXIST, "标识符");
                    //getVar
                    if (!ana.read(token))
                        return varNotExist(token);
                    nextToken();
                }
                if (keyCheck(")"))
                {
                    return SUCCESS;
                }
                else
                    return expectedError( NOTEXIST, "')'");
            }
            else
                return expectedError( NOTEXIST, "标识符");

        }
        else
            return expectedError( NOTEXIST, "'('");

    }
    else return NOTEXIST;
}

recogTYPE Grammar::ifState()
{
    recogTYPE re;
    if ( keyCheck("if") )
    {
        if ((re = condition()) == SUCCESS) 
        {
            ana.jpcStart();
            if ( keyCheck("then") )
            {
                if ((re = statement()) == SUCCESS )
                {
                    ana.jpcEnd();
                    return SUCCESS;
                }
                else
                    return expectedError( re, "语句");
            }
            else
                return expectedError(NOTEXIST, "'then'");
        }
        else 
            return expectedError(re, "条件");
    }
    else return NOTEXIST;
}

recogTYPE Grammar::whileState()
{
    recogTYPE re;
    if ( keyCheck("while") )
    {
        ana.setWhileLoop();
        if ((re = condition()) == SUCCESS) 
        {
            ana.jpcStart();
            if ( keyCheck("do") )
            {
                if ((re = statement()) == SUCCESS )
                {
                    ana.whileLoop();
                    ana.jpcEnd();
                    return SUCCESS;
                }
                else
                    return expectedError(re, "语句");
            }
            else
                return expectedError(NOTEXIST, "'do'");
        }
        else 
            return expectedError(re, "条件");
    }
    else return NOTEXIST;
}

recogTYPE Grammar::callState()
{
    if (keyCheck("call"))
    {
        if ( tokenType == IDENTI)
        {
            if (!ana.call(token)) return expectedError(NOTEXIST, "已定义的过程");
            nextToken();
            return SUCCESS;
        }
        else
            return expectedError(NOTEXIST, "标识符");
    }
    else
        return NOTEXIST;
}


recogTYPE Grammar::constState()
{
    recogTYPE re;
    if (keyCheck("const"))
    {
        if ( (re = constDef()) != SUCCESS )
            return expectedError( re, "常量定义");

        while ( keyCheck(",") )
            if ((re = constDef()) != SUCCESS)
                return expectedError( re, "常量定义");

        if (keyCheck(";")) 
        {
            return SUCCESS;
        }
        else
            return expectedError(NOTEXIST, "';'");
    }
    else
        return NOTEXIST;
}

recogTYPE Grammar::constDef()
{
    if (tokenType == IDENTI)
    {
        char con[ALENGTH];
        strcpy(con, token);

        nextToken();
        if ( keyCheck("="))
        {
            if (tokenType == NUM)
            {
                char val[ALENGTH];
                strcpy(val, token);
                if (!ana.setConst(con, val))
                    return dupliDef("常量", con);

                nextToken();
                return SUCCESS;
            }
            else
                return expectedError(NOTEXIST, "无符号整数");
        } else
            return expectedError(NOTEXIST, "'='");
    }
    else
        return NOTEXIST;
}


bool Grammar::keyCheck(char* x)
{
    if (strcmp(x, token) == 0) 
    {
        nextToken();
        return true;
    }
    else return false;
}


void Grammar::nextToken() 
{ 
    returnType re = mor.nextToken(); 
    token = re.pos; tokenType= re.type;
    strcpy(errorInfo, "不能识别的字符");
}

recogTYPE Grammar::dupliDef(char *type, char *token)
{
    strcpy(errorInfo, type);
    strcat(errorInfo, "重复定义:");
    strcat(errorInfo, token);
    return FAIL;
}


recogTYPE Grammar::varState()
{
    if (keyCheck("var"))
    {
        if (tokenType == IDENTI)
        {
            if (!ana.addVar(token))
                return dupliDef("变量", token);

            nextToken();
            while ( keyCheck(",") )
            {
                if (tokenType != IDENTI) 
                    return expectedError(NOTEXIST, "标识符");
                else
                {
                    if (!ana.addVar(token))
                        return dupliDef("变量", token);

                    nextToken();
                }
            }
            if ( keyCheck(";") ) 
                return SUCCESS;
            else
                return expectedError(NOTEXIST, "';'");
        }
        else
            return expectedError(NOTEXIST, "标识符");
    }
    else return NOTEXIST; //没有变量部分
}
void Grammar::run()
{

    if ( program() == SUCCESS )
    {
        ana.show();
    }
    else
    {
        showError();
    }
}
