CC=gcc

SRCDIR:=src
BUILDDIR:=build

main: $(SRCDIR)/main.c $(SRCDIR)/memory.h
	$(CC) $^ -o $(BUILDDIR)/$@.out

assembler: $(SRCDIR)/assembler.c
	$(CC) $^ -o $(BUILDDIR)/$@.out
