; ModuleID = 'fib.c'
source_filename = "fib.c"
target datalayout = "e-S16-p:16:16-i8:8-i16:16-m:e-n16"
target triple = "cdm"

@a = dso_local global i16 5, align 2
@ptr = dso_local global ptr null, align 1
@b = dso_local global i16 0, align 2

; Function Attrs: noinline nounwind optnone
define dso_local i16 @main() #0 {
entry:
  %retval = alloca i16, align 2
  store i16 0, ptr %retval, align 2
  store ptr getelementptr inbounds (i16, ptr @a, i16 1337), ptr @ptr, align 1
  ret i16 0
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 17.0.0 (https://github.com/llvm/llvm-project.git 59b60618ef7ef93d98042e550209c7eebd41d6dc)"}
