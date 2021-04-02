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

OBJS	:= 	main.o $(CDRSRC)files_notifier.o $(SRC)noncopyable.o $(SRC)condvar.o $(CDRSRC)reader.o \
			$(SRC)logger.o $(CDRSRC)protocol.o $(CDRSRC)create_parser.o $(CDRSRC)nokia_parser.o \
			$(SRC)exceptions_calsses.o $(SRC)mutex.o $(CDRSRC)cdr_handler.o $(CDRSRC)cdr_functors.o \
			$(SRC)thread_impl.o $(SRC)thread.o $(SRC)functor.o $(SRC)semaphore.o $(CDRSRC)cdr_sender.o\
			$(SRC)create_tcp_client.o $(SRC)tcp_client.o $(SRC)sleep_micro_sec.o $(CDRSRC)cdr_struct.o\
			$(SRC)sleep_micro_sec.o
TARGET	:= main


all: $(TARGET)

$(TARGET): $(OBJS)

check: all
	mkdir -p files
	mkdir -p files/done
	./$(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS) -r files/done

.PHONY: all check clean

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

helgrind: $(TARGET)
	valgrind --tool=helgrind --log-file=loghelgrind01.txt ./$(TARGET)

drd: $(TARGET)
	valgrind --tool=drd --log-file=logdrd01.txt ./$(TARGET)
