CC=gcc
CFLAGS=-I. -Wall -Werror -pedantic -Wstrict-prototypes -Wmissing-prototypes -std=c11 -pg
DEPS=mycp.h
OBJ=naive.o mmap.o sendfile.o mycp.o
TESTFILE_FROM=/tmp/testfile.bin
TESTFILE_TO=/tmp/testfile-copied.bin

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

mycp: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(TESTFILE_FROM):
	@echo "Creating file"
	@(strace -c dd if=/dev/urandom of=$(TESTFILE_FROM) bs=1024 count=3000 2>&1)|tail -n 1
	@sync

.PHONY: clean clean_test clean_buffers test

clean_test:
	rm -f $(TESTFILE_FROM) $(TESTFILE_TO)

clean: clean_test
	rm -f $(OBJ) mycp

test: mycp clean_test $(TESTFILE_FROM)
	sudo ./test.py
