.SUFFIXES:
.SUFFIXES:      .cpp .o .h

FMOD		= fmod/4.3
WFMOD		= fmod/Win-4.3
MFMOD		= /Developer/FMOD\ Programmers\ API\ Mac/api/inc

O			= Labyrinthe.o Chasseur.o
OBJ			= Labyrinthe.obj Chasseur.obj

WLIBGLUT	= Libs/glut32.lib
WINCGLUT	= Libs/include

LIBS64		= $(FMOD)/libfmodex64.so -lglut -lGLU -lGL -L jpeg -ljpeg -lm
LIBS		= $(FMOD)/libfmodex.so -lglut -lGLU -lGL -ljpeg -lm
WLIBS		= $(WLIBGLUT) -defaultlib:OPENGL32 -defaultlib:GLU32 gdi32.lib jpeg/libjpeg.lib $(WFMOD)/fmodex_vc.lib
MLIBS		= -framework GLUT -framework OpenGL -framework Foundation jpeg/libjpeg.a /Developer/FMOD\ Programmers\ API\ Mac/api/lib/libfmodex.dylib

CXX			= g++
CXXFLAGS	= -I $(MFMOD) -I $(FMOD) -I jpeg -O4 -Wall
CPP			= cl -nologo
CPPFLAGS	= -GX -GR -MD -O2 -I $(WFMOD) -I $(WINCGLUT) -I jpeg

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

%.obj : %.cpp
	$(CPP) $(CPPFLAGS) -c $<

cstrike:	$(O)
	$(CXX) -o $@ $(O) OpenGL.o $(LIBS)

cstrike64:	$(O)
	$(CXX) -o $@ $(O) OpenGL64.o $(LIBS64)

macstrike:	$(O)
	$(CXX) -o $@ MacOpenGL.o $(O) $(MLIBS)
	install_name_tool -change ./libfmodex.dylib /Developer/FMOD\ Programmers\ API\ Mac/api/lib/libfmodex.dylib $@

cstrike.exe: Labyrinthe.obj Chasseur.obj
	link -nologo -out:$@ Labyrinthe.obj Chasseur.obj $(WLIBS) -defaultlib:OpenGL.lib

clean:
	@rm -f cstrike $(O) macstrike cstrike.exe $(OBJ)

Labyrinthe.o:	Labyrinthe.h Chasseur.h Gardien.h
Chasseur.o: Chasseur.cpp Chasseur.h
