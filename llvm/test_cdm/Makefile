
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

BUILD_DIR=build
RESOURCES_DIR=$(BUILD_DIR)/resources
MODIFIED_RESOURCES=$(addprefix 	\
	$(RESOURCES_DIR)/, 						\
		config.properties 					\
		test_circuit.circ 					\
		test_circuit_emu.circ				\
)
UNCHANGED_RESOURCES=$(addprefix \
	$(RESOURCES_DIR)/, 						\
		cdm16.circ                  \
		logisim-banked-memory.jar   \
		logisim-cdm-emulator.jar    \
		logisim-runner-all.jar      \
)
RESOURCES=$(UNCHANGED_RESOURCES) $(MODIFIED_RESOURCES)

LIBS=$(addprefix $(BUILD_DIR)/libs/,	\
	ctype.asm														\
	string.asm													\
)

test: $(RESOURCES) $(LIBS)
	mkdir -p $(BUILD_DIR)/gen
	pytest -n auto

$(BUILD_DIR)/cdm-devkit:
	cd $(BUILD_DIR) && git clone git@github.com:cdm-processors/cdm-devkit.git
	cd $(BUILD_DIR)/cdm-devkit && git checkout 4fa59ba # To ensure in stable work

$(BUILD_DIR)/docker-dummy: config/cdm-devkit.Dockerfile
	docker build -t cdm-devkit --file $< .
	@mkdir -p $(@D)
	touch $@

build-docker: $(BUILD_DIR)/docker-dummy

$(BUILD_DIR)/cdm-devkit-tests-dummy: $(BUILD_DIR)/docker-dummy $(BUILD_DIR)/cdm-devkit
	docker run --rm -v ${PWD}/$(BUILD_DIR):/root cdm-devkit make prepare_tests
	@mkdir -p $(@D)
	touch $@

$(UNCHANGED_RESOURCES): $(BUILD_DIR)/cdm-devkit-tests-dummy
	@mkdir -p $(BUILD_DIR)/resources
	cp $(BUILD_DIR)/cdm-devkit/tests/jar/logisim-runner-all.jar $(RESOURCES_DIR)
	cp $(BUILD_DIR)/cdm-devkit/tests/resources/cdm16/circuits/* $(RESOURCES_DIR)

$(MODIFIED_RESOURCES): $(RESOURCES_DIR)/%: config/%
	mkdir -p $(@D)
	cp $< $@

prepare_tests: $(RESOURCES)

$(LIBS): $(BUILD_DIR)/libs/%.asm: libs/src/%.c
	@mkdir -p $(@D)
	# FIXME: strstr does not work with higher optimization flags
	../build/bin/clang -target cdm -S -O1 -I libs/inc $< -o $@

std-libs: $(BUILD_DIR)/libs/string.asm

clean:
	rm -rf build/*

.PHONY: run_%
.SECONDARY: