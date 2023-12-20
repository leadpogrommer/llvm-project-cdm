; ModuleID = 'fib.c'
source_filename = "fib.c"
target datalayout = "e-S16-p:16:16-i8:8-i16:16-m:e-n16"
target triple = "cdm"

; Function Attrs: noinline nounwind optnone
define dso_local i16 @fib(i16 noundef %n) #0 {
entry:
  %retval = alloca i16, align 2
  %n.addr = alloca i16, align 2
  store i16 %n, ptr %n.addr, align 2
  %0 = load i16, ptr %n.addr, align 2
  %cmp = icmp sle i16 %0, 1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load i16, ptr %n.addr, align 2
  store i16 %1, ptr %retval, align 2
  br label %return

if.end:                                           ; preds = %entry
  %2 = load i16, ptr %n.addr, align 2
  %sub = sub nsw i16 %2, 1
  %call = call i16 @fib(i16 noundef %sub)
  %3 = load i16, ptr %n.addr, align 2
  %sub1 = sub nsw i16 %3, 2
  %call2 = call i16 @fib(i16 noundef %sub1)
  %add = add nsw i16 %call, %call2
  store i16 %add, ptr %retval, align 2
  br label %return

return:                                           ; preds = %if.end, %if.then
  %4 = load i16, ptr %retval, align 2
  ret i16 %4
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 17.0.0 (https://github.com/llvm/llvm-project.git d1d5041d4e5a46b2a7cf92a0dfeaae57582f72d8)"}
