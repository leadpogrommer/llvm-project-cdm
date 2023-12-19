

//int test()
//{
//
//  volatile int _5 = 5;
//  volatile int _2 = 2;
//  volatile int _1 = 1;
//
//  int a = _5;
//  int b = _2;
//
//  int c = a + b;      // c = 7
//  int d = b + _1;      // d = 3
//
//  if(c < 55) return 1337;
//
////  goto st;
//
//  return (c+d);
//}

int fib(int n){
  if(n <= 1) return n;
  return fib(n - 1) + fib(n - 2);
}