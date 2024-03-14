#include "life.h"

void life(char src[N][N], char dst[N][N]){
  for(int y = 1; y < (N-1); y++){
    VRAM[y][31] = 2;
    for(int x = 1; x < (N-1); x++){
      int ns = 0;
      ns += src[y+1][x];
      ns += src[y][x+1];
      ns += src[y-1][x];
      ns += src[y][x-1];
      ns += src[y+1][x+1];
      ns += src[y+1][x-1];
      ns += src[y-1][x-1];
      ns += src[y-1][x+1];

      if(!src[y][x] && ns == 3)dst[y][x] = 1;
      else if ((ns == 2 || ns == 3) && src[y][x]) dst[y][x] = 1;
      else dst[y][x] = 0;
      //      dst[y][x]=src[y][x];
    }
  }
}