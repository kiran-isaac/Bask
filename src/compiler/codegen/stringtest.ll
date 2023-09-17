%string = type %struct.list

; declare %string* @list_new()
declare void @list_push(%string*, i8*)
declare i8* @list_pop(%string*)
declare i8* @list_get(%string*, i32)
declare i32 @list_size(%string*)

declare i32 @printf(i8*, ...)

define i32 @main() {
    ; %1 = call %string* @list_new()
    ; call void @list_push(%string* %1, i8* null)
    ; call void @list_push(%string* %1, i8* null)

    ; %1 = call i32 @list_size(%string* %1)


    ret i32 0
}