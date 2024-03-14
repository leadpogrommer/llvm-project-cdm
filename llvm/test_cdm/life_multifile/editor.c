#include "life.h"

__attribute__((noinline))
static int get_btn(){
  while (BTN){}
  int res = 0;
  while (!(res = BTN)){}
  return res;
}


void edit(char s[N][N]){
  int x = N/2;
  int y = N/2;
  int prevX = x;
  int prevY = y;
  VRAM[y][x] = 2 | s[y][x];
  while (1){
    int b = get_btn();
    switch (b) {
    case 1:
      x++;
      break;
    case 2:
      y++;
      break;
    case 4:
      x--;
      break;
    case 8:
      y--;
      break;
    case 16:
      s[y][x] = s[y][x] ^ 1;
      break;
    default:
      return;
    }
    VRAM[prevY][prevX] = s[prevY][prevX];
    prevY = y;
    prevX = x;
    VRAM[y][x] = 2 | s[y][x];
  }
}