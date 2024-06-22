CFLAGS=-I.. -Iunity -g
LDFLAGS=

.PHONY: all clean

all: test

clean:
	rm -rf *.o test_blit_rects

%.o: ../%.c
	$(CC) -c $(CFLAGS) -o $@ $<

unity/unity.o: unity/unity.c
	$(CC) -c $(CFLAGS) -o $@ $<

test_blit_rects.o: test_blit_rects.c
	$(CC) -c $(CFLAGS) -o $@ $<

test_blit_rects: test_blit_rects.o blit_rects.o unity/unity.o
	$(CC) $(LDFLAGS) -o $@ $^

test: test_blit_rects
	./test_blit_rects