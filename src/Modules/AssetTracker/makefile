CC 		  	= g++ -std=c++14
ROOT=$(shell git rev-parse --show-toplevel)
ARCHITECTURE = $(shell dpkg --print-architecture)

VENDOR_INC 	+= 	-I$(ROOT)/vendor/hash-library/
VENDOR_INC 	+= 	-I$(ROOT)/vendor/bitforgeutils/inc

VENDOR_DEPS += $(ROOT)/vendor/bitforgeutils/build/$(ARCHITECTURE)/dbg/*.o


SRCDIR	 	= src/
SOURCES		= $(shell find $(SRCDIR) -type f | grep cpp | cut -f 1 -d '.')
DIRSTRUCTURE = $(shell find $(INCDIR) -type d)
INCSTRUCTURE = $(patsubst %, -I%, $(DIRSTRUCTURE))


all: bfu hash
	$(MAKE) -C ./ final

final:
	$(CC) $(VENDOR_INC) $(INCSTRUCTURE) $(VENDOR_DEPS) main.cpp

bfu:
	$(MAKE) -C $(ROOT)/vendor bfu

hash:
	$(MAKE) -C $(ROOT)/vendor hash
