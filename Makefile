CC=gcc

SRCDIR:=src
BUILDDIR:=build

TESTDIR:=test

ASMINPUT:=$(wildcard $(TESTDIR)/test_asm_*.asm)
ASMOUT:=$(wildcard $(TESTDIR)/test_asm_*.out)

main: $(SRCDIR)/main.c $(SRCDIR)/chip.h
	$(CC) $^ -o $(BUILDDIR)/$@.out

assembler: $(SRCDIR)/assembler.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out

disasm: $(SRCDIR)/disassembler.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out

test_assembler: assembler
#	$(foreach var,$(ASMINPUT),)
