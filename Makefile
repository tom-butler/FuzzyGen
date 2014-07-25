BUILD_DIR = bin
CMD_DIR = src\cmd
GUI_DIR = src\gui
OBJ_DIR = src\objects
SIM_DIR = src\sims
SIMULATION_DIR = src\sims\sims

all: cmd clean

t: test clean

g: gui clean

c: clear

#CMD BUILD
cmd: cmd.o gen.o controller.o moon.o shared.o sim.o
	g++ -o $(BUILD_DIR)\cmd cmd.o gen.o controller.o moon.o shared.o sim.o -m64

cmd.o: $(CMD_DIR)\cmd.cpp
	g++ -c $(CMD_DIR)\cmd.cpp -m64

#TEST BUILD
test: test.o controller.o moon.o gen.o shared.o
	g++ -o $(BUILD_DIR)\test test.o controller.o moon.o shared.o gen.o

test.o: $(CMD_DIR)\test.cpp
	g++ -c $(CMD_DIR)\test.cpp -m64

#GRAPHICAL USER INTERFACE
gui: gui.o gen.o controller.o moon.o shared.o sim.o simgui.o
	g++ -o $(BUILD_DIR)\gui gui.o gen.o controller.o moon.o shared.o sim.o simgui.o -m64 -L"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows

gui.o: $(GUI_DIR)\gui.cpp
	g++ -c $(GUI_DIR)\gui.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

#SHARED OBJECTS
gen.o: $(OBJ_DIR)\gen.cpp $(OBJ_DIR)\gen.h
	g++ -c $(OBJ_DIR)\gen.cpp -m64

controller.o: $(OBJ_DIR)\controller.cpp $(OBJ_DIR)\controller.h
	g++ -c $(OBJ_DIR)\controller.cpp -m64

shared.o: $(OBJ_DIR)\shared.cpp $(OBJ_DIR)\shared.h
	g++ -c $(OBJ_DIR)\shared.cpp -m64

#SIMULATIONS
moon.o: $(SIMULATION_DIR)\moon.cpp $(SIMULATION_DIR)\moon.h
	g++ -c $(SIMULATION_DIR)\moon.cpp -m64

sim.o: $(SIM_DIR)\sim.cpp $(SIM_DIR)\sim.h
	g++ -c $(SIM_DIR)\sim.cpp -m64

simgui.o: $(SIM_DIR)\simgui.cpp $(SIM_DIR)\simgui.h
	g++ -c $(SIM_DIR)\simgui.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

#UTIL FUNCTIONS
.PHONY : clean
clean:
	del *.o

.PHONY : clear
clear:
	del logs\controller\*.csv logs\gen\*.csv logs\input\*.csv logs\rule\*.csv logs\set\*.csv logs\var\*.csv