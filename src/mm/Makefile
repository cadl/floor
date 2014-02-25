SUBDIR=mm/
OBJS=memory.o page.o buddy.o

mm.o:$(OBJS)
	$(LD) -r $^ $(LDFLAGS) -o mm.o

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@

