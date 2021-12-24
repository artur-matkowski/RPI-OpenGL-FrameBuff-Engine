CC 		  		= g++ -std=c++14 -g
ROOT_PROJECT	= $(shell git rev-parse --show-toplevel)
ROOT_MODULE		= $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
ARCHITECTURE 	= $(shell dpkg --print-architecture)

VENDOR_INC 	+= 	-I$(ROOT_PROJECT)/vendor/hash-library/
VENDOR_INC 	+= 	-I$(ROOT_PROJECT)/vendor/bitforgeutils/inc
VENDOR_INC  += 	-I$(ROOT_PROJECT)/vendor/glm/glm/

VENDOR_DEPS += $(ROOT_PROJECT)/vendor/bitforgeutils/build/$(ARCHITECTURE)/dbg/*.o
VENDOR_DEPS += $(ROOT_PROJECT)/vendor/hash-library/sha256.cpp


SOURCES		= $(shell find src/ -type f | grep cpp)
INCDIR		= $(ROOT_PROJECT)/src
DIRSTRUCTURE = $(shell find $(INCDIR) -type d)
INCSTRUCTURE = $(patsubst %, -I%, $(DIRSTRUCTURE))


SOURCES		+= $(shell find $(ROOT_MODULE)/../ResourceTracker/src/ -type f | grep cpp)
#SOURCES		+= $(shell find $(ROOT_MODULE)/../ResourceTracker/tests/ -type f | grep cxx)



final:
	$(CC) -DIS_EDITOR -Itests $(VENDOR_INC) $(INCSTRUCTURE) $(VENDOR_DEPS) $(SOURCES) tests/*.cxx main.cxx

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