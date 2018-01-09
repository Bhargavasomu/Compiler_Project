; ModuleID = 'B_Compiler_IR'

@data = global [100 x i32] zeroinitializer
@i = common global i32 0, align 4
@sum = common global i32 0, align 4
@x1 = common global i32 0, align 4
@x2 = common global i32 0, align 4
@a = common global i32 0, align 4
@micchkec = common global i32 0, align 4
@b = common global i32 0, align 4
@saisr = common global i32 0, align 4
@hello = common global i32 0, align 4
@this = common global i32 0, align 4
@j = common global i32 0, align 4
@somubha = common global i32 0, align 4
@0 = private unnamed_addr constant [3 x i8] c"%d\00"
@1 = private unnamed_addr constant [2 x i8] c"\0A\00"
@2 = private unnamed_addr constant [7 x i8] c"hello1\00"
@3 = private unnamed_addr constant [2 x i8] c"\0A\00"
@4 = private unnamed_addr constant [7 x i8] c"hello2\00"
@5 = private unnamed_addr constant [2 x i8] c"\0A\00"
@6 = private unnamed_addr constant [7 x i8] c"hello3\00"
@7 = private unnamed_addr constant [2 x i8] c"\0A\00"
@8 = private unnamed_addr constant [9 x i8] c"hello197\00"
@9 = private unnamed_addr constant [2 x i8] c"\0A\00"
@10 = private unnamed_addr constant [3 x i8] c"%d\00"
@11 = private unnamed_addr constant [2 x i8] c"\0A\00"
@12 = private unnamed_addr constant [12 x i8] c" hello129u4\00"
@13 = private unnamed_addr constant [2 x i8] c"\0A\00"
@14 = private unnamed_addr constant [3 x i8] c"%d\00"
@15 = private unnamed_addr constant [2 x i8] c"\0A\00"

define i32 @main() {
entry:
  store i32 1, i32* @a
  %0 = load i32, i32* @a
  %1 = call i32 @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i32 %0)
  %2 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @1, i32 0, i32 0))
  %3 = load i32, i32* @a
  %addtmp = add i32 %3, 1
  store i32 %addtmp, i32* @a
  br label %L1

L1:                                               ; preds = %GotoUnconditional, %entry
  %4 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @8, i32 0, i32 0))
  %5 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @9, i32 0, i32 0))
  %6 = load i32, i32* @a
  %multmp = mul i32 %6, 5
  store i32 %multmp, i32* @a
  %7 = load i32, i32* @a
  %8 = call i32 @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @10, i32 0, i32 0), i32 %7)
  %9 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @11, i32 0, i32 0))
  br label %L2

L2:                                               ; preds = %GotoUnconditional1, %L1
  store i32 109, i32* @a
  %10 = load i32, i32* @a
  %11 = call i32 @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @14, i32 0, i32 0), i32 %10)
  %12 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @15, i32 0, i32 0))
  ret i32 0

GotoUnconditional:                                ; No predecessors!
  %13 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @2, i32 0, i32 0))
  %14 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @3, i32 0, i32 0))
  %15 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @4, i32 0, i32 0))
  %16 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @5, i32 0, i32 0))
  %17 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @6, i32 0, i32 0))
  %18 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @7, i32 0, i32 0))
  br label %L1

GotoUnconditional1:                               ; No predecessors!
  %19 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @12, i32 0, i32 0))
  %20 = call i32 bitcast (i32 (i8*, i32)* @printf to i32 (i8*)*)(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @13, i32 0, i32 0))
  br label %L2
}

declare i32 @printf(i8*, i32)
