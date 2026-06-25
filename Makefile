CC = gcc
CFLAGS = -O2 -Wall -Wextra
COQC = coqc

TARGET = opencode.bin
OBJS = omi.o tetragrammatron.o metatron.o imo.o opencode.o
PROOF_DIR = proof
VFILE = $(PROOF_DIR)/phi_proof.v
VOFILE = $(PROOF_DIR)/phi_proof.vo

.PHONY: all coq clean smoke

all: $(TARGET) coq

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

omi.o: omi.c omi.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

tetragrammatron.o: tetragrammatron.c tetragrammatron.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

metatron.o: metatron.c metatron.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

imo.o: imo.c imo.h metatron.h tetragrammatron.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

opencode.o: opencode.c omi.h tetragrammatron.h metatron.h imo.h
	$(CC) $(CFLAGS) -c -o $@ $< -lm

coq: $(VOFILE)

$(VOFILE): $(VFILE)
	$(COQC) $<

smoke: $(TARGET)
	./$(TARGET) --eval '(cons 1 2)'
	./$(TARGET) --geom
	./$(TARGET) --boot
	./$(TARGET) --twin
	./$(TARGET) --check
	./$(TARGET) --render-frame > /tmp/frame.json
	./$(TARGET) --render-ppm > /tmp/frame.ppm

clean:
	rm -f $(TARGET) $(OBJS) $(PROOF_DIR)/*.vo $(PROOF_DIR)/*.glob $(PROOF_DIR)/*.aux
