declare void @print_int(i64)

define i64 @fun() {
    %1 = mul i64 5, 2
    %2 = add i64 %1, 1
    %3 = mul i64 10, 2
    %4 = sub i64 %2, %3
    ret i64 %3
}

define i64 @main() {
    %1 = call i64 @fun()
    call void @print_int(i64 %1)
    ret i64 %1
}