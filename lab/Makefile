OBJS = main.o parsing.o isbuiltin.o not_builtin.o child_handler.o

CFLAGS = -c

INC = -I .

santashell: $(OBJS)
	$(CC) -o $@ $(OBJS)
.c.o:
	$(CC) $(INC) $(CFLAGS) $<

clean:
	rm *.o santashell

dep:
	gccmakedep $(SRCS)