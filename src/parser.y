%{
  #define YYDEBUG 1
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <bits/stdc++.h>
  #include "requiredStructs.h"
  #include "astclasses.cpp"
  using namespace std;
  extern FILE *yyin;
  extern char* yytext;
  int yylex (void);
  void yyerror (char const *s);
  char* variables[1005];        /* Can have only 1005 variables at max */
  int variablesInd = -1;
  char* array1[1005];
  int arrayInd = -1;
  char* labels[1005];
  int labelsInd = -1;
  vector<string> gotolabelsVector;
  int parseErrorFlag = 0;
  void checkAndPushVariable(char* str);
  void ParseCheckPushArray(char* str);
  void checkVariableDeclaration(char* str);
  void checkGotoLabelDeclaration();
  void checkArrayDeclaration(char* str,int flag);
  void checkVariableArray(char* str);
  void pushLabelStack(char* str);
  void pushGotoLabelStack(string str);
  void checkLabelDeclaration(char* str);
  void pushArray(char* str);
  void codegenFunc(string Name,int type,int declaredLength);
  Function *createFunc(IRBuilder<> &Builder,string name);
  BasicBlock *createBB(Function *foofunc,string name);
  void setFuncArgs(Function* fooFunc,vector<string> FunArgs);
  void BasicBuildLLVM(Module * TheModule);
  codeblock* CB;
  unordered_map<string,int> VariablesDictionary;
  unordered_map<string,int> LabelsDictionary;
  vector<string> FunArgs;
  Function *fooFunc;
  map<string,BasicBlock*> labelMap;
%}

%union
{
    char* namer;
    Ref structType;
    A* temp;
    variable* VAR;
    Numerical_Predicate* NP;
    GenArray* GA;
    Numerical_Expression* NE;
    expr* EXPR;
    T* TT;
    readClass* RS;
    argsClass* ARGS;
    OptionalArgsClass* OE;
    PrintSyntaxClass* PS;
    IfBlockSyntaxClass* IB;
    ElseBlockSyntaxClass* EB;
    StatementListClass* SL;
    WhileLoopSyntaxClass* WL;
    ForLoopDetailsClass* FLD;
    ForLoopSyntaxClass* FLS;
    AssignmentSyntaxClass* AS;
    possibilitiesClass* POS;
    GoToSyntaxClass* GT;
    LabelDeclarationSyntaxClass* LDS;
    OptionalIfClass* OIF;
    P* ppt;
    codeblock* CBLOCK;
}

%token <namer> NUMBER
%token <namer> IDENTIFIER
%token ETOK
%token DATATYPE
%token <namer> ARRAY_NUM
%token <namer> ARRAY_LETTER
%token <namer> SEMICOLON
%token <namer> COLON
%token DOT
%token QUESTIONMARK
%token <namer> COMMA
%token DECLBLOCK
%token CODEBLOCK

%token <namer> LESSER
%token <namer> GREATER
%token <namer> EQUAL
%token <namer> DOUBLE_EQUAL
%token <namer> GREATER_EQUAL
%token <namer> LESSER_EQUAL

%token <namer> AND
%token <namer> OR
%token <namer> LOGICAL_AND
%token <namer> LOGICAL_OR
%token <namer> LOGICAL_XOR

%token <namer> FOR
%token <namer> WHILE
%token <namer> IF
%token <namer> ELSE

%token <namer> PRINT_WITHOUT_NEW_LINE
%token <namer> PRINT_WITH_NEW_LINE
%token <namer> READ
%token <namer> GOTO

%token SINGLEQUOTE
%token DOUBLEQUOTE
%token <namer> STRING

/* LOGICAL_AND is & */
%left EQUAL
%left OR
%left AND
%left LOGICAL_OR
%left LOGICAL_XOR
%left LOGICAL_AND
%left DOUBLE_EQUAL
%left GREATER GREATER_EQUAL
%left LESSER LESSER_EQUAL
%left <namer> '+'
%left <namer> '-'
%left <namer> '*'
%left <namer> '/'
%left <namer> '%'


%type <structType> Conjunctions
%type <structType> Comparators
%type <structType> symbols
%type <structType> print
%type <temp> A
%type <VAR> variable
%type <NP> Numerical_Predicate
%type <GA> GenArray
%type <NE> Numerical_Expression
%type <EXPR> EXPRESSION
%type <TT> T
%type <RS> ReadSyntax;
%type <ARGS> args;
%type <OE> OptionalArgs
%type <PS> PrintSyntax
%type <EB> ElseBlockSyntax
%type <SL> statement_list
%type <IB> IfBlockSyntax
%type <WL> WhileLoopSyntax
%type <FLD> ForLoopDetails
%type <FLS> ForLoopSyntax
%type <GT> GoToSyntax
%type <POS> possibilities
%type <AS> AssignmentSyntax
%type <LDS> LabelDeclarationSyntax
%type <OIF> OptionalIf
%type <ppt> P
%type <CBLOCK> code_block

%%

program : decl_block code_block

decl_block: DECLBLOCK '{' declaration_list '}'
declaration_list : single_declaration | declaration_list single_declaration
single_declaration : DATATYPE var SEMICOLON
var : IDENTIFIER{checkAndPushVariable($1);codegenFunc($1,0,0);} | ARRAY_NUM{ParseCheckPushArray($1);} | var COMMA IDENTIFIER{checkAndPushVariable($3);codegenFunc($3,0,0);} | var COMMA ARRAY_NUM{ParseCheckPushArray($3);}

/* Syntax of Generalized Array */
GenArray : IDENTIFIER '[' Numerical_Predicate ']' {checkVariableArray($1);$$ = new GenArray($1,$3);}

code_block: CODEBLOCK '{' statement_list '}' {$$ = new codeblock($3);CB=$$;}
Conjunctions : AND{strcpy($$.val,$1);} | OR{strcpy($$.val,$1);}
Comparators : LESSER{strcpy($$.val,$1);} | GREATER{strcpy($$.val,$1);} | DOUBLE_EQUAL{strcpy($$.val,$1);} | GREATER_EQUAL{strcpy($$.val,$1);} | LESSER_EQUAL{strcpy($$.val,$1);}
variable : IDENTIFIER{checkVariableDeclaration($1);$$ = new variable($1);} | ARRAY_NUM{checkArrayDeclaration($1,0);$$ = new variable($1);} | GenArray{$$ = new variable($1);}

/* Below is the Grammar for statement_list */
statement_list : statement_list P{$$=$1;$$->push($2);} | P{$$ = new StatementListClass();$$->push($1);}
P : AssignmentSyntax SEMICOLON{$$ = $1;} | PrintSyntax SEMICOLON{$$ = $1;} | ReadSyntax SEMICOLON{$$ = $1;} | ForLoopSyntax{$$ = $1;} | WhileLoopSyntax{$$ = $1;} | IfBlockSyntax{$$ = $1;} | LabelDeclarationSyntax{$$ = $1;} | GoToSyntax SEMICOLON{$$ = $1;}

/* Below Grammar is for the AssignmentSyntax (a=4,a[i]=3) */
AssignmentSyntax : variable EQUAL Numerical_Predicate {$$ = new AssignmentSyntaxClass($1,$2,$3);}

/* Below Grammar is for the Label Declaration */
LabelDeclarationSyntax : IDENTIFIER COLON possibilities {pushLabelStack($1);$$ = new LabelDeclarationSyntaxClass($1,$2,$3);BasicBlock* thenbb=createBB(fooFunc,string($1));labelMap[string($1)]=thenbb;}
possibilities : AssignmentSyntax SEMICOLON{$$ = new possibilitiesClass($1,$2);} | PrintSyntax SEMICOLON {$$ = new possibilitiesClass($1,$2);}| ReadSyntax SEMICOLON{$$ = new possibilitiesClass($1,$2);} | GoToSyntax SEMICOLON{$$ = new possibilitiesClass($1,$2);}

/* Below Grammar is for the GOTO Syntax */
GoToSyntax : GOTO IDENTIFIER OptionalIf {pushGotoLabelStack($2);$$ = new GoToSyntaxClass($1,$2,$3);} | GOTO IDENTIFIER {pushGotoLabelStack($2);$$ = new GoToSyntaxClass($1,$2);}
OptionalIf : IF EXPRESSION {$$ = new OptionalIfClass($1,$2);}

/* Below Grammar is for the FOR LOOP */
ForLoopSyntax : FOR variable EQUAL ForLoopDetails '{' statement_list '}' {$$ = new ForLoopSyntaxClass($1,$2,$3,$4,$6);}
ForLoopDetails : Numerical_Predicate COMMA Numerical_Predicate{$$ = new ForLoopDetailsClass($1,$3);} | Numerical_Predicate COMMA Numerical_Predicate COMMA Numerical_Predicate{$$ = new ForLoopDetailsClass($1,$3,$5);}

/* Below Grammar is for the WHILE LOOP */
WhileLoopSyntax : WHILE EXPRESSION '{' statement_list '}' {$$ = new WhileLoopSyntaxClass($1,$2,$4);}

/* Below Grammar is for the IF ELSE LOOP */
IfBlockSyntax : IF EXPRESSION '{' statement_list '}' ElseBlockSyntax {$$ = new IfBlockSyntaxClass($1,$2,$4,$6);} | IF EXPRESSION '{' statement_list '}'{$$ = new IfBlockSyntaxClass($1,$2,$4);}
ElseBlockSyntax : ELSE '{' statement_list '}' {$$ = new ElseBlockSyntaxClass($1,$3);}

/* Below Grammar is for the PRINT Syntax */
PrintSyntax : print STRING OptionalArgs{$$ = new PrintSyntaxClass($1.val,$2,$3);} | print STRING{$$ = new PrintSyntaxClass($1.val,$2);} | print variable{$$ = new PrintSyntaxClass($1.val,$2);} | print variable OptionalArgs{$$ = new PrintSyntaxClass($1.val,$2,$3);}
print : PRINT_WITHOUT_NEW_LINE{strcpy($$.val,$1);} | PRINT_WITH_NEW_LINE{strcpy($$.val,$1);}
OptionalArgs : args OptionalArgs{$$ = new OptionalArgsClass($1,$2);} | args{$$ = new OptionalArgsClass($1);}
args : COMMA variable{$$ = new argsClass($1,$2);}

/* Below Grammar is for the READ Syntax */
ReadSyntax : READ variable {$$ = new readClass($1,$2);}

/* Start of the Grammar of 'Numerical_Predicate' */
Numerical_Predicate : Numerical_Predicate '+' Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      Numerical_Predicate '-' Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      Numerical_Predicate '*' Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      Numerical_Predicate '/' Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      Numerical_Predicate '%' Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      Numerical_Predicate LOGICAL_AND Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      Numerical_Predicate LOGICAL_OR Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      Numerical_Predicate LOGICAL_XOR Numerical_Predicate{$$ = new Numerical_Predicate($1,$2,$3);}
                      |
                      A{$$ = new Numerical_Predicate($1);}
A : variable{$$ = new A($1);} | NUMBER{$$=new A(string($1));} | '(' Numerical_Predicate ')'{$$ = new A($2);}

/* Start of the Grammar of 'Numerical_Expression' */
Numerical_Expression : Numerical_Predicate symbols Numerical_Predicate{$$ = new Numerical_Expression($1,$2.val,$3);}
symbols : Comparators{strcpy($$.val,$1.val);}

/* Start of the Grammar of 'Logical_Expression' */
EXPRESSION : T Conjunctions EXPRESSION{$$ = new expr($1,$2.val,$3);} | T{$$ = new expr($1);}
T : Numerical_Expression{$$ = new T($1);} | '(' EXPRESSION ')'{$$ = new T($2);}

/* End of the Grammar for 'Expression' */


%%

void yyerror (char const *s)
{
    parseErrorFlag = 1;         // Error has occurred in Parsing Stage
    fprintf (stderr, "%s\n", s);
    printf("%s\n",yytext);
}

void checkAndPushVariable(char* str)
{
    int i;
    int foundFlag=0;
    for(i=0;i<=variablesInd;i++)
    {
        if(strcmp(str,variables[i])==0)
        {
            foundFlag=1;
            break;
        }
    }
    if(foundFlag==0)
    {
        variablesInd++;
        variables[variablesInd] = (char *)malloc(sizeof(100));
        strcpy(variables[variablesInd],str);
        return;
    }
    else
    {
        char* forwardStr = (char *)malloc(sizeof(120));
        strcpy(forwardStr,"\nError : Variable ");
        strcat(forwardStr,str);
        strcat(forwardStr," is declared More Than Once");
        yyerror(forwardStr);
        exit(0);
    }
}

void pushArray(char* str)
{
    int foundFlag=0,i;
    for(i=0;i<=arrayInd;i++)
    {
        if(strcmp(str,array1[i])==0)
        {
            foundFlag=1;
            break;
        }
    }
    if(foundFlag==0)
    {
        arrayInd++;
        array1[arrayInd] = (char *)malloc(sizeof(100));
        strcpy(array1[arrayInd],str);
        return;
    }
    else
    {
        char* forwardStr = (char *)malloc(sizeof(120));
        strcpy(forwardStr,"\nError : Array ");
        strcat(forwardStr,str);
        strcat(forwardStr," is declared More Than Once");
        yyerror(forwardStr);
        exit(0);
    }
}

void ParseCheckPushArray(char* str)
{
    int i=0,j;
    while((str[i]!='\0')&&(str[i]!='['))
    {
        i++;
    }
    char *tempStr1 = (char *)malloc(sizeof(char)*120);
    char *size = (char *)malloc(sizeof(char)*120);
    j=i+1;
    while(str[j]!=']')
    {
        size[j-i-1] = str[j];
        j++;
    }
    size[j-i-1] = '\0';
    strncpy(tempStr1,str,i);
    pushArray(tempStr1);
    codegenFunc(string(tempStr1),1,stoi(size));
}

void checkVariableDeclaration(char* str)
{
    int i;
    int foundFlag=0;
    for(i=0;i<=variablesInd;i++)
    {
        if(strcmp(str,variables[i])==0)
        {
            foundFlag=1;
            break;
        }
    }
    if(foundFlag==1)
        return;
    else
    {
        char* forwardStr = (char *)malloc(sizeof(120));
        strcpy(forwardStr,"\nVariable ");
        strcat(forwardStr,str);
        strcat(forwardStr," is not declared");
        yyerror(forwardStr);
        exit(0);
    }
}

void checkArrayDeclaration(char* str,int flag)
{
    int i=0,j;
    while((str[i]!='\0')&&(str[i]!='['))
    {
        i++;
    }
    char *tempStr1 = (char *)malloc(sizeof(char)*120);
    char *tempStr2 = (char *)malloc(sizeof(char)*120);
    strncpy(tempStr1,str,i);
    j=i+1;
    while((str[j]!='\0')&&(str[j]!=']'))
    {
        tempStr2[j-i-1]=str[j];
        j++;
    }
    tempStr2[j-1]='\0';
    int foundFlag=0;
    for(i=0;i<=arrayInd;i++)
    {
        if(strcmp(tempStr1,array1[i])==0)
        {
            foundFlag=1;
            break;
        }
    }
    if(foundFlag==1)
        return;
    else
    {
        char* forwardStr = (char *)malloc(sizeof(120));
        strcpy(forwardStr,"\nArray ");
        strcat(forwardStr,tempStr1);
        strcat(forwardStr," is not declared");
        yyerror(forwardStr);
        exit(0);
    }
    if(flag==1)
        checkVariableDeclaration(tempStr2);
}

void checkVariableArray(char* str)
{
    int i;
    int foundFlag=0;
    for(i=0;i<=arrayInd;i++)
    {
        if(strcmp(str,array1[i])==0)
        {
            foundFlag=1;
            break;
        }
    }
    if(foundFlag==1)
        return;
    else
    {
        char* forwardStr = (char *)malloc(sizeof(120));
        strcpy(forwardStr,"\nArray ");
        strcat(forwardStr,str);
        strcat(forwardStr," is not declared");
        yyerror(forwardStr);
        exit(0);
    }
}

void pushLabelStack(char* str)
{
    int i;
    int foundFlag=0;
    for(i=0;i<=labelsInd;i++)
    {
        if(strcmp(str,labels[i])==0)
        {
            foundFlag=1;
            break;
        }
    }
    if(foundFlag==0)
    {
        labelsInd++;
        labels[labelsInd] = (char *)malloc(sizeof(100));
        strcpy(labels[labelsInd],str);
        return;
    }
    else
    {
        char* forwardStr = (char *)malloc(sizeof(120));
        strcpy(forwardStr,"\nError : Label ");
        strcat(forwardStr,str);
        strcat(forwardStr," is declared More Than Once");
        yyerror(forwardStr);
        exit(0);
    }
}

void pushGotoLabelStack(string str)
{
    gotolabelsVector.push_back(str);
    return;
}

void checkGotoLabelDeclaration()
{
    int i,j;
    int labelFoundFlag;
    int crctFlag = 0;
    string str;
    for(i=0;i<gotolabelsVector.size();i++)
    {
        // check if gotolabelsVector[i] is a valid label
        labelFoundFlag=0;
        for(j=0;j<=labelsInd;j++)
        {
            str = labels[j];
            if(str==gotolabelsVector[i])
            {
                labelFoundFlag=1;
                break;
            }
        }
        if(labelFoundFlag==1)
            continue;
        else
        {
            crctFlag=1;
            break;
        }
    }
    if(crctFlag==1)
    {
        string forwardStr = "\nLabel " + gotolabelsVector[i] + " is not declared";
        const char *cstr = forwardStr.c_str();
        yyerror(cstr);
    }
}

void codegenFunc(string name,int type,int len)  // type = 0 for int, 1 for array
{
    if(type==0)
    {
        TheModule->getOrInsertGlobal(name,Builder.getInt32Ty());
        GlobalVariable *gvar=TheModule->getNamedGlobal(name);
        gvar->setLinkage(GlobalValue::CommonLinkage);
        gvar->setAlignment(4);
        ConstantInt* const_int_val = ConstantInt::get(Context, APInt(32,0));
        gvar->setInitializer(const_int_val);
    }
    else
    {
        Type *ty;
        ty = Type::getInt32Ty(Context);
        ArrayType* arrType = ArrayType::get(ty,(unsigned int)len);
        PointerType* PointerTy_1 = PointerType::get(ArrayType::get(ty,(unsigned int)len),0);
        GlobalVariable* gv = new GlobalVariable(*TheModule,arrType,false,GlobalValue::ExternalLinkage,0,name);
        gv->setInitializer(ConstantAggregateZero::get(arrType));
    }
    Value* v = ConstantInt::get(getGlobalContext(), APInt(32,1));
}

Function *createFunc(IRBuilder<> &Builder,string name)
{
    std::vector<Type *> Integers(FunArgs.size(),Type::getInt32Ty(Context));
    FunctionType *funcType = llvm::FunctionType::get(Builder.getInt32Ty(),Integers,false);
    Function* fooFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage,name,TheModule);
    return fooFunc;
}
BasicBlock *createBB(Function *foofunc,string name)
{
    return BasicBlock::Create(Context,name,foofunc);
}
void setFuncArgs(Function* fooFunc,vector<string> FunArgs)
{
    unsigned Idx = 0;
    Function::arg_iterator AI,AE;
    for(AI = fooFunc->arg_begin(),AE = fooFunc->arg_end(); AI!=AE; ++AI,++Idx)
    {
    AI->setName(FunArgs[Idx]);
    }
}
void BasicBuildLLVM(Module * TheModule)
{
    fooFunc = createFunc(Builder,"main");
    BasicBlock  *entry = createBB(fooFunc,"entry");
    Builder.SetInsertPoint(entry);
}

int main(int argc, char *argv[])
{
	if(argc == 1)
    {
		fprintf(stderr, "Correct usage: bcc filename\n");
		exit(1);
	}
	if(argc > 2)
    {
		fprintf(stderr, "Passing more arguments than necessary.\n");
		fprintf(stderr, "Correct usage: bcc filename\n");
	}
	yyin = fopen(argv[1], "r");
    BasicBuildLLVM(TheModule);
	yyparse();
    checkGotoLabelDeclaration();
    cout << "\nParsing is Completed Successfully and the below is the output\n" << endl;
    if(parseErrorFlag==0)
    {
        Interpreter* interpreter = new Interpreter();
        CB->accept(interpreter);
        CB->codegen();
        Builder.CreateRet(Builder.getInt32(0));
        TheModule->dump();
    }
}
