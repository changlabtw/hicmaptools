.KEEP_STAT:

all: genBiMap formatmap hicmaptools

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS = -g -DDEBUG
else
    CFLAGS = -DNDEBUG
endif

Compiler	= g++
FLAGS		= -Wall $(CFLAGS) -O2
LIB			=

OBJECTgenBiMap	= genBiMap.o binmap.o

OBJECTformatmap	= formatmap.o binmap.o index.o

OBJECThicmaptools	= hicmaptools.o binmap.o index.o query.o interval.o bat.o region.o couple.o random.o

genBiMap:	$(OBJECTgenBiMap)
			$(Compiler) $(FLAGS) $(OBJECTgenBiMap) -o genBiMap $(LIB)
			mv -f genBiMap ../bin/.

formatmap:	$(OBJECTformatmap)
			$(Compiler) $(FLAGS) $(OBJECTformatmap) -o formatmap $(LIB)
			mv -f formatmap ../bin/.

hicmaptools:	$(OBJECThicmaptools)
			$(Compiler) $(FLAGS) $(OBJECThicmaptools) -o hicmaptools $(LIB)
			mv -f hicmaptools ../bin/.

%.o:		%.cpp
			$(Compiler) $(FLAGS) -c $<

clean:
			rm -f *.o

install:
			mv -f ../genBiMap ~/bin/.
			mv -f ../hicmaptools ~/bin/.
