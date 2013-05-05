# Hari Ganesan 2/4/13
# Makefile for ghost-story

# compiler options
CC=g++
CFLAGS=-g3
INCLUDE =-I/usr/include -I/usr/local/include  -O2 -march=x86-64 -fmessage-length=0 -Wall -I/usr/X11R6/include
LIBDIR = -L/usr/X11R6/lib -Wl,-rpath,/usr/X11R6/lib -lfltk_images -lpng  -lz -lfltk_gl -lGLU -lGL -lfltk -lm -lXext -lX11 -lsupc++ -lfontconfig -lXft  -lXinerama
#SDL=-lSDLmain -lSDL -lSDL_ttf -lSDL_mixer -lSDL_image
#COCOA=-framework Cocoa
#GL=-framework OpenGL -L/sw/lib
SRC=src

all: $(SRC)/*.o main clean

# build main
main: main.o Actor.o
	$(CC) $(INCLUDE) $(LIBDIR) $^ -o $@

# build object files
$(SRC)/*.o: $(SRC)/*.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $^

clean:
	rm *.o
