CC 		  		= g++ -std=c++14 -g
ROOT_PROJECT	= $(shell git rev-parse --show-toplevel)
ROOT_MODULE		= $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ARCHITECTURE 	= $(shell dpkg --print-architecture)

VENDOR_INC 	+= 	-I$(ROOT_PROJECT)/vendor/hash-library/
VENDOR_INC 	+= 	-I$(ROOT_PROJECT)/vendor/bitforgeutils/inc
VENDOR_INC  += 	-I$(ROOT_PROJECT)/vendor/glm/glm/

VENDOR_DEPS += $(ROOT_PROJECT)/vendor/bitforgeutils/build/$(ARCHITECTURE)/dbg/*.o
VENDOR_DEPS += $(ROOT_PROJECT)/vendor/hash-library/sha256.cpp


SRCDIR	 	= src/
SOURCES		= $(shell find $(SRCDIR) -type f | grep cpp)
INCDIR		= $(ROOT_PROJECT)/src
DIRSTRUCTURE = $(shell find $(INCDIR) -type d)
INCSTRUCTURE = $(patsubst %, -I%, $(DIRSTRUCTURE))



final:
	$(CC) -DIS_EDITOR $(VENDOR_INC) $(INCSTRUCTURE) $(VENDOR_DEPS) $(SOURCES) main.c

all: bfu hash
	$(MAKE) -C ./ final

bfu:
	$(MAKE) -C $(ROOT_PROJECT)/vendor bfu

hash:
	$(MAKE) -C $(ROOT_PROJECT)/vendor hash

test:
	./a.out $(ROOT_MODULE)

clean-test:
	rm -f assets/Resource_Trackers/*.json
	rm -f assets/*.txt