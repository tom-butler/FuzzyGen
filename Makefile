SRC_DIR = src
BUILD_DIR = bin
GUI_DIR = src\gui

all: gen clean

t: test clean

g: gui clean

gen: gen.o controller.o sim.o
	g++ -o $(BUILD_DIR)\gen gen.o controller.o sim.o -m64

test: test.o controller.o sim.o gen.o
	g++ -o $(BUILD_DIR)\test test.o controller.o sim.o gen.o

gui: gen.o controller.o guisim.o
	g++ -o $(BUILD_DIR)\gui gen.o controller.o guisim.o -m64 -L"C:\Program Files\mingw-w64\freeglut\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows

test.o: $(SRC_DIR)\test.cpp
	g++ -c $(SRC_DIR)\test.cpp -m64

gen.o: $(SRC_DIR)\gen.cpp $(SRC_DIR)\gen.h
	g++ -c $(SRC_DIR)\gen.cpp -m64

controller.o: $(SRC_DIR)\controller.cpp $(SRC_DIR)\controller.h
	g++ -c $(SRC_DIR)\controller.cpp -m64

sim.o: $(SRC_DIR)\sim.cpp $(SRC_DIR)\sim.h
	g++ -c $(SRC_DIR)\sim.cpp -m64

guisim.o: $(SRC_DIR)\sim.cpp $(GUI_DIR)\sim.h
	g++ -c -o guisim.o $(SRC_DIR)\sim.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\Common Files\MinGW\freeglut\include"

.PHONY : clean
clean:
	del *.o