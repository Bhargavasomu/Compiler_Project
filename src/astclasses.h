#include <bits/stdc++.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>

using namespace std;
using namespace llvm;

extern unordered_map<string,int> VariablesDictionary;
extern unordered_map<string,int> LabelsDictionary;      // stores the index location of the labels in the statements list
extern Function* fooFunc;
extern BasicBlock *createBB(Function *foofunc,string name);
extern map<string,BasicBlock*> labelMap;

class Visitor;
class Interpreter;
class AST;
class Numerical_Predicate;
class A;
class variable;
class GenArray;
class Numerical_Expression;
class expr;
class T;
class argsClass;
class OptionalArgsClass;
class PrintSyntaxClass;
class ElseBlockSyntaxClass;
class StatementListClass;
class IfBlockSyntaxClass;
class readClass;
class WhileLoopSyntaxClass;
class ForLoopDetailsClass;
class ForLoopSyntaxClass;
class AssignmentSyntaxClass;
class GoToSyntaxClass;
class possibilitiesClass;
class LabelDeclarationSyntaxClass;
class OptionalIfClass;
class P;
class codeblock;

/* LLVM Useful Variables */
static Module *TheModule = new Module("B_Compiler_IR",llvm::getGlobalContext());
static LLVMContext &Context = getGlobalContext();
static IRBuilder<> Builder(Context);
class Visitor       // all the traversal algorithms are stored under a single class
{
    public:
        virtual int visit(A* temp){}
        virtual int visit(Numerical_Predicate* temp){}
        virtual int visit(variable* temp){}
        virtual int visit(GenArray* temp){}
        virtual int visit(Numerical_Expression* temp){}
        virtual int visit(expr* temp){}
        virtual int visit(T* temp){}
        virtual int visit(argsClass* temp){}
        virtual int visit(OptionalArgsClass* temp){}
        virtual int visit(PrintSyntaxClass* temp){}
        virtual int visit(ElseBlockSyntaxClass* temp){}
        virtual int visit(StatementListClass* temp){}
        virtual int visit(IfBlockSyntaxClass* temp){}
        virtual int visit(readClass* temp){}
        virtual int visit(WhileLoopSyntaxClass* temp){}
        virtual int visit(ForLoopDetailsClass* temp){}
        virtual int visit(ForLoopSyntaxClass* temp){}
        virtual int visit(AssignmentSyntaxClass* temp){}
        virtual int visit(GoToSyntaxClass* temp){}
        virtual int visit(possibilitiesClass* temp){}
        virtual int visit(LabelDeclarationSyntaxClass* temp){}
        virtual int visit(OptionalIfClass* temp){}
        virtual int visit(P* temp){}
        virtual int visit(codeblock* temp){}
};

class AST
{
    public:
        AST()
        {

        };
        virtual int accept(Visitor* v)=0;
        virtual Value* codegen() {};
};

class P:public AST
{
    public:
        string id;
        string label;
        P()
        {
            id="P";
            label="-1";
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen() {};
};


class variable:public AST       // defined in astclasses.cpp
{
    public:
        string str1;
        int Ctype;
        GenArray* ga1;
        string id;
        variable(GenArray* temp);
        variable(char* temp);
        virtual int accept(Visitor* v);
        virtual Value* codegen();
};

class A:public AST
{
    public:
        int n;
        Numerical_Predicate* np1;
        variable* var1;
        int Ctype;
        string id;
        A(string str)
        {
            id = "A";
            this->Ctype = 1;
            this->n = stoi(str);
        }
        A(Numerical_Predicate* temp)
        {
            id = "A";
            this->Ctype = 2;
            this->np1 = temp;
        }
        A(variable* temp)
        {
            id = "A";
            this->Ctype = 3;
            this->var1 = temp;
        }
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen();   // defined in astclasses.cpp
};

class Numerical_Predicate:public AST
{
    public:
        A* a_ptr2;
        Numerical_Predicate* np2;   // this is the second Half of the Numerical_Predicate
        Numerical_Predicate* np7;   // this is the first half of the Mumerical_Predicate
        string op2;
        int Ctype;
        string id;
        Numerical_Predicate(A* temp)
        {
            id = "NP";
            this->Ctype=1;
            this->a_ptr2 = temp;
        };
        Numerical_Predicate(Numerical_Predicate* temp1,string temp2,Numerical_Predicate* temp3)
        {
            id = "NP";
            this->Ctype=2;
            this->np7 = temp1;
            this->np2 = temp3;
            this->op2 = temp2;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            if(Ctype==1)
            {
                if(a_ptr2->Ctype!=2)
                    return a_ptr2->codegen();
                else
                    return a_ptr2->np1->codegen();
            }
            else
            {
                Value* left = np7->codegen();
                Value* right = np2->codegen();
                Value* v;
                if(op2=="+")
                    v = Builder.CreateAdd(left,right,"addtmp");
                else if (op2 == "-")
                    v = Builder.CreateSub(left,right,"subtmp");
                else if (op2 == "*")
                    v = Builder.CreateMul(left,right,"multmp");
                else if (op2 == "/")
                    v = Builder.CreateUDiv(left,right,"divtmp");
                else if(op2 == "%")
                    v = Builder.CreateURem(left,right,"modtmp");
                else if (op2 == "&")
                    v = Builder.CreateAnd(left,right,"andtmp");
                else if (op2 == "|")
                    v = Builder.CreateOr(left,right,"ortmp");
                else if (op2 == "^")
                    v = Builder.CreateXor(left,right,"xortmp");
                return v;
            }
        };
};

class GenArray:public AST       // defined in astclasses.cpp
{
    public:
        string str2;
        Numerical_Predicate* np3;
        string id;
        GenArray(string temp1,Numerical_Predicate* temp2);
        virtual int accept(Visitor* v);
        virtual Value* codegen();
};

class Numerical_Expression:public AST
{
    public:
        Numerical_Predicate* np4;   // this is the left side
        Numerical_Predicate* np5;
        string symbol;
        string id;
        Numerical_Expression(Numerical_Predicate* temp1,char* temp2,Numerical_Predicate* temp3)
        {
            id="NE";
            this->symbol = temp2;
            this->np4 = temp1;
            this->np5 = temp3;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* left = np4->codegen();
            Value* right = np5->codegen();
            Value* v;
            if(symbol == "<")
                v = Builder.CreateICmpULT(left,right,"ltcomparetmp");
            else if(symbol == ">")
                v = Builder.CreateICmpUGT(left,right,"gtcomparetmp");
            else if(symbol == "<=")
                v = Builder.CreateICmpULE(left,right,"lecomparetmp");
            else if(symbol == ">=")
                v = Builder.CreateICmpUGE(left,right,"gecomparetmp");
            else if(symbol == "==")
                v = Builder.CreateICmpEQ(left,right,"equalcomparetmp");
            return v;
        };
};

class T:public AST
{
    public:
        Numerical_Expression* ne1;
        expr* expr1;
        int Ctype;
        string id;
        T(Numerical_Expression* temp1)
        {
            id="T";
            this->Ctype=1;
            this->ne1 = temp1;
        };
        T(expr* temp2)
        {
            id="T";
            this->Ctype=2;
            this->expr1 = temp2;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen() {};
};

class expr:public AST
{
    public:
        T* t1;
        char* str3;
        expr* expr2;
        int Ctype;
        string id;
        expr(T* temp)
        {
            id="EXPR";
            this->Ctype = 1;
            this->t1 = temp;
        };
        expr(T* temp1,char* temp2,expr* temp3)
        {
            id="EXPR";
            this->Ctype = 2;
            this->t1 = temp1;
            this->str3 = temp2;
            this->expr2 = temp3;
        }
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            if(Ctype==1)
            {
                if(t1->Ctype==1)
                    return t1->ne1->codegen();
                else
                    return t1->expr1->codegen();
            }
        };
};


class readClass:public P
{
    public:
        char* str4;
        variable* var2;
        string id;
        readClass(char* temp1,variable* temp2)
        {
            id="READ";
            this->str4 = temp1;
            this->var2 = temp2;
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* v;
            vector< Value* >args;
            vector< Type* >type;
            string s = "%d";
            Value* x = Builder.CreateGlobalStringPtr(s);
            args.push_back(x);
            type.push_back(x->getType());
            v = var2->codegen();
            args.push_back(v);
            type.push_back(v->getType());
            llvm::ArrayRef<Type*> typeargs(type);
            llvm::ArrayRef<Value*> refargs(args);
            llvm::FunctionType *FType = FunctionType::get(Type::getInt32Ty(Context), typeargs, false);
            Constant* printfunc = TheModule->getOrInsertFunction("scanf", FType);
            return Builder.CreateCall(printfunc,refargs);
        };
};

class argsClass:public AST
{
    public:
        char* str5;     // Has only COMMA token
        variable* var3;
        string id;
        argsClass(char* temp1,variable* temp2)
        {
            id="ARGS";
            this->str5 = temp1;
            this->var3 = temp2;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* v;
            vector< Value* >args;
            vector< Type* >type;
            string s = " %d";
            Value* x = Builder.CreateGlobalStringPtr(s);
            args.push_back(x);
            type.push_back(x->getType());
            v = var3->codegen();
            v = Builder.CreateLoad(v);
            args.push_back(v);
            type.push_back(v->getType());
            llvm::ArrayRef<Type*> typeargs(type);
            llvm::ArrayRef<Value*> refargs(args);
            llvm::FunctionType *FType = FunctionType::get(Type::getInt32Ty(Context), typeargs, false);
            Constant* printfunc1 = TheModule->getOrInsertFunction("printf", FType);
            return Builder.CreateCall(printfunc1,refargs);
        };
};

class OptionalArgsClass:public AST
{
    public:
        argsClass* args1;
        OptionalArgsClass* oa1;
        int Ctype;
        string id;
        OptionalArgsClass(argsClass* temp)
        {
            id="OPTARGS";
            this->Ctype=1;
            this->args1 = temp;
        };
        OptionalArgsClass(argsClass* temp1,OptionalArgsClass* temp2)
        {
            id="OPTARGS";
            this->Ctype=2;
            this->args1 = temp1;
            this->oa1 = temp2;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* v;
            v = args1->codegen();
            if(Ctype==2)
                v = oa1->codegen();
            return v;
        };
};

class PrintSyntaxClass:public P
{
    public:
        string print1;
        string string1;
        int Ctype;      // constructor type
        OptionalArgsClass* oa2;
        variable* var6;
        string id;
        PrintSyntaxClass(char* temp1,char* temp2)
        {
            id="PRINT";
            this->Ctype = 1;
            this->print1 = temp1;
            this->string1 = temp2;
        };
        PrintSyntaxClass(char* temp1,char* temp2,OptionalArgsClass* temp3)
        {
            id="PRINT";
            this->Ctype = 2;
            this->print1 = temp1;
            this->string1 = temp2;
            this->oa2 = temp3;
        };
        PrintSyntaxClass(char* temp1,variable* temp2)
        {
            id="PRINT";
            this->Ctype = 3;
            this->print1 = temp1;
            this->var6 = temp2;
        };
        PrintSyntaxClass(char* temp1,variable* temp2,OptionalArgsClass* temp3)
        {
            id="PRINT";
            this->Ctype = 4;
            this->print1 = temp1;
            this->var6 = temp2;
            this->oa2 = temp3;
        }
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* v;
            if((Ctype==1)||(Ctype==2))
            {
                vector< Value* >args;
                vector< Type* >type;
                string s = string1.substr(1,string1.size()-2);
                Value* x = Builder.CreateGlobalStringPtr(s);
                args.push_back(x);
                type.push_back(x->getType());
                llvm::ArrayRef<Type*> typeargs(type);
                llvm::ArrayRef<Value*> refargs(args);
                llvm::FunctionType *FType = FunctionType::get(Type::getInt32Ty(Context), typeargs, false);
                Constant* printfunc = TheModule->getOrInsertFunction("printf", FType);
                v = Builder.CreateCall(printfunc,refargs);
                if(Ctype==2)
                {
                    Value* v1 = oa2->codegen();
                }
            }
            else
            {
                vector< Value* >args;
                vector< Type* >type;
                string s = "%d";
                Value* x = Builder.CreateGlobalStringPtr(s);
                args.push_back(x);
                type.push_back(x->getType());
                v = var6->codegen();
                v = Builder.CreateLoad(v);
                args.push_back(v);
                type.push_back(v->getType());
                llvm::ArrayRef<Type*> typeargs(type);
                llvm::ArrayRef<Value*> refargs(args);
                llvm::FunctionType *FType = FunctionType::get(Type::getInt32Ty(Context), typeargs, false);
                Constant* printfunc = TheModule->getOrInsertFunction("printf", FType);
                v = Builder.CreateCall(printfunc,refargs);
                if(Ctype==4)
                {
                    Value* v1 = oa2->codegen();
                }
            }

            if(print1=="println")
            {
                vector< Value* >args1;
                vector< Type* >type1;
                string s1 = "\n";
                Value* x1 = Builder.CreateGlobalStringPtr(s1);
                args1.push_back(x1);
                type1.push_back(x1->getType());
                llvm::ArrayRef<Type*> typeargs(type1);
                llvm::ArrayRef<Value*> refargs(args1);
                llvm::FunctionType *FType1 = FunctionType::get(Type::getInt32Ty(Context), typeargs, false);
                Constant* printfunc1 = TheModule->getOrInsertFunction("printf", FType1);
                v = Builder.CreateCall(printfunc1,refargs);
            }
            return v;
        };
};

class StatementListClass:public AST
{
    public:
        vector<P*> statementsVector;
        string id;
        void push(P* temp)
        {
            this->statementsVector.push_back(temp);
        }
        StatementListClass()
        {
            id = "SL";
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* v;
            for(int i=0;i<statementsVector.size();i++)
            {
                v = statementsVector[i]->codegen();
            }
            return v;
        };
};

class ElseBlockSyntaxClass:public AST
{
    public:
        char* str6;     // has only ELSE token
        StatementListClass* sl1;
        string id;
        ElseBlockSyntaxClass(char* temp1,StatementListClass* temp2)
        {
            id="ELSEBLOCK";
            this->str6 = temp1;
            this->sl1 = temp2;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            return sl1->codegen();
        };
};

class IfBlockSyntaxClass:public P
{
    public:
        char* str7;         // this has only IF token
        expr* expr3;
        StatementListClass* sl2;
        ElseBlockSyntaxClass* eb1;
        int Ctype;
        string id;
        IfBlockSyntaxClass(char* temp1,expr* temp2,StatementListClass* temp3,ElseBlockSyntaxClass* temp4)
        {
            id="IFBLOCK";
            this->Ctype=1;
            this->str7 = temp1;
            this->expr3 = temp2;
            this->sl2 = temp3;
            this->eb1 = temp4;
        };
        IfBlockSyntaxClass(char* temp1,expr* temp2,StatementListClass* temp3)
        {
            id="IFBLOCK";
            this->Ctype=2;
            this->str7 = temp1;
            this->expr3 = temp2;
            this->sl2 = temp3;
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value *v = ConstantInt::get(getGlobalContext(), APInt(32,0));
            Value* condition = expr3->codegen();
            BasicBlock *ifbl = createBB(fooFunc,"if");
            BasicBlock *elsebl = createBB(fooFunc,"else");
            BasicBlock *mergebl = createBB(fooFunc,"ifmerge");
            Builder.CreateCondBr(condition,ifbl,elsebl);
            Builder.SetInsertPoint(ifbl);
            Value* ifv;
            Value* elsev;
            ifv = sl2->codegen();
            Builder.CreateBr(mergebl);
            ifbl = Builder.GetInsertBlock();
            Builder.SetInsertPoint(elsebl);
            if(Ctype==1)
                elsev = eb1->codegen();
            Builder.CreateBr(mergebl);
            elsebl = Builder.GetInsertBlock();
            Builder.SetInsertPoint(mergebl);
            return v;
        };
};


class WhileLoopSyntaxClass:public P
{
    public:
        char* str8;         // has only WHILE token
        expr* expr4;
        StatementListClass* sl3;
        string id;
        WhileLoopSyntaxClass(char* temp1,expr* temp2,StatementListClass* temp3)
        {
            id="WHILELOOP";
            this->str8 = temp1;
            this->expr4 = temp2;
            this->sl3 = temp3;
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            BasicBlock *loop = createBB(fooFunc,"loop");
            BasicBlock *after = createBB(fooFunc,"afterloop");
            Value *v3 = expr4->codegen();
            Value *ifcon = Builder.CreateICmpNE(v3,Builder.getInt1(0),"whilecon");
            Builder.CreateCondBr(ifcon, loop, after);
            Builder.SetInsertPoint(loop);
            Value *v = sl3->codegen();
            v3 = expr4->codegen();
            Value *ifcon1 = Builder.CreateICmpNE(v3,Builder.getInt1(0),"whilecon");
            Builder.CreateCondBr(ifcon1, loop, after);
            Builder.SetInsertPoint(after);
        };
};

class ForLoopDetailsClass:public AST
{
    public:
        Numerical_Predicate* start;
        Numerical_Predicate* end;
        Numerical_Predicate* increment;
        int Ctype;
        string id;
        ForLoopDetailsClass(Numerical_Predicate* temp1,Numerical_Predicate* temp2)
        {
            id="FORLOOP";
            this->start = temp1;
            this->end = temp2;
            this->Ctype = 1;
        };
        ForLoopDetailsClass(Numerical_Predicate* temp1,Numerical_Predicate* temp2,Numerical_Predicate* temp3)
        {
            id="FORLOOPDET";
            this->start = temp1;
            this->end = temp2;
            this->increment = temp3;
            this->Ctype = 2;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen() {};
};

class ForLoopSyntaxClass:public P
{
    public:
        char* str9; // has only FOR token
        variable* var4;
        char* str10;    // has only EQUAL token
        ForLoopDetailsClass* fld1;
        StatementListClass* sl4;
        string id;
        ForLoopSyntaxClass(char* temp1,variable* temp2,char* temp3,ForLoopDetailsClass* temp4,StatementListClass* temp5)
        {
            id="FORLOOP";
            this->str9 = temp1;
            this->var4 = temp2;
            this->str10 = temp3;
            this->fld1 = temp4;
            this->sl4 = temp5;
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* val = fld1->start->codegen();
            BasicBlock *loop=createBB(fooFunc,"loop");
            BasicBlock *after=createBB(fooFunc,"afterloop");
            Value* a = var4->codegen();
            Value* v3 = fld1->end->codegen();
            Value *v34=Builder.CreateStore(val,a);
            Value *v45=Builder.CreateLoad(a);
            Value *end1=Builder.CreateICmpULT(val,v3,"condi");
            Builder.CreateCondBr(end1, loop, after);
            Builder.SetInsertPoint(loop);
            Value *v=sl4->codegen();
            Value* inc;
            if(fld1->Ctype==1)
                inc = Builder.getInt32(1);
            else
                inc = fld1->increment->codegen();
            Value *ty=Builder.CreateLoad(a);
            Value *re=Builder.CreateAdd(ty,inc,"next");
            Builder.CreateStore(re,a);
            Value *end=Builder.CreateICmpULT(re,v3,"loopcond");
            Builder.CreateCondBr(end, loop, after);
            Builder.SetInsertPoint(after);
        };
};

class AssignmentSyntaxClass:public P
{
    public:
        variable* var5;
        char* str11;    // has only Equal Token
        Numerical_Predicate* np6;
        string id;
        AssignmentSyntaxClass(variable* temp1,char* temp2,Numerical_Predicate* temp3)
        {
            id="ASSGN";
            this->var5=temp1;
            this->str11 = temp2;
            this->np6 = temp3;
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Value* v = var5->codegen();
            Value* leftSideVariable = v;
            Value* rightSideValue = np6->codegen();
            Value* x = Builder.CreateStore(rightSideValue,leftSideVariable);
            return x;
        };
};

class OptionalIfClass:public AST
{
    public:
        char* str15;    // stores only IF token
        expr* expr5;
        string id;
        OptionalIfClass(char* temp1,expr* temp2)
        {
            id="OIF";
            this->str15 = temp1;
            this->expr5 = temp2;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen() {};
};

class GoToSyntaxClass:public P
{
    public:
        char* str16;    // stores only GOTO token
        string str17;    // stores IDENTIFIER token
        OptionalIfClass* oif1;
        int Ctype;
        string id;
        GoToSyntaxClass(char* temp1,char* temp2,OptionalIfClass* temp3)
        {
            id="GOTO";
            this->str16 = temp1;
            this->str17 = temp2;
            this->oif1 = temp3;
            this->Ctype = 1;
        };
        GoToSyntaxClass(char* temp1,char* temp2)
        {
            id="GOTO";
            this->str16 = temp1;
            this->str17 = temp2;
            this->Ctype = 2;
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            if(Ctype==1)
            {
                BasicBlock* x = labelMap[str17];
                BasicBlock *aftergoto = BasicBlock::Create(Context,"aftergoto",fooFunc);
                Value* con = oif1->expr5->codegen();
                Builder.CreateCondBr(con,x,aftergoto);
                Builder.SetInsertPoint(aftergoto);
            }
            else
            {
                BasicBlock* aftergoto = createBB(fooFunc,"GotoUnconditional");
                Builder.CreateBr(labelMap[str17]);
                Builder.SetInsertPoint(aftergoto);
            }
        };
};

class possibilitiesClass:public AST
{
    public:
        char* str12;    // Contains only SEMICOLON token
        AssignmentSyntaxClass* as1;
        PrintSyntaxClass* ps1;
        readClass* rs1;
        GoToSyntaxClass* goto1;
        int Ctype;
        string id;
        possibilitiesClass(AssignmentSyntaxClass* temp1,char* temp2)
        {
            id="POSS";
            this->as1 = temp1;
            this->str12 = temp2;
            this->Ctype=1;
        };
        possibilitiesClass(PrintSyntaxClass* temp1,char* temp2)
        {
            id="POSS";
            this->ps1 = temp1;
            this->str12 = temp2;
            this->Ctype=2;
        };
        possibilitiesClass(readClass* temp1,char* temp2)
        {
            id="POSS";
            this->rs1 = temp1;
            this->str12 = temp2;
            this->Ctype=3;
        };
        possibilitiesClass(GoToSyntaxClass* temp1,char* temp2)
        {
            id="POSS";
            this->goto1 = temp1;
            this->str12 = temp2;
            this->Ctype=4;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            if(Ctype==1)
                return as1->codegen();
            else if(Ctype==2)
                return ps1->codegen();
            else if(Ctype==3)
                return rs1->codegen();
            else
                return goto1->codegen();
        };
};



class LabelDeclarationSyntaxClass:public P
{
    public:
        string str13;    // stores IDENTIFIER token
        string str14;    // stores only COLON token
        possibilitiesClass* pos1;
        string id;
        LabelDeclarationSyntaxClass(string temp1,string temp2,possibilitiesClass* temp3)
        {
            id="LABEL";
            this->str13 = temp1;
            this->str14 = temp2;
            this->pos1 = temp3;
            label = str13;
        };
        virtual string getID()
        {
            return id;
        };
        virtual string getLabel()
        {
            return label;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            Builder.CreateBr(labelMap[str13]);
            Builder.SetInsertPoint(labelMap[str13]);
            pos1->codegen();
        };
};

class codeblock:public AST
{
    public:
        StatementListClass* sl5;
        string id;
        codeblock(StatementListClass* temp)
        {
            id="CODEBLOCK";
            this->sl5 = temp;
        };
        virtual int accept(Visitor* v)
		{
			return v->visit(this);
		};
        virtual Value* codegen()
        {
            return sl5->codegen();
        };
};

class Interpreter:public Visitor
{
    public:
        Interpreter()
        {

        };
        int visit(A* temp)
        {
            if(temp->Ctype==1)
                return temp->n;
            else if(temp->Ctype==2)
                return temp->np1->accept(this);
            else
                return temp->var1->accept(this);
            return 0;
        }
        int visit(Numerical_Predicate* temp)
        {
            if(temp->Ctype==1)
            {
                return temp->a_ptr2->accept(this);
            }
            else
            {
                int LHS = temp->np7->accept(this);
                int RHS = temp->np2->accept(this);
                string operation = temp->op2;
                if(operation=="+")
                    return (LHS + RHS);
                else if(operation=="-")
                    return (LHS - RHS);
                else if(operation=="*")
                    return (LHS * RHS);
                else if(operation=="/")
                    return (LHS / RHS);
                else if(operation=="%")
                    return (LHS % RHS );
                else if(operation=="&")
                    return (LHS & RHS);
                else if(operation=="|")
                    return (LHS | RHS);
                else if(operation=="^")
                    return (LHS ^ RHS);
            }
            return 0;
        }
        int visit(variable* temp)       // Gives the values of the variable
        {
            if(temp->Ctype==1)
                return VariablesDictionary[temp->str1];
            else
            {
                string varPartInArray = temp->ga1->str2 + '[';
                int valuePartInArray = temp->ga1->np3->accept(this);
                varPartInArray += to_string(valuePartInArray);
                varPartInArray += ']';
                return VariablesDictionary[varPartInArray];
            }
            return 0;
        }
        int visit(Numerical_Expression* temp)
        {
            int LHS = temp->np4->accept(this);
            int RHS = temp->np5->accept(this);
            string symb = temp->symbol;
            if(symb=="<")
                return (LHS < RHS);
            else if(symb==">")
                return (LHS > RHS);
            else if(symb=="==")
                return (LHS == RHS);
            else if(symb==">=")
                return (LHS >= RHS);
            else if(symb=="<=")
                return (LHS <= RHS);
            return 0;
        }
        int visit(expr* temp)
        {
            if(temp->Ctype==1)
                return temp->t1->accept(this);
            else
            {
                int LHS = temp->t1->accept(this);
                int RHS = temp->expr2->accept(this);
                string symb = temp->str3;
                if(symb=="and")
                    return (LHS && RHS);
                else if(symb=="or")
                    return (LHS || RHS);
            }
            return 0;
        }
        int visit(T* temp)
        {
            if(temp->Ctype==1)
                return temp->ne1->accept(this);
            else
                return temp->expr1->accept(this);
        }
        int visit(argsClass* temp)
        {
            return temp->var3->accept(this);
        }
        int visit(OptionalArgsClass* temp)
        {
            if(temp->Ctype==1)
            {
                cout << " " << temp->args1->accept(this);
            }
            else
            {
                cout << " " << temp->args1->accept(this);
                temp->oa1->accept(this);
            }
            return 0;
        }
        int visit(PrintSyntaxClass* temp)
        {
            string toPrintString = temp->string1;
            if((temp->Ctype) == 1)
            {
                toPrintString = toPrintString.substr(1,toPrintString.size()-2);
                cout << toPrintString;
                if((temp->print1)=="println")
                    cout << endl;
            }
            else if((temp->Ctype)==2)
            {
                toPrintString = toPrintString.substr(1,toPrintString.size()-2);
                cout << toPrintString;
                int waste = temp->oa2->accept(this);
                if((temp->print1)=="println")
                    cout << endl;
            }
            else if((temp->Ctype)==3)
            {
                int value = temp->var6->accept(this);
                cout << value;
                if((temp->print1)=="println")
                    cout << endl;
            }
            else
            {
                int value = temp->var6->accept(this);
                cout << value;
                int waste = temp->oa2->accept(this);
                if((temp->print1)=="println")
                    cout << endl;
            }
            return 0;
        }
        int visit(readClass* temp)
        {
            if(temp->var2->Ctype==1)
            {
                cin >> VariablesDictionary[temp->var2->str1];
            }
            else
            {
                string varPartInArray = temp->var2->ga1->str2 + '[';
                int valuePartInArray = temp->var2->ga1->np3->accept(this);
                varPartInArray += to_string(valuePartInArray);
                varPartInArray += ']';
                cin >> VariablesDictionary[varPartInArray];
            }
            return 0;
        }
        int visit(StatementListClass* temp)
        {
            int i,j;
            for(i=0;i<temp->statementsVector.size();i++)
            {
                if((temp->statementsVector[i]->getID())=="LABEL")    // Means that this is a labelClass
                {
                    LabelsDictionary[temp->statementsVector[i]->getLabel()] = i;
                }
            }
            i=0;
            while(i<(temp->statementsVector.size()))
            {
                if((temp->statementsVector[i]->getID())=="GOTO")    // means that goto syntax was called
                {
                    j = temp->statementsVector[i]->accept(this);
                    if(j == -1)
                        i++;
                    else
                        i=j;
                }
                else
                {
                    int x = temp->statementsVector[i]->accept(this);
                    i++;
                }
            }
            return 0;
        }
        int visit(IfBlockSyntaxClass* temp)
        {
            int trueFlag = temp->expr3->accept(this);
            if(trueFlag==1)
            {
                return temp->sl2->accept(this);
            }
            else
            {
                if((temp->Ctype)==2)
                    return 0;
                else
                    return temp->eb1->accept(this);
            }
            return 0;
        }
        int visit(ElseBlockSyntaxClass* temp)
        {
            return temp->sl1->accept(this);
        }
        int visit(WhileLoopSyntaxClass* temp)
        {
            int waste;
            int exprValue = temp->expr4->accept(this);
            while(exprValue==1)
            {
                waste = temp->sl3->accept(this);
                exprValue = temp->expr4->accept(this);
            }
            return 0;
        }
        int visit(ForLoopSyntaxClass* temp)
        {
            string varName;
            int startNumber = temp->fld1->start->accept(this);
            int endNumber = temp->fld1->end->accept(this);
            int inc=1,waste;
            if((temp->fld1->Ctype)==2)
                inc=temp->fld1->increment->accept(this);
            // find the variable
            if(temp->var4->Ctype==1)
                varName = temp->var4->str1;
            else
            {
                string varPartInArray = temp->var4->ga1->str2 + '[';
                int valuePartInArray = temp->var4->ga1->np3->accept(this);
                varPartInArray += to_string(valuePartInArray);
                varPartInArray += ']';
                varName = varPartInArray;
            }
            int iterator;
            for(iterator=startNumber;iterator<endNumber;iterator+=inc)
            {
                VariablesDictionary[varName] = iterator;
                waste = temp->sl4->accept(this);
            }
            VariablesDictionary[varName] = iterator;
            return 0;
        }
        int visit(AssignmentSyntaxClass* temp)
        {
            if(temp->var5->Ctype==1)
                VariablesDictionary[temp->var5->str1] = temp->np6->accept(this);
            else
            {
                string varPartInArray = temp->var5->ga1->str2 + '[';
                int valuePartInArray = temp->var5->ga1->np3->accept(this);
                varPartInArray += to_string(valuePartInArray);
                varPartInArray += ']';
                int exprValue = temp->np6->accept(this);
                VariablesDictionary[varPartInArray] = exprValue;
            }
            return 0;
        }
        int visit(GoToSyntaxClass* temp)
        {
            int i;
            if((temp->Ctype)==2)
                i=LabelsDictionary[temp->str17];
            else
            {
                if((temp->oif1->accept(this))==1)
                    i=LabelsDictionary[temp->str17];
                else
                    i=-1;
            }
            return i;
        }
        int visit(possibilitiesClass* temp)
        {
            if((temp->Ctype)==1)
            {
                temp->as1->accept(this);
            }
            else if((temp->Ctype)==2)
            {
                temp->ps1->accept(this);
            }
            else if((temp->Ctype)==3)
            {
                temp->rs1->accept(this);
            }
            else if((temp->Ctype)==4)
            {
                temp->goto1->accept(this);
            }
            return 0;
        }
        int visit(LabelDeclarationSyntaxClass* temp)
        {
            return temp->pos1->accept(this);
        }
        int visit(OptionalIfClass* temp)
        {
            return temp->expr5->accept(this);
        }
        int visit(codeblock* temp)
        {
            int x = temp->sl5->accept(this);
            return 0;
        };
};
