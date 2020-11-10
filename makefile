OUT		  	= ogle
VERSION	 	= .1.0.0

CC 		  	= g++ -std=c++11 -fpermissive -I/usr/include/libdrm -I/opt/vc/include 

CPPFLAGS 	= -I/usr/include/freetype2 -I/usr/include/freetype2/freetype

INCDIR	 	= inc/
OBJDIR	 	= obj/
INSTALLDIR	= /usr/lib/
HEADERDIR	= /usr/include/

SRCDIR	 	= src/
SOURCES		= $(shell find $(SRCDIR) -type f | grep cpp | cut -f 1 -d '.')
DIRSTRUCTURE = $(shell find $(INCDIR) -type d)
INCSTRUCTURE = $(patsubst %, -I%, $(DIRSTRUCTURE))

DEPGL 		=  -lpng -lGL -lEGL -lGLESv2  -ldrm -lgbm -lBFutils  -lX11 -lXext
#DEPGL 		=  -lpng -lbrcmEGL -lbrcmGLESv2  -L/opt/vc/lib

OBJECTS 	= $(SOURCES:%.cpp=$(OBJDIR)%.o)
OBJECTS2 	= $(patsubst %, %.o, $(SOURCES))
OBJECTS 	= $(OBJECTS2:%.cpp=$(OBJDIR)%.o)

#INCLUDES += $(SOURCES:%.cpp=$(OBJDIR)%.hpp)

#HEADER_DEPS += 	-I./libs/libjpeg-turbo
HEADER_DEPS += 	-I./libs/glm/
#DEPS 		+= 	../02_Common/01_C-Logger
#DEPS 		+= 	../02_Common/03_UdpSocket
#DEPS 		+=	../02_Common/05_EventSystem

#STATIC_LINK	= ./libs/libjpeg-turbo/build/libturbojpeg.a

DEBUG_CC 	+= -g -DLOG_LEVEL=DebugLevel::ALL
RELEASE_CC	+= -O3 -DLOG_LEVEL=DebugLevel::INFO -DNOTRACE

#################################################################################


all:
	./prebuild.sh
	make debug
	make release



debug: CC 			+= $(DEBUG_CC)
#debug: CC 			+= -DPROFILER_ACTIVE
debug: BUILDPATH 	= build/dbg/
debug: OBJDIR 		= build/dbg/obj/
debug: OBJECTS 		= $(SOURCES:%.cpp=$(OBJDIR)%.o)
debug: STATIC_LINK	= $(BUILDPATH)lib/*.a
debug:  $(OUT) 
#	ar rcs build/dbg/lib/lib$(OUT).a build/dbg/obj/*.o

release: CC 			+= $(RELEASE_CC)
release: BUILDPATH 		= build/rel/
release: OBJDIR 		= build/rel/obj/
release: OBJECTS 		= $(SOURCES:%.cpp=$(OBJDIR)%.o)
release: STATIC_LINK	= $(BUILDPATH)lib/*.a
release: $(OUT) 
#	ar rcs build/rel/lib/lib$(OUT).a build/rel/obj/*.o


$(OUT): pch $(SOURCES)
	$(CC) -shared -o $(BUILDPATH)$@.so $(CPPFLAGS) $(INCSTRUCTURE) $(HEADER_DEPS)  $(OBJDIR)* $(DEPGL) -lturbojpeg -lpthread -lfreetype -Iglm 
	$(CC) -o $(BUILDPATH)$@ $(CPPFLAGS) $(INCSTRUCTURE) $(HEADER_DEPS)  $(OBJDIR)* main.cpp $(DEPGL) -lturbojpeg -lpthread -lfreetype -Iglm 


$(SOURCES): pch $(INCDIR)$(@.hpp) $(SRCDIR)$@
	$(CC) -c $(CPPFLAGS) $(INCSTRUCTURE) $(HEADER_DEPS)  $(DEPGL) $(@).cpp -o $(OBJDIR)$(notdir $@).o $(DEPGL) -lpthread -lfreetype -Iglm -llogger -ludpsocket -lnetworkedEvents -fpic
	

pch: 
	$(CC) inc/PrecompiledHeaders.hpp

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
