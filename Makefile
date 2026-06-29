CC = gcc
CFLAGS = -O2 -Wall -Wextra
COQC = coqc

TARGET = omicron.bin
CORE_DIR = core
OUTPUT_DIR = output
CORE_TARGET = $(CORE_DIR)/$(TARGET)
OBJS = $(CORE_DIR)/omi.o $(CORE_DIR)/tetragrammatron.o $(CORE_DIR)/metatron.o $(CORE_DIR)/imo.o $(CORE_DIR)/omicron.o
PROOF_DIR = proof
VFILE = $(PROOF_DIR)/phi_proof.v
VOFILE = $(PROOF_DIR)/phi_proof.vo

.PHONY: all coq clean smoke test-js test-c test-metatron-scribe test-omicron-boot portal-build portal-lint

all: $(CORE_TARGET) coq portal-build

$(CORE_TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(CORE_DIR)/omi.o: $(CORE_DIR)/omi.c $(CORE_DIR)/omi.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

$(CORE_DIR)/tetragrammatron.o: $(CORE_DIR)/tetragrammatron.c $(CORE_DIR)/tetragrammatron.h $(CORE_DIR)/omi.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

$(CORE_DIR)/metatron.o: $(CORE_DIR)/metatron.c $(CORE_DIR)/metatron.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

$(CORE_DIR)/imo.o: $(CORE_DIR)/imo.c $(CORE_DIR)/imo.h $(CORE_DIR)/metatron.h $(CORE_DIR)/tetragrammatron.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

$(CORE_DIR)/omicron.o: $(CORE_DIR)/omicron.c $(CORE_DIR)/omicron.h $(CORE_DIR)/omi.h $(CORE_DIR)/tetragrammatron.h $(CORE_DIR)/metatron.h $(CORE_DIR)/imo.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

coq: $(VOFILE)

$(VOFILE): $(VFILE)
	$(COQC) $<

smoke: $(CORE_TARGET)
	mkdir -p $(OUTPUT_DIR)
	./$(CORE_TARGET) --eval '(cons 1 2)'
	./$(CORE_TARGET) --geom
	./$(CORE_TARGET) --boot
	./$(CORE_TARGET) --twin
	./$(CORE_TARGET) --check
	./$(CORE_TARGET) --render-frame > $(OUTPUT_DIR)/frame.json
	./$(CORE_TARGET) --render-ppm > $(OUTPUT_DIR)/frame.ppm
	make test-c
	make test-js

test-c: scripts/test-c-scaffold
	./scripts/test-c-scaffold
	make test-metatron-scribe
	make test-omicron-boot

scripts/test-c-scaffold: scripts/test-c-scaffold.c $(CORE_DIR)/omi.c $(CORE_DIR)/omi.h
	$(CC) $(CFLAGS) -I$(CORE_DIR) -o $@ scripts/test-c-scaffold.c $(CORE_DIR)/omi.c -lm

test-metatron-scribe: scripts/test-metatron-scribe
	./scripts/test-metatron-scribe

scripts/test-metatron-scribe: scripts/test-metatron-scribe.c $(CORE_DIR)/metatron.c $(CORE_DIR)/metatron.h $(CORE_DIR)/tetragrammatron.c $(CORE_DIR)/tetragrammatron.h $(CORE_DIR)/omi.c $(CORE_DIR)/omi.h
	$(CC) $(CFLAGS) -I$(CORE_DIR) -o $@ scripts/test-metatron-scribe.c $(CORE_DIR)/metatron.c $(CORE_DIR)/tetragrammatron.c $(CORE_DIR)/omi.c -lm

test-omicron-boot: scripts/test-omicron-boot
	./scripts/test-omicron-boot

$(CORE_DIR)/omicron.test.o: $(CORE_DIR)/omicron.c $(CORE_DIR)/omicron.h $(CORE_DIR)/omi.h $(CORE_DIR)/tetragrammatron.h $(CORE_DIR)/metatron.h $(CORE_DIR)/imo.h
	$(CC) $(CFLAGS) -I$(CORE_DIR) -Dmain=omicron_legacy_main -c -o $@ $(CORE_DIR)/omicron.c -lm

scripts/test-omicron-boot: scripts/test-omicron-boot.c $(CORE_DIR)/omicron.test.o $(CORE_DIR)/metatron.c $(CORE_DIR)/metatron.h $(CORE_DIR)/tetragrammatron.c $(CORE_DIR)/tetragrammatron.h $(CORE_DIR)/omi.c $(CORE_DIR)/omi.h
	$(CC) $(CFLAGS) -I$(CORE_DIR) -o $@ scripts/test-omicron-boot.c $(CORE_DIR)/omicron.test.o $(CORE_DIR)/metatron.c $(CORE_DIR)/tetragrammatron.c $(CORE_DIR)/omi.c -lm

portal-build:
	cd portal && npm run build

portal-lint:
	cd portal && npm run lint

test-js: portal-build portal-lint
	cd portal && npm run test

clean:
	rm -f $(CORE_TARGET) $(OBJS) $(CORE_DIR)/omicron.test.o scripts/test-c-scaffold scripts/test-metatron-scribe scripts/test-omicron-boot $(PROOF_DIR)/*.vo $(PROOF_DIR)/*.glob $(PROOF_DIR)/*.aux
	rm -rf portal/dist
