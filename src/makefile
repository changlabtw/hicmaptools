.KEEP_STAT:

all: 3CONET-build hicmaptools

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS = -g -DDEBUG
else
    CFLAGS = -DNDEBUG
endif

Compiler	= g++
FLAGS		= -Wall $(CFLAGS) -O2
LIB		=

OBJECT3CONETbuild	= 3CONET-build.o binmap.o

OBJECThicmaptools	= hicmaptools.o binmap.o index.o query.o interval.o bat.o region.o couple.o

3CONET-build:	$(OBJECT3CONETbuild)
			$(Compiler) $(FLAGS) $(OBJECT3CONETbuild) -o 3CONET-build $(LIB)
			mv -f 3CONET-build ../bin/.

hicmaptools:	$(OBJECThicmaptools)
			$(Compiler) $(FLAGS) $(OBJECThicmaptools) -o hicmaptools $(LIB)
			mv -f hicmaptools ../bin/.

%.o:		%.cpp
			$(Compiler) $(FLAGS) -c $<

clean:
			rm -f *.o

install:
			mv -f ../3CONET-build ~/bin/.
			mv -f ../hicmaptools ~/bin/.
