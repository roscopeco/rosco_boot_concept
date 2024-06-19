CFLAGS=$(shell pkg-config --cflags sdl2) -I..
LDFLAGS=$(shell pkg-config --libs sdl2)

.PHONY: all clean

all: test

clean:
	rm -rf *.o test

%.o: ../%.c
	$(CC) -c $(CFLAGS) -o $@ $<

test: main.o model.o view.o controller.o text.o sdl_backend.o
	$(CC) $(LDFLAGS) -o $@ $^
