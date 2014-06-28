SRC_DIR = src
BUILD_DIR = bin

all: test clean

test: test.o controller.o sim.o
	g++ -o $(BUILD_DIR)\test test.o controller.o

test.o: $(SRC_DIR)\test.cpp
	g++ -c $(SRC_DIR)\test.cpp

controller.o: $(SRC_DIR)\controller.cpp $(SRC_DIR)\controller.h
	g++ -c $(SRC_DIR)\controller.cpp

sim.o: $(SRC_DIR)\sim.cpp $(SRC_DIR)\sim.h
	g++ -c $(SRC_DIR)\sim.cpp

.PHONY : clean
clean:
	del *.o