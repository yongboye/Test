OBJS=gLog.o

#CC=clang++ 
#CC=g++ -std=c++11 -g
CC=gcc
LIBFLAGS=-shared 
CFLAGS=-fPIC

#VPATH=base
#INCLUDE=-I"${HOME}/Downloads/boost_1_64_0"
#LIBPATH=-L"${HOME}/Downloads/boost_1_64_0/stage/lib"
INCLUDE=-I"/usr/local/include"
LIBPATH=-L"/usr/local/lib"
OUTPUT=libmygLog.so

${OUTPUT}: ${OBJS} ${BASEOBJS}
	$(CC) -DBOOST_LOG_DYN_LINK $^ -o $@ $(CFLAGS) $(LIBFLAGS) $(LIBPATH) -lglog -lpthread -lstdc++
	cp $@ ${HOME}/libs
	sudo cp $@ /usr/lib

.cpp.o:
	$(CC) -DBOOST_LOG_DYN_LINK $(CFLAGS) $(INCLUDE) -c $<

clean:
	@echo "clean project"
	rm *.o
	@echo "clean complete"

.PHONY:clean
