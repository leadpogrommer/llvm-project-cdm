#!/bin/zsh

set -e

cd "$0:P:h"
cmake-build-debug-clang/bin/clang -target $1 -S  -emit-llvm fib.c -o "fib.$1.ll" -mllvm -view-isel-dags -mllvm -view-sched-dags