CFLAGS=-I/opt/homebrew/include
LDFLAGS=-L/opt/homebrew/lib -lSDL2

.PHONY: all clean

all: test

clean:
	rm -rf *.o test

test: main.o model.o view.o controller.o sdl_backend.o
	$(CC) $(LDFLAGS) -o $@ $^
