OUT		  	= asapi
VERSION	 	= .1.0.0
ARCHITECTURE = $(shell dpkg --print-architecture)

CC 		  	= g++ -std=c++14 -I/usr/include/libdrm -I/usr/include/bitforge/utils

CPPFLAGS 	= -I/usr/include/freetype2 -I/usr/include/freetype2/freetype -DIMGUI_IMPL_OPENGL_ES2 
#-DDEBUG_MEMORY_ALLOC

INCDIR	 	= src/
OBJDIR	 	= obj/
VENDOR_EDITOR_ONLY_DIR  =  vendor/lib/editorOnly/*
VENDOR_SHARED_DIR  =  #vendor/lib/shared/*
VENDOR_DIR = $(VENDOR_SHARED_DIR)
INSTALLDIR	= /usr/lib/
HEADERDIR	= /usr/include/

SRCDIR	 	= src/
SOURCES		= $(shell find $(SRCDIR) -type f | grep cpp | cut -f 1 -d '.')
DIRSTRUCTURE = $(shell find $(INCDIR) -type d)
INCSTRUCTURE = $(patsubst %, -I%, $(DIRSTRUCTURE))


DEPGL 		= -lGL -lEGL -lGLESv2  -ldrm -lgbm -lX11 -lXext -lbitforgeutils -lpng
#DEPGL 		=  -lpng -lbrcmEGL -lbrcmGLESv2  -L/opt/vc/lib

OBJECTS 	= $(SOURCES:%.cpp=$(OBJDIR)%.o)
OBJECTS2 	= $(patsubst %, %.o, $(SOURCES))
OBJECTS 	= $(OBJECTS2:%.cpp=$(OBJDIR)%.o)

#INCLUDES += $(SOURCES:%.cpp=$(OBJDIR)%.hpp)

#HEADER_DEPS += 	-I./libs/libjpeg-turbo
HEADER_DEPS += 	-I./vendor/glm/glm/
#DEPS 		+= 	../02_Common/01_C-Logger
#DEPS 		+= 	../02_Common/03_UdpSocket
#DEPS 		+=	../02_Common/05_EventSystem

#STATIC_LINK	= ./libs/libjpeg-turbo/build/libturbojpeg.a

DEBUG_CC 	+= -g -DLOG_LEVEL=DebugLevel::ALL ${PROFILER_FLAG}
RELEASE_CC	+= -O3 -DLOG_LEVEL=DebugLevel::INFO -DNOTRACE ${PROFILER_FLAG}

COLOR=\033[0;32m
NC=\033[0m # No Color

#################################################################################

all:
	./prebuild.sh
	make lib
	make debug-editor
	make release-editor
	make debug-player
	make release-player

print:
	$(OBJECTS)

debug-player: DEBUG_CC 	+= -DIS_PLAYER
debug-player: BUILDPATH 	= build/dbg/player/
debug-player: debug

release-player: RELEASE_CC 	+= -DIS_PLAYER
release-player: BUILDPATH 	= build/rel/player/
release-player: release


debug-editor: DEBUG_CC 		+=  -DIS_EDITOR
debug-editor: BUILDPATH 	 =  build/dbg/editor/
debug-editor: HEADER_DEPS 	+= 	-I./vendor/imgui/
debug-editor: HEADER_DEPS 	+= 	-I./vendor/ImGuiFileDialog/ImGuiFileDialog/
debug-editor: VENDOR_DIR 	+=  $(VENDOR_EDITOR_ONLY_DIR)
debug-editor: debug

release-editor: RELEASE_CC 	+=  -DIS_EDITOR
release-editor: BUILDPATH 	 =  build/rel/editor/
release-editor: HEADER_DEPS += 	-I./vendor/imgui/
release-editor: HEADER_DEPS += 	-I./vendor/ImGuiFileDialog/ImGuiFileDialog/
release-editor: VENDOR_DIR 	+=  $(VENDOR_EDITOR_ONLY_DIR)
release-editor: release

ifeq ($(ARCHITECTURE),armhf)
	@echo -----Build for target
debug: DEBUG_CC 	+= -DIS_TARGET
endif
debug: CC 			+= $(DEBUG_CC)
debug: OBJDIR 		= $(BUILDPATH)obj/
debug: OBJECTS 		= $(SOURCES:%.cpp=$(OBJDIR)%.o)
debug: STATIC_LINK	= $(BUILDPATH)lib/*.a
debug:  $(OUT) 
#	ar rcs build/dbg/lib/lib$(OUT).a build/dbg/obj/*.o

ifeq ($(ARCHITECTURE),armhf)
	@echo -----Build for target
release: RELEASE_CC 	+= -DIS_TARGET
endif
release: CC 			+= $(RELEASE_CC)
release: OBJDIR 		= $(BUILDPATH)obj/
release: OBJECTS 		= $(SOURCES:%.cpp=$(OBJDIR)%.o)
release: STATIC_LINK	= $(BUILDPATH)lib/*.a
release: $(OUT) 
#	ar rcs build/rel/lib/lib$(OUT).a build/rel/obj/*.o


$(OUT): $(SOURCES)
	$(CC) -shared -o $(BUILDPATH)$@.so $(CPPFLAGS) $(INCSTRUCTURE) $(HEADER_DEPS)  $(OBJDIR)* $(VENDOR_DIR) $(DEPGL) 
	@echo 
	$(CC) -o $(BUILDPATH)$@ $(CPPFLAGS) $(INCSTRUCTURE) $(HEADER_DEPS)  $(OBJDIR)* $(VENDOR_DIR) main.cpp  $(DEPGL) 
	@echo 


$(SOURCES): $(INCDIR)$(@.hpp) $(SRCDIR)$@ 
	@echo "${COLOR}$(OBJDIR)$(notdir $@).o${NC}:"
	$(CC) -c $(CPPFLAGS) $(INCSTRUCTURE) $(HEADER_DEPS) $(@).cpp -o $(OBJDIR)$(notdir $@).o -fPIC
	@echo 
	
test:
	@$(MAKE) ${OUT} -B
	@$(MAKE) r

clean: 
	rm -fr build/*
	./prebuild.sh

remove:
	rm -rf $(HEADERDIR)$(OUT)  
	rm ${INSTALLDIR}lib${OUT}.so*


install:
	cp build/rel/${OUT}.so ${INSTALLDIR}lib${OUT}.so$(VERSION)
	ln -sf ${INSTALLDIR}lib${OUT}.so$(VERSION) ${INSTALLDIR}lib${OUT}.so
	cp -r inc $(HEADERDIR)$(OUT) 

lib:
	$(MAKE) -C vendor lib