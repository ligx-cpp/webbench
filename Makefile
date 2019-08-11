###########################################
#Makefile for simple programs
###########################################
CC=g++
CC_FLAG=-g -Wall 
SRCS=main
OBJS=main.o socket_connect.o request.o bench.o 
$(SRCS):$(OBJS)	
	$(CC) -o $@ $(OBJS)	
.SUFFIXES: .o .cpp
.cpp.o:	
	$(CC) $(CC_FLAG) -c $*.cpp -o $*.o 
.PRONY:clean
clean:	
	@echo "Removing linked and compiled files......"	
	rm -f $(OBJS) $(SRCS)
