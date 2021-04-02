CC := gcc
#CXX := gcc

WARNS		= -ansi -Wall -Werror -pedantic -Wconversion
CPPFLAGS 	= -I./inc -I../inc
CFLAGS	= $(WARNS) -g
CXXFLAGS	= $(WARNS)
CXXFLAGS += -g
LDLIBS   = -lpthread

#TARGET=$(shell basename $(CURDIR))_test.out

SRC = src/
OBJS :=	main.o $(SRC)hashMap.o $(SRC)list.o $(SRC)list_functions.o $(SRC)listIt.o $(SRC)queue.o\
		$(SRC)vector.o $(SRC)protocol.o $(SRC)groupMng.o $(SRC)serverMng.o $(SRC)tcpServer.o\
		$(SRC)user.o $(SRC)usersMng.o

TARGET :=  main

all: $(TARGET)

$(TARGET): $(OBJS)

check: all
	./$(TARGET)

gdb: $(TARGET)
	gdb ./$(TARGET) -q

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f *.out *.o



