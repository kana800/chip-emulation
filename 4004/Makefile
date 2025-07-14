CC=gcc

SRCDIR:=src
BUILDDIR:=build

TESTDIR:=test

ASMINPUT:=$(wildcard $(TESTDIR)/test_asm_*.asm)
ASMOUT:=$(wildcard $(TESTDIR)/test_asm_*.out)

emu: $(SRCDIR)/emulator.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out

assembler: $(SRCDIR)/assembler.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out

disasm: $(SRCDIR)/disassembler.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out

test_assembler: assembler
#	$(foreach var,$(ASMINPUT),)

test_accu: $(TESTDIR)/test_emu_accumulator.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out
	echo "running accumulator testing..."
	./$(BUILDDIR)/$@.out
	echo "test completed"

test_reg: $(TESTDIR)/test_reg_pair.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out
	echo "running register testing..."
	./$(BUILDDIR)/$@.out
	echo "test completed"

test_ram: $(TESTDIR)/test_emu_ram.c
	$(CC) $^ -g -o $(BUILDDIR)/$@.out
	echo "running DATA RAM testing..."
	./$(BUILDDIR)/$@.out
	echo "test completed"

clean:
	rm -r out
