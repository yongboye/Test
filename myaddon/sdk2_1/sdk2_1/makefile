OBJS=sdk2_1.o 

#CC=clang++ 
#CC=g++ -std=c++11 -g
#CC=gcc
CC=arm-linux-gnueabihf-gcc
LIBFLAGS=-shared 
CFLAGS=-fPIC

#VPATH=/home/pi/socketlib
INCLUDE=-I"/usr/include"
LIBPATH=-L"/usr/lib/arm-linux-gnueabihf"
OUTPUT=libsdk2_1.so

${OUTPUT}: ${OBJS}
	#$(CC) $^ -mcmodel=medium -o $@  $(LIBFLAGS) $(CFLAGS) $(LIBPATH) -lboost_log -lboost_log_setup -lboost_thread -lboost_system -lboost_filesystem -lboost_regex -lboost_chrono -lboost_date_time -lboost_locale -lboost_program_options -lpthread
	$(CC) $^ -o $@  $(LIBFLAGS) $(CFLAGS) $(LIBPATH) -lboost_log -lboost_log_setup -lboost_thread -lboost_system -lboost_filesystem -lboost_chrono -lboost_date_time -lboost_locale -lboost_program_options -lpthread
	cp $@ /home/pi/libs
	cp $@ /home/pi/nodejs/golang/myaddon/sdk2_1/x64/Release
	sudo cp $@ /usr/lib

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $<

clean:
	@echo "clean project"
	rm *.o
	@echo "clean complete"

.PHONY:clean
