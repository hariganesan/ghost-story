// Hari Ganesan 5/5/13
// ghost-story GL commands library file

#include <iostream>
#include <sstream>
#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_File_Chooser.H>		// FLTK file chooser
// open gl
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <GL/glu.h>

#include "Actor.hpp"

// OPENGL WINDOW CLASS
class MyGlWindow {
	Stage *s;
	double fg;                       // foreground brightness
	double bg;                       // background brightness

	void FixViewport(int W,int H);
	void draw ();
	void resize(int X,int Y,int W,int H);
public:
	MyGlWindow(Stage *g, int X,int Y,int W,int H,const char*L=0);
	void SetBrightness(double new_fg, double new_bg);
};