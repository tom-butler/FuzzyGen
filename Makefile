BIN_DIR = bin
BUILD_DIR = build
CMD_DIR = src\cmd
GUI_DIR = src\gui
OBJ_DIR = src\shared
LIB_DIR = libs
CONT_DIR = src\shared\controller
CONT_H_DIR = src\shared\controller\h
CONT_CPP_DIR = src\shared\controller\cpp
SIM_DIR = src\sims

all: cmd

t: test clean

g: gui

clean: clean

clear: clear

#CMD BUILD
cmd: $(BUILD_DIR)\cmd.o $(BUILD_DIR)\gen.o $(BUILD_DIR)\shared.o $(BUILD_DIR)\controller.o $(BUILD_DIR)\breed.o $(BUILD_DIR)\create.o $(BUILD_DIR)\mutate.o $(BUILD_DIR)\run.o $(BUILD_DIR)\select.o $(BUILD_DIR)\sim.o $(BUILD_DIR)\moon.o $(BUILD_DIR)\harrier.o $(BUILD_DIR)\pendulum.o
	g++ -o $(BIN_DIR)\cmd $(BUILD_DIR)\cmd.o $(BUILD_DIR)\gen.o $(BUILD_DIR)\shared.o $(BUILD_DIR)\controller.o $(BUILD_DIR)\breed.o $(BUILD_DIR)\create.o $(BUILD_DIR)\mutate.o $(BUILD_DIR)\run.o $(BUILD_DIR)\select.o $(BUILD_DIR)\sim.o $(BUILD_DIR)\moon.o $(BUILD_DIR)\harrier.o $(BUILD_DIR)\pendulum.o -m64

$(BUILD_DIR)\cmd.o: $(CMD_DIR)\cmd.cpp
	g++ -o $(BUILD_DIR)\cmd.o -c -g $(CMD_DIR)\cmd.cpp -m64

#TEST BUILD
test: $(BUILD_DIR)\test.o $(BUILD_DIR)\controller.o $(BUILD_DIR)\moon.o $(BUILD_DIR)\gen.o $(BUILD_DIR)\shared.o
	g++ -o $(BIN_DIR)\test $(BUILD_DIR)\test.o $(BUILD_DIR)\controller.o $(BUILD_DIR)\moon.o $(BUILD_DIR)\shared.o $(BUILD_DIR)\gen.o

$(BUILD_DIR)\test.o: $(CMD_DIR)\test.cpp
	g++ -o $(BIN_DIR)\test.o -c -g $(CMD_DIR)\test.cpp -m64

#GRAPHICAL USER INTERFACE
gui: $(BUILD_DIR)\gui.o $(BUILD_DIR)\simgui.o $(BUILD_DIR)\gen.o $(BUILD_DIR)\shared.o $(BUILD_DIR)\controller.o $(BUILD_DIR)\breed.o $(BUILD_DIR)\create.o $(BUILD_DIR)\mutate.o $(BUILD_DIR)\run.o $(BUILD_DIR)\select.o $(BUILD_DIR)\sim.o $(BUILD_DIR)\moon.o $(BUILD_DIR)\moondraw.o $(BUILD_DIR)\harrier.o $(BUILD_DIR)\harrierdraw.o $(BUILD_DIR)\pendulum.o $(BUILD_DIR)\pendulumdraw.o
	g++ -o $(BIN_DIR)\gui $(BUILD_DIR)\gui.o $(BUILD_DIR)\simgui.o $(BUILD_DIR)\gen.o $(BUILD_DIR)\shared.o $(BUILD_DIR)\controller.o $(BUILD_DIR)\breed.o $(BUILD_DIR)\create.o $(BUILD_DIR)\mutate.o $(BUILD_DIR)\run.o $(BUILD_DIR)\select.o $(BUILD_DIR)\sim.o $(BUILD_DIR)\moon.o $(BUILD_DIR)\moondraw.o $(BUILD_DIR)\harrier.o $(BUILD_DIR)\harrierdraw.o $(BUILD_DIR)\pendulum.o $(BUILD_DIR)\pendulumdraw.o -m64 -L"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows

$(BUILD_DIR)\gui.o: $(GUI_DIR)\gui.cpp $(GUI_DIR)\gui.h
	g++ -o $(BUILD_DIR)\gui.o -c $(GUI_DIR)\gui.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

$(BUILD_DIR)\simgui.o: $(SIM_DIR)\simgui.cpp $(SIM_DIR)\simgui.h
	g++ -o $(BUILD_DIR)\simgui.o -c $(SIM_DIR)\simgui.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

$(BUILD_DIR)\lodepng.o: $(LIB_DIR)\lodepng\lodepng.cpp $(LIB_DIR)\lodepng\lodepng.h
	g++ -o $(BUILD_DIR)\lodepng.o -c $(LIB_DIR)\lodepng\lodepng.cpp -m64

#SHARED OBJECTS
$(BUILD_DIR)\gen.o: $(OBJ_DIR)\gen.cpp $(OBJ_DIR)\gen.h
	g++ -o $(BUILD_DIR)\gen.o -c -g $(OBJ_DIR)\gen.cpp -m64

$(BUILD_DIR)\shared.o: $(OBJ_DIR)\shared.cpp $(OBJ_DIR)\shared.h
	g++ -o $(BUILD_DIR)\shared.o -c -g $(OBJ_DIR)\shared.cpp -m64

$(BUILD_DIR)\controller.o: $(CONT_DIR)\controller.cpp $(CONT_DIR)\controller.h
	g++ -o $(BUILD_DIR)\controller.o -c -g $(CONT_DIR)\controller.cpp -m64

#CONTROLLER OBJECTS
$(BUILD_DIR)\breed.o: $(CONT_CPP_DIR)\breed.cpp $(CONT_H_DIR)\breed.h
	g++ -o $(BUILD_DIR)\breed.o -c -g $(CONT_CPP_DIR)\breed.cpp -m64

$(BUILD_DIR)\create.o: $(CONT_CPP_DIR)\create.cpp $(CONT_H_DIR)\create.h
	g++ -o $(BUILD_DIR)\create.o -c -g $(CONT_CPP_DIR)\create.cpp -m64

$(BUILD_DIR)\mutate.o: $(CONT_CPP_DIR)\mutate.cpp $(CONT_H_DIR)\mutate.h
	g++ -o $(BUILD_DIR)\mutate.o -c -g $(CONT_CPP_DIR)\mutate.cpp -m64

$(BUILD_DIR)\run.o: $(CONT_CPP_DIR)\run.cpp $(CONT_H_DIR)\run.h
	g++ -o $(BUILD_DIR)\run.o -c -g $(CONT_CPP_DIR)\run.cpp -m64

$(BUILD_DIR)\select.o: $(CONT_CPP_DIR)\select.cpp $(CONT_H_DIR)\select.h
	g++ -o $(BUILD_DIR)\select.o -c -g $(CONT_CPP_DIR)\select.cpp -m64

#SIMULATIONS
$(BUILD_DIR)\moon.o: $(SIM_DIR)\moon\moon.cpp $(SIM_DIR)\moon\moon.h
	g++ -o $(BUILD_DIR)\moon.o -c -g $(SIM_DIR)\moon\moon.cpp -m64

$(BUILD_DIR)\moondraw.o: $(SIM_DIR)\moon\moondraw.cpp $(SIM_DIR)\moon\moondraw.h
	g++ -o $(BUILD_DIR)\moondraw.o -c $(SIM_DIR)\moon\moondraw.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

$(BUILD_DIR)\pendulum.o: $(SIM_DIR)\pendulum\pendulum.cpp $(SIM_DIR)\pendulum\pendulum.h
	g++ -o $(BUILD_DIR)\pendulum.o -c -g $(SIM_DIR)\pendulum\pendulum.cpp -m64

$(BUILD_DIR)\pendulumdraw.o: $(SIM_DIR)\pendulum\pendulumdraw.cpp $(SIM_DIR)\pendulum\pendulumdraw.h
	g++ -o $(BUILD_DIR)\pendulumdraw.o -c $(SIM_DIR)\pendulum\pendulumdraw.cpp -m64 -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

$(BUILD_DIR)\harrier.o: $(SIM_DIR)\harrier\harrier.cpp $(SIM_DIR)\harrier\harrier.h
	g++ -o $(BUILD_DIR)\harrier.o -c -g $(SIM_DIR)\harrier\harrier.cpp -m64

$(BUILD_DIR)\harrierdraw.o: $(SIM_DIR)\harrier\harrierdraw.cpp $(SIM_DIR)\harrier\harrierdraw.h
	g++ -o $(BUILD_DIR)\harrierdraw.o -c $(SIM_DIR)\harrier\harrierdraw.cpp -m64 -lSOIL -D FREEGLUT_STATIC -I"C:\Program Files\mingw-w64\x86_64-4.9.0-posix-sjlj-rt_v3-rev2\mingw64\include"

$(BUILD_DIR)\sim.o: $(SIM_DIR)\sim.cpp $(SIM_DIR)\sim.h
	g++ -o $(BUILD_DIR)\sim.o -c -g $(SIM_DIR)\sim.cpp -m64

#UTIL FUNCTIONS
.PHONY : clean
clean:
	del $(BUILD_DIR)\*.o

.PHONY : clear
clear:
	del logs\controller\*.csv logs\gen\*.csv logs\input\*.csv logs\rule\*.csv logs\set\*.csv logs\var\*.csv