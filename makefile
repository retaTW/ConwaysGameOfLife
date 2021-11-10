CC = g++
CFLAGS = -Wall -std=c++14 -O3
EXE = main.exe
SRC = $(wildcard ./*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

.PHONY: all clean

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

%.o:%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o *.exe
	
