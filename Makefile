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

ifeq ($(ENABLE_ANIM),true)
EXTRA_CFLAGS+=-DENABLE_ANIM
export ENABLE_ANIM=true
else
export ENABLE_ANIM=false
endif

ifeq ($(TEST_ANIM),true)
EXTRA_CFLAGS+=-DSHOW_TEST_ANIM
export TEST_ANIM=true
else
export TEST_ANIM=false
endif

ifeq ($(ENABLE_TIMER),true)
EXTRA_CFLAGS+=-DENABLE_TIMER
export ENABLE_TIMER=true
else
export ENABLE_TIMER=false
endif

ifeq ($(MODEL_DEBUG),true)
EXTRA_CFLAGS+=-DMODEL_DEBUG
export MODEL_DEBUG=true
else
export MODEL_DEBUG=false
endif

ifeq ($(BLIT_DEBUG),true)
EXTRA_CFLAGS+=-DBLIT_DEBUG
export BLIT_DEBUG=true
else
export BLIT_DEBUG=false
endif

ifeq ($(TEST_PATTERN),true)
EXTRA_CFLAGS+=-DTEST_PATTERN_DEBUG
export TEST_PATTERN=true
else
export TEST_PATTERN=false
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

