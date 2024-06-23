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
	$(OBJCOPY) -I binary -B m68k -O elf32-m68k                               \
		--rename-section .data=.rodata                                       \
		--redefine-sym _binary_$(IMAGEBASE)_$(IMAGEEXT)_start=_image_start   \
		--redefine-sym _binary_$(IMAGEBASE)_$(IMAGEEXT)_end=_image_end       \
		--redefine-sym _binary_$(IMAGEBASE)_$(IMAGEEXT)_size=_image_size     \
		$< $@
