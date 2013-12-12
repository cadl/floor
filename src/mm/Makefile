SUBDIR=mm/
OBJS=memory.o page.o

mm.o:$(OBJS)
	$(LD) -r $^ $(LDFLAGS) -o mm.o

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@

