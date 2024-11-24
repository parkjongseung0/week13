CC = gcc
CFLAGS = -Wall -g -pg
OBJ = open.o close.o put.o get.o do_snapshot.o do_recovery.o
LIB = libkvs.a
TARGET = kvs_static

all: $(LIB) $(TARGET)

$(LIB): $(OBJ)
	ar rcs $(LIB) $(OBJ)

$(TARGET): main.o $(LIB)
	$(CC) $(CFLAGS) -o $(TARGET) main.o -L. -lkvs

main.o: main.c kvs.h
	$(CC) $(CFLAGS) -c main.c -o main.o

open.o: open.c kvs.h
	$(CC) $(CFLAGS) -c open.c -o open.o

close.o: close.c kvs.h
	$(CC) $(CFLAGS) -c close.c -o close.o

put.o: put.c kvs.h
	$(CC) $(CFLAGS) -c put.c -o put.o

get.o: get.c kvs.h
	$(CC) $(CFLAGS) -c get.c -o get.o

do_snapshot.o: do_snapshot.c kvs.h
	$(CC) $(CFLAGS) -c do_snapshot.c -o do_snapshot.o

do_recovery.o: do_recovery.c kvs.h
	$(CC) $(CFLAGS) -c do_recovery.c -o do_recovery.o

clean:
	rm -f *.o $(LIB) $(TARGET)
	rm -f kvs.img
	rm -f gp_kvs_static.gprof
