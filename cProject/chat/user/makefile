CC=gcc 
TARGET=$(shell basename $(CURDIR))_test.out
CFLAGS=-ansi -pedantic -Wall -g3  -Werror -Wconversion -c
OBJS=clientMng.o vector.o ui.o client_net.o protocol.o main_client.o sender.out listener.out

$(TARGET): $(OBJS)
	gcc -o $(TARGET) $(OBJS) 

	
run: $(TARGET)
	./$(TARGET)
	

gdb: $(TARGET)
	gdb ./$(TARGET) -q
	
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

clientMng.o: clientMng.c clientMng.h client_net.c client_net.h ui.c ui.h protocol.c protocol.h vector.c vector.h
main_client.o: main_client.c clientMng.c clientMng.h
vector.o: vector.c vector.h
ui.o: ui.c ui.h
client_net.o: client_net.c client_net.h
protocol.o: protocol.c protocol.h
sender.out: sender.c
	gcc sender.c -o sender.out
	
listener.out: listener.c
	gcc listener.c -o listener.out
	
	
	
