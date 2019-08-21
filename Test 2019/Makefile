CC = gcc
CFLAGS = --std=c99 -lpthread -Wall -Werror -lrt -lm
.PHONY: defult all clean

default: q1 q2 q3 q4
all: default

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $, $(CFLAGS)

q1: q1.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

q2: q1.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

q3: q1.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

q4: q1.c $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	-rm -f *.o
	-rm -f q1 q2 q3 q4	
