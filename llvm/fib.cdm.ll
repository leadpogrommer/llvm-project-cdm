; ModuleID = 'fib.c'
source_filename = "fib.c"
target datalayout = "e-S16-p:16:16-i8:8-i16:16-m:e-n16"
target triple = "cdm"

; Function Attrs: noinline nounwind optnone
define dso_local i16 @test() #0 {
entry:
  %a = alloca i16, align 2
  %b = alloca i16, align 2
  %c = alloca i16, align 2
  %d = alloca i16, align 2
  store i16 5, ptr %a, align 2
  store i16 2, ptr %b, align 2
  %0 = load i16, ptr %a, align 2
  %1 = load i16, ptr %b, align 2
  %add = add nsw i16 %0, %1
  store i16 %add, ptr %c, align 2
  %2 = load i16, ptr %b, align 2
  %add1 = add nsw i16 %2, 1
  store i16 %add1, ptr %d, align 2
  %3 = load i16, ptr %c, align 2
  %4 = load i16, ptr %d, align 2
  %add2 = add nsw i16 %3, %4
  ret i16 %add2
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 17.0.0 (git@github.com:leadpogrommer/llvm-project-cdm.git 1a11572868051e1fefde8fa1b510bc3f963bff4a)"}
