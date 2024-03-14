#include "life.h"

void display(char data[N][N]){
  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      VRAM[i][j] = data[i][j];
    }
  }
}