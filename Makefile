# Make Xosera test program for rosco_m68k
#
# vim: set noet ts=8 sw=8
# Copyright (c) 2021 Xark
# MIT LICENSE

# location of xosera_m68k_api
XOSERA_M68K_API?=../xosera_m68k_api

# use generic common make rules for Xosera + rosco_m68k build
include $(XOSERA_M68K_API)/common_xosera_m68k.mk

EXTRA_CFLAGS+=-std=c2x -Wno-format
EXTRA_LIBS+=

all: $(BINARY) $(DISASM) sdl2/test

clean: cleansdl

sdl2/test: sdl2/Makefile $(OBJECTS)
	make -C sdl2 test

cleansdl:
	make -C sdl2 clean

