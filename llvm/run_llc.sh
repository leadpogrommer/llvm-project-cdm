#!/bin/zsh

set -e

cd "$0:P:h"
cmake-build-debug-clang/bin/llc -march=$1 -filetype=asm \
  -view-isel-dags \
  "fib.$1.ll"