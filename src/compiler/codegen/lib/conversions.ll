declare float @"int#to"(i32 %i)

; define double @"int#to"(i32 %i, i8* %str_ptr) {
;     %0 = call float @"int#to"(i32 %i)
;     %1 = sitofp i32 %0 to double
;     ret double %1
; }