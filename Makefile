CC=gcc

SRCDIR:=src
BUILDDIR:=build
TESTDIR:=test

main: $(SRCDIR)/main.c $(SRCDIR)/memory.h
	$(CC) $^ -o $(BUILDDIR)/$@.out

assembler: $(SRCDIR)/assembler.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out

run_test_1: assembler 
	$(BUILDDIR)/$^.out $(TESTDIR)/test_1.asm
