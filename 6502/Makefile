CC=gcc

SRCDIR:=src
BUILDDIR:=build

TESTDIR:=test

ASMINPUT:=$(wildcard $(TESTDIR)/test_*.hex)
ASMOUT:=$(ASMINPUT:.hex=.asm)

all:
	$(CC) -g $(SRCDIR)/disassembler.c -o $(BUILDDIR)/disassembler.out

emu:
	$(CC) -g $(SRCDIR)/emulator.c -o $(BUILDDIR)/emulator.out

test_chip:
	$(CC) -g $(TESTDIR)/test_chip.c -o $(BUILDDIR)/chip_test.out

d_test:
	for fname in $(ASMINPUT); do \
		outname=$${fname%.hex}.asm; \
		echo "checking $$fname and $$outname..."; \
		$(BUILDDIR)/disassembler.out $$fname > $(BUILDDIR)/temp_output; \
		diff -u $$outname $(BUILDDIR)/temp_output; \
	done

clean:
	rm -r $(BUILDDIR)/*.out
