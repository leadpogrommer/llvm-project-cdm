

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


// GLOBALS & OFFSETS
//int a = 5;
//int b;
//int* ptr;
//char *str = "fuck";

//__attribute__((noinline))
//void add(int c){
//  b = a + c;
//}

//int main(int i){
//  //  add(1337);
//  //  volatile int letter_f = str[0];
//  ptr = &a + i;
//  return str[1];
//}


// ARRS

//unsigned char field[32][32];
//
//int main(){
//  for(int i = 0; i < 32; i++){
//    for(int j = 0; j < 32; j++){
//      field[i][j] = i+j;
//    }
//  }
//}

//#define N 32
//
//void *memcpy(char *dst, char *src, int sz){
//  void *ret = 0;
//  while (sz--) *(dst++) = *(src++);
//}
//
//void life(char src[N][N], char dst[N][N]){
//  for(int y = 0; y < N; y++){
//    for(int x = 0; x < N; x++){
////      int ns = 0;
////      if(y < (N-1)) ns += src[y+1][x];
////      if(x < (N-1)) ns += src[y][x+1];
////      if(y > 0) ns += src[y-1][x];
////      if(x > 0) ns += src[y][x-1];
////      if(y < (N-1) && x < (N-1)) ns += src[y+1][x+1];
////      if(y < (N-1) && x > 0) ns += src[y+1][x-1];
////      if(y > 0 && x > 0) ns += src[y-1][x-1];
////      if(y > 0 && x < (N-1)) ns += src[y-1][+1];
////
////      if(!src[y][x] && ns == 3)dst[y][x] = 1;
////      else if ((ns == 2 || ns == 3) && src[y][x]) dst[y][x] = 1;
////      else dst[y][x] = 0;
//      dst[y][x] = src[y][x];
//    }
//  }
//}
//
//
//
//char s[N][N] = {
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
//};
//
//char d[N][N];
//
//int main(){
//  life(s, d);
//
//  return 0;
//}

//
//char d[N][N];
//
//int main(){
//  life(s, d);
//  return (int)&d;
//}

//int aaa(int i){
//  return i & 2;
//}

//int f(char src[N][N], char dst[N][N], int i, int j){
//  dst[i][j] = src[i][j];
//}

//int f(char src[N][N], char dst[N][N], int i, int j){
//  return src[i][j];
//}


// case
//
//void foo(){
//  *(int*)0xFF00 = 1337;
//}
//
//int lut(int i){
//
//  switch (i) {
//    case 1:
//      return i+0xf100;
//      break;
//    case 2:
//      return i+0xf200;
//    case 3:
//      *(int*)0xFF00 = 1337;
//      break;
//    case 4:
//      return i+0xf400;;
//      break;
//    case 5:
//      return i+0xf500;
//    default:
//      return -1;
//    }
//    return 0xBABE;
//}

// addi/subi
//
//int add1(int i){
//  return i + 13;
//}
//
//int add2(int i){
//  return i + 1337;
//}
//
//int add3(int i){
//  return i - 1337;
//}
//
//int add4(int i){
//  return  i - 17;
//}
//
//int arr[10];
//
//int get_i(int i){
//  return arr[i] + arr[i+1] + arr[3];
//}
//
//void set_i(int i){
//  arr[i] = 1337;
//}
//
//char carr[10];
//char get_ci(int i){
//  return carr[i] + carr[i+1] + carr[3];
//}
//
//void set_ci(int i){
//  carr[i] = 228;
//}

// TODO: does not compile
//int shift8(int i){
//  return i << 9;
//}


//void store_byte_on_stack(char i, int j, char k, int l){
//  volatile char a;
////  volatile int b;
////  volatile char c;
////  volatile int d;
//  a = i;
////  b = j;
////  c = k;
////  d = l;
//  // TODO: It leaves gaps
//
//}
//

//
//static char s[32][32];
//
//static int static_int = 228;
//extern int extern_int;
//int normal_int = 1337;
//
//
//__attribute__((noinline))
//static int static_fun(){
//  return s[4][4];
//}
//
//
//int external_fun();
//
//__attribute__((noinline))
//int normal_fun(){
//  return 1337;
//}
//
//
//
//int main(){
//  static_fun();
//  external_fun();
//  normal_fun();
//
//  return static_int + extern_int + normal_int;
//}
//



//extern volatile int UART0_DATA;
//
//__attribute__((noinline))
//void write_char_ptr(char* c) {
//  UART0_DATA = *c;
//}
//
//void __bootloader_panic_c() {
//  char c = 'f';
//
//  write_char_ptr(&c);
//}
//
//
//


//__attribute__((noinline))
//int f1(int a){
//  return a > 15 ? 1337 : 228;
//}
//
//int f2(int a, int b){
//  return a < b;
//}
//
//int f3(_Bool c){
//  return c ? 1447 : 228;
//}
//
////int foo(int a, int b, _Bool c) {
//////  return c ? 1337 : 228; // select
////  return a > 15 ? 1337 : 228; // select_cc
//////  return a < b; // setcc
////}
//
//int main(){
//  volatile int a = f1(16);
//  return f1(3);
//}
//
//
//


// ISR CC

volatile int tick;

// correct
__attribute__((noinline))
void some_fun(){
  tick++;
}

// almost correct
__attribute__((noinline))
void some_fun_calling_fun(){
  some_fun();
}

// correct
__attribute__((noinline))
__attribute__((CDM_ISR))
void some_isr(){
  tick++;
}


__attribute__((noinline))
__attribute__((CDM_ISR))
void some_isr_calling_fun(){
  some_fun();
}
