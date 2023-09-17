declare i32 @sprintf(i8*, ...)

; define string @"int#to"(i32 %i, i8* %str_ptr) {
;     %format_str = alloca [4 x i8]
;     %f1 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 0
;     store i8 37, i8* %f1
;     %f2 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 1
;     store i8 100, i8* %f2
;     %f3 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 2
;     store i8 10, i8* %f3
;     %f4 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 3
;     store i8 0, i8* %f4

;     call i32 (i8*, ...) @sprintf(i8* %str_ptr, i8* %f1, i32 %i)
;     ret void
; }

; define float @"int#to"(i32 %i) {
;     %f1 = sitofp i32 %i to float
;     ret float %f1
; }

