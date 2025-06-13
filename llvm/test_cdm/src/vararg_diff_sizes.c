#include <stdarg.h>

long long sum(int Count, ...) {
  long long Result = 0;

  va_list Args;
  va_start(Args, Count);

  Result += va_arg(Args, long long);
  Result += va_arg(Args, long);

  va_end(Args);
  return Result;
}

int main(void) { return sum(2, 3ll, 9l); }
