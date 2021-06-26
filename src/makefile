.KEEP_STAT:

all: genBinMap formatmap hicmaptools

DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CFLAGS = -g -DDEBUG
else
    CFLAGS = -DNDEBUG
endif

Compiler	= g++
FLAGS		= -Wall $(CFLAGS) -O2 --std=c++0x
LIB			= -lcurl -lz

OBJECTgenBinMap	= genBinMap.o binmap.o

OBJECTformatmap	= formatmap.o binmap.o index.o

OBJECThicmaptools	= hicmaptools.o binmap.o index.o query.o interval.o bat.o region.o couple.o straw.o

genBinMap:	$(OBJECTgenBinMap)
			$(Compiler) $(FLAGS) $(OBJECTgenBinMap) -o genBinMap $(LIB)
			mv -f genBinMap ../bin/.

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
			mv -f ../bin/genBiMap /bin/.
			mv -f ../bin/hicmaptools /bin/.
