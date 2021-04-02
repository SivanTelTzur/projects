CC := $(CXX)
CXX := g++

WARNS		= -Wall -Werror -pedantic
CPPFLAGS 	= -I../inc -I../../inc -I../../../inc
CFLAGS 		= $(WARNS) -g
CXXFLAGS 	= -std=c++03 $(WARNS)
CXXFLAGS += -g
LDLIBS   = -lpthread

#
HDRS = ../../inc/
SRC = ../../../src/
SRCSMART = ../../src/

OBJS	:= main.o $(SRC)exceptions_calsses.o $(SRC)thread_impl.o $(SRC)mutex.o \
			$(SRC)noncopyable.o $(SRC)thread.o $(SRC)waitableQueueException.o\
			$(SRC)functor.o $(SRC)logger.o $(SRC)semaphore.o $(SRC)mutex_guard.o $(SRC)thread_group.o\
			$(SRC)conditionalVariable.o $(SRC)conditionalVariableException.o $(SRC)threadpool.o \
			$(SRC)threadpool_exceptions.o $(SRC)sleep_micro_sec.o $(SRCSMART)sample_sensor.o \
			$(SRC)create_tcp_client.o $(SRC)tcp_client.o 

TARGET	:= main

all: $(TARGET)

$(TARGET): $(OBJS)

check: all
	./$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS) *.gch

.PHONY: all check clean

recheck: clean all
	./$(TARGET) -v

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

helgrind: $(TARGET)
	valgrind --tool=helgrind --log-file=loghelgrind01.txt ./$(TARGET)

drd: $(TARGET)
	valgrind --tool=drd --log-file=logdrd01.txt ./$(TARGET)
