declare i32 @power(i32, i32)
declare void @int_to_string(i32, i8*)
declare i32 @printf(i8*, ...)

define i32 @test() {
    %et1 = call i32 @power(i32 2, i32 108)
    ret i32 %et1
}

define void @printint(i32 %i) {
    %format_str = alloca [4 x i8]
    %1 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 0
    store i8 37, i8* %1
    %2 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 1
    store i8 100, i8* %2
    %3 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 2
    store i8 10, i8* %3
    %4 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 3
    store i8 0, i8* %4

    %5 = call i32 (i8*, ...) @printf(i8* %1, i32 %i)
    ret void
}

define i32 @main() {
    %1 = call i32 @test()

    %str = alloca [12 x i8]
    %str_ptr = getelementptr [12 x i8], [12 x i8]* %str, i32 0, i32 0

    call void (i32, i8*) @int_to_string(i32 %1, i8* %str_ptr)

    call i32 (i8*, ...) @printf(i8* %str_ptr)

    ret i32 %1
}