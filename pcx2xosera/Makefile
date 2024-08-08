.PHONY: all clean

all: pcx2xosera

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

pcx2xosera: main.o
	$(CC) -o $@ $^
	
clean: 
	$(RM) *.o pcx2xosera
