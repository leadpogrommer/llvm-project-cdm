

typedef unsigned int uint;
typedef signed char schar;
typedef unsigned char uchar;


#define DS(type) \
volatile int s1_##type; \
volatile type s2_##type; \
volatile type s3_##type; \
volatile type s4_##type; \
__attribute__((noinline)) \
void do_shifts_##type(type a, type b){ \
  s1_##type = a << 3; \
  s2_##type = b << 15; \
  s3_##type = a >> 4; \
  s4_##type = b >> 14; \
}


DS(int)
DS(uint)
DS(schar)
DS(uchar)

int main(){
  do_shifts_int(-228, -1337);
  do_shifts_uint(228, 1337);
  do_shifts_schar(-57, -99);
  do_shifts_uchar(211, 122);
}
