CC=gcc
CFLAGS=-Wall

objects = test1 test2 test3 test4 test5
all: $(objects)

$(objects): %: %.c
	$(CC) $(CFLAGS) -o $@ $<


run1:
	./test1

run2:
	./test2

run3:
	./test3

run4:
	./test4

run5:
	./test5
