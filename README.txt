ABOUT
~~~~~
FuzzyGen
Version 0.5
Written by Tom Butler
with assistance from Robert Cox
Lander image is creative commons: source: http://historicspacecraft.com/Lunar_Module.html


DEVELOPMENT INSTALLATION
~~~~~~~~~~~~~~~~~~~~~~~~
-install MinGW-w64 found here: http://sourceforge.net/projects/mingw-w64/
-add the /bin folder to your system path variable
-download freeGlut MinGW package found here: http://www.transmissionzero.co.uk/software/freeglut-devel/
-place freeglut/bin/x64/freeeglut.dll in the mingw64/bin folder
-place freeglut/include/GL in mingw64/include
-place contents of freeglut/lib in mingw64/lib

-check Makefile for the correct path in gui and gui.o

BUILDING
~~~~~~~~
make will build the cmd version
make t will build the cmd test version
make g will build the gui test version

