CFLAGS=$(shell pkg-config --cflags sdl2)
LDFLAGS=$(shell pkg-config --libs sdl2)

.PHONY: all clean

all: test

clean:
	rm -rf *.o test

test: main.o model.o view.o controller.o text.o sdl_backend.o
	$(CC) $(LDFLAGS) -o $@ $^
