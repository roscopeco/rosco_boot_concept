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

IMAGEBASE?=splash
IMAGEEXT=pcx
IMAGESRC=$(IMAGEBASE).$(IMAGEEXT)
IMAGEBIN=$(IMAGEBASE).o

$(ELF): $(IMAGEBIN)

$(IMAGEBIN): $(IMAGESRC)
	$(LD) -T bin_obj.ld -r -o $@ -b binary $<

clean: clean-bins

clean-bins:
	rm -rf $(IMAGEBIN)

