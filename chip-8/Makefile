CC=gcc

BUILDDIR:=build
SRCDIR:=src
TESTDIR:=test

emu: $(SRCDIR)/emu.c
	$(CC) $^ -g -lncurses -o $(BUILDDIR)/emu

gh: $(TESTDIR)/generate_sample_hex.c
	$(CC) $^ -o $(BUILDDIR)/gh
