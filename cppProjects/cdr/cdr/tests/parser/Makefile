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

OBJS	:= main.o $(SRC)noncopyable.o $(SRC)logger.o $(CDRSRC)create_parser.o $(CDRSRC)nokia_parser.o $(SRC)exceptions_calsses.o $(SRC)mutex.o
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
