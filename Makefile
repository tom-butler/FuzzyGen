SRC_DIR = src
BUILD_DIR = bin

all: gen clean

gen: gen.o controller.o sim.o
	g++ -o $(BUILD_DIR)\test gen.o controller.o sim.o

gen.o: $(SRC_DIR)\gen.cpp $(SRC_DIR)\gen.h
	g++ -c $(SRC_DIR)\gen.cpp

controller.o: $(SRC_DIR)\controller.cpp $(SRC_DIR)\controller.h
	g++ -c $(SRC_DIR)\controller.cpp

sim.o: $(SRC_DIR)\sim.cpp $(SRC_DIR)\sim.h
	g++ -c $(SRC_DIR)\sim.cpp

.PHONY : clean
clean:
	del *.o