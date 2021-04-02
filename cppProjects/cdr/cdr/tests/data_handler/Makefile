CC := $(CXX)
CXX := g++

WARNS		= -Wall -Werror -pedantic
CPPFLAGS 	= -I../../../inc -I../../inc -I../../../tests/inc
CFLAGS 		= $(WARNS) -g `mysql_config --cxxflags`
CXXFLAGS 	= -std=c++03 $(WARNS)
CXXFLAGS += -g
LDLIBS   = -L/usr/lib/x86_64-linux-gnu  -lmysqlcppconn -lpthread -lz -lm -lrt -latomic -ldl 

#
HDRS = ../../inc/
SRC = ../../../src/
CDRSRC = ../../src/

OBJS	:= 	main.o $(CDRSRC)cdr_receiver.o $(SRC)noncopyable.o $(SRC)condvar.o $(CDRSRC)func_server.o \
			$(SRC)logger.o $(CDRSRC)protocol.o $(SRC)exceptions_calsses.o $(SRC)mutex.o $(SRC)thread_impl.o \
			$(SRC)thread.o $(SRC)functor.o $(SRC)semaphore.o $(SRC)sleep_micro_sec.o \
			$(SRC)create_tcp_server.o $(SRC)tcp_server.o $(CDRSRC)subscribers_mng.o $(CDRSRC)subscriber_data.o \
			$(CDRSRC)func_data.o $(CDRSRC)cdr_subscriber.o $(CDRSRC)cdr_data_mng.o $(CDRSRC)cdr_converter.o \
			$(CDRSRC)aggregator.o $(SRC)create_tcp_server.o $(CDRSRC)queries_tcp_server.o $(CDRSRC)func_query.o \
			$(CDRSRC)create_query_server.o $(CDRSRC)query_gateway.o $(SRC)cpp_sql.o $(CDRSRC)cdr_struct.o
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
