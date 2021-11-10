CC = g++
CFLAGS = -std=c++17
EXE = main.exe
SRC = $(wildcard ./*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))
DEP = .dependency

.PHONY: all clean dep

all: $(OBJ)
	$(CC) -o $(EXE) $(OBJ)

dep:
	$(CC) -MM $(SRC) > .dependency

clean:
	rm -f *.o *.exe $(DEP)
	
%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<