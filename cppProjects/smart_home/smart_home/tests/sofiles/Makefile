CXX := g++
CC := $(CXX)

WARNS		= -Wall -Werror -pedantic -fPIC	
CPPFLAGS 	= -I../../inc -I../../../inc
CFLAGS 		= $(WARNS) -g
CXXFLAGS 	= -std=c++03 $(WARNS)
CXXFLAGS += -g
LDLIBS   = -lpthread -ldl

HDRS = ../../inc/
SRC = ../../../src/
SRCSMART = ../../src/

OBJS	:=  TemperatureDemoAgent.o

TARGET=TemperatureDemo.so

all: $(TARGET)
$(TARGET) : $(OBJS)

$(TARGET):
	$(CC) -shared -o $(TARGET) $(OBJS)
	
clean:
	$(RM) $(TARGET) $(OBJS) *.gch
