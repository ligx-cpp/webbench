###########################################
#Makefile for simple programs
###########################################
CC=g++
CC_FLAG=-std=c++11 -g -Wall
INCLUDE=-I.
LIB=-pthread


SRCS=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

target=main
$(target):$(OBJS)
	$(CC) $(LIB) $^ -o $@
%.o:%.cpp
	$(CC) -c $^ $(CC_FLAG) $(INCLUDE) -o $@
.PRONY:clean
clean:	
	@echo "Removing linked and compiled files......"	
	rm -f *.o
