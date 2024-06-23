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
endif
export ENABLE_ANIM

ifeq ($(TEST_ANIM),true)
EXTRA_CFLAGS+=-DSHOW_TEST_ANIM
endif
export TEST_ANIM

ifeq ($(ENABLE_TIMER),true)
EXTRA_CFLAGS+=-DENABLE_TIMER
endif
export ENABLE_TIMER

ifeq ($(MODEL_DEBUG),true)
EXTRA_CFLAGS+=-DMODEL_DEBUG
endif
export MODEL_DEBUG

ifeq ($(BLIT_DEBUG),true)
EXTRA_CFLAGS+=-DBLIT_DEBUG
endif
export BLIT_DEBUG

ifeq ($(TEST_PATTERN),true)
EXTRA_CFLAGS+=-DTEST_PATTERN_DEBUG
endif
export TEST_PATTERN

ifeq ($(BIZCAT),true)
EXTRA_CFLAGS+=-DALWAYS_BIZCAT
endif
export BIZCAT

ifeq ($(TOPAZ),true)
EXTRA_CFLAGS+=-DALWAYS_TOPAZ
endif
export TOPAZ

ifneq ($(HRES),)
EXTRA_CFLAGS+=-DVIEW_HRES=$(HRES)
endif
export HRES

ifneq ($(VRES),)
EXTRA_CFLAGS+=-DVIEW_VRES=$(VRES)
endif
export VRES

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

