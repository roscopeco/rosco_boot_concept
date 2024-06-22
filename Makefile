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

ifeq ($(MODEL_DEBUG),true)
EXTRA_CFLAGS+=-DMODEL_DEBUG
endif

ifeq ($(BLIT_DEBUG),true)
EXTRA_CFLAGS+=-DBLIT_DEBUG
endif

.PHONY: all clean tests

all: tests $(BINARY) $(DISASM) sdl2/test

clean: cleansdl cleantests

tests: tests/Makefile	
	make -C tests test

sdl2/test: tests sdl2/Makefile $(OBJECTS)
	make -C sdl2 test

cleantests:
	make -C tests clean

cleansdl:
	make -C sdl2 clean

