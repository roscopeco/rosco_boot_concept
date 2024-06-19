ROSCO_M68K_DEFAULT_DIR=../../..

ifndef ROSCO_M68K_DIR
$(info NOTE: ROSCO_M68K_DIR not set, using libs: ../../../code/software/libs)
ROSCO_M68K_DIR=$(ROSCO_M68K_DEFAULT_DIR)
else
$(info NOTE: Using ROSCO_M68K_DIR libs in: $(ROSCO_M68K_DIR))
endif

-include $(ROSCO_M68K_DIR)/code/software/software.mk

EXTRA_CFLAGS=-std=c2x -Wno-format
EXTRA_LIBS=

all: rosco_boot_concept.bin sdl2/test
clean: cleansdl

sdl2/test: sdl2/Makefile $(OBJECTS)
	make -C sdl2 test

cleansdl:
	make -C sdl2 clean
