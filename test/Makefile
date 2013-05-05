# Hari Ganesan 2/4/13
# Makefile for ghost-story testing

# compiler options
CC=g++
CFLAGS=-g3
INCLUDE = -I/usr/include -I/usr/local/include -O2 -march=x86-64 -fmessage-length=0 -Wall -I/usr/X11R6/include
LIBS = -L/usr/X11R6/lib -Wl,-rpath,/usr/X11R6/lib -lfltk_images -lpng  -lz -lfltk_gl \
	-lGLU -lGL -lfltk -lm -lXext -lX11 -lsupc++ -lfontconfig -lXft -lXinerama
#LIBDIR = -lfltk -fltk_gl -lfltk_gl -lGLU -lGL -lm -lpng -lX11 -lfontconfig -lXft -lXinerama

#SDL=-lSDLmain -lSDL -lSDL_ttf -lSDL_mixer -lSDL_image
#COCOA=-framework Cocoa
#GL=-framework OpenGL -L/sw/lib
SRC=src

all: *.o test clean

# build test
test: test.cpp
	$(CC) $^ $(INCLUDE) $(LIBS) -o $@

# build object files
*.o: *.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $^

clean:
	rm *.o
