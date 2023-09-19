; ModuleID = 'llvm-link'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.list = type { i64**, i32 }

@hello_world = unnamed_addr constant [13 x i8] c"hello world\0A\00", align 1

define void @"int#to"(i32 %i, i8* %str_ptr) {
  %format_str = alloca [4 x i8]
  %f1 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 0
  store i8 37, i8* %f1
  %f2 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 1
  store i8 100, i8* %f2
  %f3 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 2
  store i8 10, i8* %f3
  %f4 = getelementptr [4 x i8], [4 x i8]* %format_str, i32 0, i32 3
  store i8 0, i8* %f4
  %1 = call i32 (i8*, ...) @sprintf(i8* %str_ptr, i8* %f1, i32 %i)
  ret void
}

declare i32 @sprintf(i8*, ...)

define i32 @power(i32 %base, i32 %exponent) {
  %result = alloca i32
  store i32 1, i32* %result
  %i = alloca i32
  store i32 0, i32* %i
  br label %loop

loop:                                             ; preds = %body, %0
  %i2 = load i32, i32* %i
  %cmp = icmp slt i32 %i2, %exponent
  br i1 %cmp, label %body, label %exit

body:                                             ; preds = %loop
  %tmp = load i32, i32* %result
  %mul = mul i32 %tmp, %base
  store i32 %mul, i32* %result
  %tmp2 = load i32, i32* %i
  %add = add i32 %tmp2, 1
  store i32 %add, i32* %i
  br label %loop

exit:                                             ; preds = %loop
  %final_result = load i32, i32* %result
  ret i32 %final_result
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local %struct.list* @list_new() #0 {
  %1 = alloca %struct.list*, align 8
  %2 = call noalias i8* @malloc(i64 16) #2
  %3 = bitcast i8* %2 to %struct.list*
  store %struct.list* %3, %struct.list** %1, align 8
  %4 = call noalias i8* @malloc(i64 8) #2
  %5 = bitcast i8* %4 to i64**
  %6 = load %struct.list*, %struct.list** %1, align 8
  %7 = getelementptr inbounds %struct.list, %struct.list* %6, i32 0, i32 0
  store i64** %5, i64*** %7, align 8
  %8 = load %struct.list*, %struct.list** %1, align 8
  %9 = getelementptr inbounds %struct.list, %struct.list* %8, i32 0, i32 0
  %10 = load i64**, i64*** %9, align 8
  %11 = getelementptr inbounds i64*, i64** %10, i64 0
  store i64* null, i64** %11, align 8
  %12 = load %struct.list*, %struct.list** %1, align 8
  %13 = getelementptr inbounds %struct.list, %struct.list* %12, i32 0, i32 1
  store i32 0, i32* %13, align 8
  %14 = load %struct.list*, %struct.list** %1, align 8
  ret %struct.list* %14
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @list_push(%struct.list* %0, i8* %1) #0 {
  %3 = alloca %struct.list*, align 8
  %4 = alloca i8*, align 8
  store %struct.list* %0, %struct.list** %3, align 8
  store i8* %1, i8** %4, align 8
  %5 = load %struct.list*, %struct.list** %3, align 8
  %6 = getelementptr inbounds %struct.list, %struct.list* %5, i32 0, i32 1
  %7 = load i32, i32* %6, align 8
  %8 = add nsw i32 %7, 1
  store i32 %8, i32* %6, align 8
  %9 = load %struct.list*, %struct.list** %3, align 8
  %10 = getelementptr inbounds %struct.list, %struct.list* %9, i32 0, i32 1
  %11 = load i32, i32* %10, align 8
  %12 = srem i32 %11, 2
  %13 = icmp ne i32 %12, 0
  br i1 %13, label %14, label %28

14:                                               ; preds = %2
  %15 = load %struct.list*, %struct.list** %3, align 8
  %16 = getelementptr inbounds %struct.list, %struct.list* %15, i32 0, i32 0
  %17 = load i64**, i64*** %16, align 8
  %18 = bitcast i64** %17 to i8*
  %19 = load %struct.list*, %struct.list** %3, align 8
  %20 = getelementptr inbounds %struct.list, %struct.list* %19, i32 0, i32 1
  %21 = load i32, i32* %20, align 8
  %22 = sext i32 %21 to i64
  %23 = mul i64 8, %22
  %24 = call i8* @realloc(i8* %18, i64 %23) #2
  %25 = bitcast i8* %24 to i64**
  %26 = load %struct.list*, %struct.list** %3, align 8
  %27 = getelementptr inbounds %struct.list, %struct.list* %26, i32 0, i32 0
  store i64** %25, i64*** %27, align 8
  br label %28

28:                                               ; preds = %14, %2
  %29 = load i8*, i8** %4, align 8
  %30 = bitcast i8* %29 to i64*
  %31 = load %struct.list*, %struct.list** %3, align 8
  %32 = getelementptr inbounds %struct.list, %struct.list* %31, i32 0, i32 0
  %33 = load i64**, i64*** %32, align 8
  %34 = load %struct.list*, %struct.list** %3, align 8
  %35 = getelementptr inbounds %struct.list, %struct.list* %34, i32 0, i32 1
  %36 = load i32, i32* %35, align 8
  %37 = sub nsw i32 %36, 1
  %38 = sext i32 %37 to i64
  %39 = getelementptr inbounds i64*, i64** %33, i64 %38
  store i64* %30, i64** %39, align 8
  ret void
}

; Function Attrs: nounwind
declare dso_local i8* @realloc(i8*, i64) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @list_get(%struct.list* %0, i32 %1) #0 {
  %3 = alloca %struct.list*, align 8
  %4 = alloca i32, align 4
  store %struct.list* %0, %struct.list** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load %struct.list*, %struct.list** %3, align 8
  %6 = getelementptr inbounds %struct.list, %struct.list* %5, i32 0, i32 0
  %7 = load i64**, i64*** %6, align 8
  %8 = load i32, i32* %4, align 4
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds i64*, i64** %7, i64 %9
  %11 = load i64*, i64** %10, align 8
  %12 = bitcast i64* %11 to i8*
  ret i8* %12
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @list_size(%struct.list* %0) #0 {
  %2 = alloca %struct.list*, align 8
  store %struct.list* %0, %struct.list** %2, align 8
  %3 = load %struct.list*, %struct.list** %2, align 8
  %4 = getelementptr inbounds %struct.list, %struct.list* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  ret i32 %5
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @list_empty(%struct.list* %0) #0 {
  %2 = alloca %struct.list*, align 8
  store %struct.list* %0, %struct.list** %2, align 8
  %3 = load %struct.list*, %struct.list** %2, align 8
  %4 = getelementptr inbounds %struct.list, %struct.list* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 8
  %6 = icmp eq i32 %5, 0
  %7 = zext i1 %6 to i32
  ret i32 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @list_pop(%struct.list* %0) #0 {
  %2 = alloca %struct.list*, align 8
  %3 = alloca i8*, align 8
  store %struct.list* %0, %struct.list** %2, align 8
  %4 = load %struct.list*, %struct.list** %2, align 8
  %5 = getelementptr inbounds %struct.list, %struct.list* %4, i32 0, i32 0
  %6 = load i64**, i64*** %5, align 8
  %7 = load %struct.list*, %struct.list** %2, align 8
  %8 = getelementptr inbounds %struct.list, %struct.list* %7, i32 0, i32 1
  %9 = load i32, i32* %8, align 8
  %10 = sub nsw i32 %9, 1
  %11 = sext i32 %10 to i64
  %12 = getelementptr inbounds i64*, i64** %6, i64 %11
  %13 = load i64*, i64** %12, align 8
  %14 = bitcast i64* %13 to i8*
  store i8* %14, i8** %3, align 8
  %15 = load %struct.list*, %struct.list** %2, align 8
  %16 = getelementptr inbounds %struct.list, %struct.list* %15, i32 0, i32 1
  %17 = load i32, i32* %16, align 8
  %18 = add nsw i32 %17, -1
  store i32 %18, i32* %16, align 8
  %19 = load i8*, i8** %3, align 8
  ret i8* %19
}

define void @print_int(i64 %i) {
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
  %1 = call i32 (i8*, ...) @printf(i8* %f2, i64 %i)
  ret void
}

declare i32 @printf(i8*, ...)

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}
!llvm.module.flags = !{!1}

!0 = !{!"clang version 10.0.0-4ubuntu1 "}
!1 = !{i32 1, !"wchar_size", i32 4}
