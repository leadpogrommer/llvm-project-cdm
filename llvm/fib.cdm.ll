; ModuleID = 'fib.c'
source_filename = "fib.c"
target datalayout = "e-S16-p:16:16-i8:8-i16:16-m:e-n16"
target triple = "cdm"

; Function Attrs: noinline nounwind optnone
define dso_local i16 @test() #0 {
entry:
  %retval = alloca i16, align 2
  %_5 = alloca i16, align 2
  %_2 = alloca i16, align 2
  %_1 = alloca i16, align 2
  %a = alloca i16, align 2
  %b = alloca i16, align 2
  %c = alloca i16, align 2
  %d = alloca i16, align 2
  store volatile i16 5, ptr %_5, align 2
  store volatile i16 2, ptr %_2, align 2
  store volatile i16 1, ptr %_1, align 2
  %0 = load volatile i16, ptr %_5, align 2
  store i16 %0, ptr %a, align 2
  %1 = load volatile i16, ptr %_2, align 2
  store i16 %1, ptr %b, align 2
  %2 = load i16, ptr %a, align 2
  %3 = load i16, ptr %b, align 2
  %add = add nsw i16 %2, %3
  store i16 %add, ptr %c, align 2
  %4 = load i16, ptr %b, align 2
  %5 = load volatile i16, ptr %_1, align 2
  %add1 = add nsw i16 %4, %5
  store i16 %add1, ptr %d, align 2
  %6 = load i16, ptr %c, align 2
  %cmp = icmp slt i16 %6, 2
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i16 1337, ptr %retval, align 2
  br label %return

if.end:                                           ; preds = %entry
  %7 = load i16, ptr %c, align 2
  %8 = load i16, ptr %d, align 2
  %add2 = add nsw i16 %7, %8
  store i16 %add2, ptr %retval, align 2
  br label %return

return:                                           ; preds = %if.end, %if.then
  %9 = load i16, ptr %retval, align 2
  ret i16 %9
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 17.0.0 (https://github.com/llvm/llvm-project.git da165d53ae89607520d58dcd1da488899885ee6a)"}
