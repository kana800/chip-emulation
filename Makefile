CC=gcc

SRCDIR:=src
BUILDDIR:=build

main: $(SRCDIR)/main.c
	$(CC) $^ -o $(BUILDDIR)/$@.out
