declare i32 @printf(i8*, ...)
declare void @"int#to"(i64, i8*)

define void @print_int(i64 %i) {
    ; format string for printing a long int with a newline
    %f1 = alloca [6 x i8]
    %f2 = getelementptr [6 x i8], [6 x i8]* %f1, i32 0, i32 0
    store i8 37, i8* %f2
    %f3 = getelementptr [6 x i8], [6 x i8]* %f1, i32 0, i32 1
    store i8 108, i8* %f3
    %f4 = getelementptr [6 x i8], [6 x i8]* %f1, i32 0, i32 2
    store i8 100, i8* %f4
    %f5 = getelementptr [6 x i8], [6 x i8]* %f1, i32 0, i32 3
    store i8 10, i8* %f5
    %f6 = getelementptr [6 x i8], [6 x i8]* %f1, i32 0, i32 4
    store i8 0, i8* %f6



    call i32 (i8*, ...) @printf(i8* %f2, i64 %i)
    ret void
}