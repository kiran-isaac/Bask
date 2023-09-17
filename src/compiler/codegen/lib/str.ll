declare i32 @printf(i8*, ...)
declare i32 @sprintf(i8*, ...)

define i32 @power(i32 %base, i32 %exponent) {
    %result = alloca i32
    store i32 1, i32* %result
    %i = alloca i32
    store i32 0, i32* %i
    br label %loop

loop:
    %i2 = load i32, i32* %i
    %cmp = icmp slt i32 %i2, %exponent
    br i1 %cmp, label %body, label %exit

body:
    %tmp = load i32, i32* %result
    %mul = mul i32 %tmp, %base
    store i32 %mul, i32* %result
    %tmp2 = load i32, i32* %i
    %add = add i32 %tmp2, 1
    store i32 %add, i32* %i
    br label %loop

exit:
    %final_result = load i32, i32* %result
    ret i32 %final_result
}

define void @int_to_string(i32 %i, i8* %str_ptr) {
    %format_str = alloca [4 x i8]
    %f1 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 0
    store i8 37, i8* %f1
    %f2 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 1
    store i8 100, i8* %f2
    %f3 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 2
    store i8 10, i8* %f3
    %f4 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 3
    store i8 0, i8* %f4

    call i32 (i8*, ...) @sprintf(i8* %str_ptr, i8* %f1, i32 %i)
    ret void
}