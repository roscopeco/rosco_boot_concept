# Make rosco_m68k boostrap intro

# location of xosera_m68k_api
XOSERA_M68K_API?=../xosera_m68k_api

# use generic common make rules for Xosera + rosco_m68k build
include $(XOSERA_M68K_API)/common_xosera_m68k.mk

EXTRA_CFLAGS+=-std=c2x -Wno-format -Iinclude
EXTRA_LIBS+=

IMAGEBASE?=rosco_m68k_logo_simple_pixel
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
