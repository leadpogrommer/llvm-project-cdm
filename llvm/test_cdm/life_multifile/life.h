#pragma once
#define N 32

volatile extern int BTN;
volatile extern char VRAM[N][N];

void life(char src[N][N], char dst[N][N]);
void edit(char s[N][N]);
void display(char data[N][N]);