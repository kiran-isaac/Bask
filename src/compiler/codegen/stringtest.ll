%string = type { i8*, i32 }

declare %string @string_new(i32)

define i32 @main() {
    %1 = call %string @string_new(i32 10)
    
    ret i32 0
}