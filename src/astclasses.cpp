#include "astclasses.h"

inline variable::variable(GenArray* temp)
{
    id="var";
    this->ga1 = temp;
    this->Ctype = 2;
}

inline variable::variable(char* temp)
{
    id="var";
    this->str1 = temp;
    this->Ctype = 1;
};

inline int variable::accept(Visitor* v)
{
    return v->visit(this);
};

inline Value* variable::codegen()
{
    Value* v;
    if(Ctype==1)
    {
        size_t found = str1.find_first_of("[");
        if(found <= str1.size())
        {
            string loc = "";
            int flag=0;
            string num = "";
            int i=0;
            while( (str1[i]!= '\0') && (str1[i]!= ']'))
            {
                if(str1[i]=='[')
                {
                    flag=1;
                    i++;
                    continue;
                }
                if(flag==0)
                    loc += str1[i];
                else
                    num += str1[i];
                i++;
            }
            v = TheModule->getGlobalVariable(loc);
            Value* pos = Builder.getInt32(stoi(num));
            vector<Value*> arrpos;
            arrpos.push_back(Builder.getInt32(0));
            arrpos.push_back(pos);
            v = Builder.CreateGEP(v,arrpos,"Pos");
        }
        else
        {
            v = TheModule->getNamedGlobal(str1);
        }
    }
    else
    {
        v = ga1->codegen();
    }
    return v;
};

inline GenArray::GenArray(string temp1,Numerical_Predicate* temp2)
{
    id="genarray";
    this->str2 = temp1;
    this->np3 = temp2;
};

inline int GenArray::accept(Visitor* v)
{
    return v->visit(this);
};

inline Value* GenArray::codegen()
{
    Value* v = TheModule->getGlobalVariable(str2);
    Value* pos = np3->codegen();
    vector<Value*> arrpos;
    arrpos.push_back(Builder.getInt32(0));
    arrpos.push_back(pos);
    v = Builder.CreateGEP(v,arrpos,"Pos");
    return v;
};

inline Value* A::codegen()
{
    if(Ctype==1)
    {
        // store the integer in a register
        Value* constant = Builder.getInt32(n);
        return constant;
    }
    else if(Ctype==3)
    {
        Value* v = var1->codegen();
        v = Builder.CreateLoad(v);
        return v;
    }
};
