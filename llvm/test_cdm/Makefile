
CC=../cmake-build-debug-clang/bin/clang
C_OPTS=-target cdm -S -O1

ASM=cocas
ASM_OPTS=-t cdm16

EMU=/home/ilya/work/cdm_clang/cdm-devkit/logisim/logisim-cdm-emulator/standalone/build/libs/standalone-all.jar

out_asm/%.asm: src/%.c $(CC)
	$(CC) $(C_OPTS) -o $@ $+

out_bin/%.bin: out_asm/%.asm start.asm
	$(ASM) $(ASM_OPTS) -o $@ $+

run_%: out_bin/%.bin
	java -jar $(EMU) $+ emu_out/$@.json 1000000000

#clean:
#	rm -rv out_asm/*
#	rm -rv out_bin/*
#
#.PHONY: clean
#

.PHONY: run_%
.SECONDARY: