CC=gcc

SRCDIR:=src
BUILDDIR:=build
TESTDIR:=test

main: $(SRCDIR)/main.c $(SRCDIR)/memory.h
	$(CC) $^ -o $(BUILDDIR)/$@.out

assembler: $(SRCDIR)/assembler.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out

test_assembler: assembler
	rm -r $(BUILDDIR)/test_arth.obj
	$(BUILDDIR)/$^.out $(TESTDIR)/test_arth.asm >> $(BUILDDIR)/test_arth.obj
	diff -y $(TESTDIR)/test_arth.obj $(BUILDDIR)/test_arth.obj 
