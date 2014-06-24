SRC_DIR = src
BUILD_DIR = bin

all: test clean

test: test.o controller.o
	g++ -o $(BUILD_DIR)\test test.o controller.o

test.o: $(SRC_DIR)\test.cpp
	g++ -c $(SRC_DIR)\test.cpp

controller.o: $(SRC_DIR)\controller.cpp $(SRC_DIR)\controller.h
	g++ -c $(SRC_DIR)\controller.cpp

.PHONY : clean
clean:
	del *.o