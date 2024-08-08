CFLAGS=-I.. -Iunity -g
LDFLAGS=

.PHONY: all clean

all: test

clean:
	rm -rf *.o test_gfx_utils_measure_text

%.o: ../%.c
	$(CC) -c $(CFLAGS) -o $@ $<

unity/unity.o: unity/unity.c
	$(CC) -c $(CFLAGS) -o $@ $<

test_gfx_utils_measure_text.o: test_gfx_utils_measure_text.c
	$(CC) -c $(CFLAGS) -o $@ $<

test_gfx_utils_measure_text: test_gfx_utils_measure_text.o graphics_utils.o unity/unity.o
	$(CC) $(LDFLAGS) -o $@ $^

test: test_gfx_utils_measure_text
	./test_gfx_utils_measure_text
