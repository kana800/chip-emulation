CC=gcc

SRCDIR:=src
BUILDDIR:=build

TESTDIR:=test

ASMINPUT:=$(wildcard $(TESTDIR)/test_asm_*.asm)
ASMOUT:=$(wildcard $(TESTDIR)/test_asm_*.out)

all:
	$(CC) -g $(SRCDIR)/disassembler.c -o $(BUILDDIR)/disassembler.out

clean:
	rm -r $(BUILDDIR)/*.out
