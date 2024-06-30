# Make Xosera test program for rosco_m68k
#
# vim: set noet ts=8 sw=8
# Copyright (c) 2021 Xark
# MIT LICENSE

# location of xosera_m68k_api
XOSERA_M68K_API?=../xosera_m68k_api

# use generic common make rules for Xosera + rosco_m68k build
include $(XOSERA_M68K_API)/common_xosera_m68k.mk

EXTRA_CFLAGS+=-std=c2x -Wno-format -Iinclude
EXTRA_LIBS+=

IMAGEBASE?=welcome
IMAGEEXT=pcx
IMAGESRC=$(IMAGEBASE).$(IMAGEEXT)

kmain.o: splash.h

splash.h: $(IMAGESRC)
	echo "#ifndef _SPLASH_DATA_H" > $@
	echo "#define _SPLASH_DATA_H" >> $@
	xxd -i $(IMAGESRC) | sed -E "s/$(IMAGEBASE)_$(IMAGEEXT)/splash_data/g" >> $@
	echo "#endif" >> $@

clean: clean-data

clean-data:
	rm -f splash.h
