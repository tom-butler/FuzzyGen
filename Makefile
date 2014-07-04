BUILD_DIR = bin
CMD_DIR = src\cmd
GUI_DIR = src\gui
OBJ_DIR = src\objects

all: cmd clean

t: test clean

g: gui clean

#CMD BUILD
cmd: cmd.o gen.o controller.o sim.o
	g++ -o $(BUILD_DIR)\cmd cmd.o gen.o controller.o sim.o -m64

cmd.o: $(CMD_DIR)\cmd.cpp
	g++ -c $(CMD_DIR)\cmd.cpp -m64

#TEST BUILD
test: test.o controller.o sim.o gen.o
	g++ -o $(BUILD_DIR)\test test.o controller.o sim.o gen.o

test.o: $(CMD_DIR)\test.cpp
	g++ -c $(CMD_DIR)\test.cpp -m64

#GRAPHICAL USER INTERFACE
gui: gui.o gen.o controller.o sim.o
	g++ -o $(BUILD_DIR)\gui gui.o gen.o controller.o sim.o -m64 -L"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows

gui.o: $(GUI_DIR)\gui.cpp $(GUI_DIR)\gui.h
	g++ -c $(GUI_DIR)\gui.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

#SHARED OBJECTS
gen.o: $(OBJ_DIR)\gen.cpp $(OBJ_DIR)\gen.h
	g++ -c $(OBJ_DIR)\gen.cpp -m64

controller.o: $(OBJ_DIR)\controller.cpp $(OBJ_DIR)\controller.h
	g++ -c $(OBJ_DIR)\controller.cpp -m64

sim.o: $(OBJ_DIR)\sim.cpp $(OBJ_DIR)\sim.h
	g++ -c $(OBJ_DIR)\sim.cpp -m64

#UTIL FUNCTIONS
.PHONY : clean
clean:
	del *.o