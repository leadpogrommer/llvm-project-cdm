

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

//int a = 228;
//int b;
//
//
//void add(int  c){
//   b = a + c;
//}
//

int a = 5;
int b;



__attribute__((noinline))
void add(int c){
  b = a + c;
}

int main(){
  add(1337);
  return b;
}


//void add(int a){
//  volatile int cntr = a;
//}

//int fib(int n){
//  if(n <= 1) return n;
//  return fib(n - 1) + fib(n - 2);
//}

//int use_local_var(int a){
//  volatile int b = a + 1;
//  return b;
//}


//int s(int a, int b, int c, int d){
//  return a+b+c+d;
//}
//
//int fib2(int n){
//  int prev =
//}