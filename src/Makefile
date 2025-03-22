OBJS = $(wildcard *.cpp)

CC = g++

COMPILER_FLAGS = -Wall -Werror -std=c++20

OBJ_NAME = main 

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) -o $(OBJ_NAME)

clean:
	rm -rf $(OBJ_NAME)