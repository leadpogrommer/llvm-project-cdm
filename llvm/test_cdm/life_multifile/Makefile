CC=../../cmake-build-debug-clang/bin/clang
C_OPTS=-target cdm -S -O1

ASM=cocas
ASM_OPTS=-t cdm16


build/%.asm: %.c  $(CC)
	$(CC) $(C_OPTS) -o $@ $+

build/life.img: build/display.asm build/editor.asm build/life.asm build/main.asm start.asm
	$(ASM) $(ASM_OPTS) -o $@ $+

.SECONDARY: