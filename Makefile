INC=/usr/include/postgresql 
LIB=/usr/lib
CFLAGS=-I$(INC)
LDLIBS=-L$(LIB) -lpq
CC=gcc

ALL=connect create create2 select print cursor async1 async2
# ponizej przyklady regul i zaleznosci kompilacji wybranych plikow
connect: connect.c
	$(CC) $(CFLAGS) -o connect connect.c $(LDLIBS)
create: create.c
	$(CC) $(CFLAGS) -o create create.c $(LDLIBS)
create2: create2.c
	$(CC) $(CFLAGS) -o create2 create2.c $(LDLIBS)
select: select.c
	$(CC) $(CFLAGS) -o select select.c $(LDLIBS)
print: print.c
	$(CC) $(CFLAGS) -o print print.c $(LDLIBS)
cursor: cursor.c
	$(CC) $(CFLAGS) -o cursor cursor.c $(LDLIBS)
async1: async1.c
	$(CC) $(CFLAGS) -o async1 async1.c $(LDLIBS)
async2: async2.c
	$(CC) $(CFLAGS) -o async2 async2.c $(LDLIBS)
clean:
	@rm -f *.o *~ $(ALL)
all: clean connect create create2 select print cursor async1 async2
