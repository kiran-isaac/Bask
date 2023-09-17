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