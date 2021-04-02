CC := $(CXX)
CXX := g++

WARNS		= -Wall -Werror -pedantic
CPPFLAGS 	= -I../../../inc -I../../inc -I../../../tests/inc
CFLAGS 		= $(WARNS) -g
CXXFLAGS 	= -std=c++03 $(WARNS)
CXXFLAGS += -g
LDLIBS   = -lpthread

#
HDRS = ../../inc/
SRC = ../../../src/
CDRSRC = ../../src/

OBJS	:= 	main.o $(CDRSRC)cdr_receiver.o $(SRC)noncopyable.o $(SRC)condvar.o $(CDRSRC)func_server.o \
			$(SRC)logger.o $(CDRSRC)protocol.o $(SRC)exceptions_calsses.o $(SRC)mutex.o $(SRC)thread_impl.o \
			$(SRC)thread.o $(SRC)functor.o $(SRC)semaphore.o $(SRC)sleep_micro_sec.o $(CDRSRC)cdr_struct.o \
			$(SRC)create_tcp_server.o $(SRC)tcp_server.o 
TARGET	:= main


all: $(TARGET)

$(TARGET): $(OBJS)

check: all
	./$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS) 

.PHONY: all check clean

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

helgrind: $(TARGET)
	valgrind --tool=helgrind --log-file=loghelgrind01.txt ./$(TARGET)

drd: $(TARGET)
	valgrind --tool=drd --log-file=logdrd01.txt ./$(TARGET)
