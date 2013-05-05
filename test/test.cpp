// test.cpp

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

//
// Simple resizable 2D GL window
// erco 10/08/05
//

class MyGlWindow : public Fl_Gl_Window {
    // DRAW METHOD
    //      OpenGL window: (w,h) is upper right, (-w,-h) is lower left, (0,0) is center
    //
    void draw() {
        // First time? init viewport, etc.
        if (!valid()) {
            valid(1);
            glLoadIdentity();
            glViewport(0,0,w(),h());
            glOrtho(-w(),w(),-h(),h(),-1,1);
        }
        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT);
        // Draw white 'X'
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINE_STRIP); glVertex2f(w(), h()); glVertex2f(-w(),-h()); glEnd();
        glBegin(GL_LINE_STRIP); glVertex2f(w(),-h()); glVertex2f(-w(), h()); glEnd();
    }
    // HANDLE WINDOW RESIZING
    //    If window reshaped, need to readjust viewport/ortho
    //
    void resize(int X,int Y,int W,int H) {
        Fl_Gl_Window::resize(X,Y,W,H);
        glLoadIdentity();
        glViewport(0,0,W,H);
        glOrtho(-W,W,-H,H,-1,1);
        redraw();
    }
public:
    // CONSTRUCTOR
    MyGlWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {
    }
};
// MAIN
int main() {
     Fl_Window win(500, 300, "OpenGL X");
     MyGlWindow mygl(10, 10, win.w()-20, win.h()-20);
     win.end();
     win.resizable(mygl);
     win.show();
     return(Fl::run());
}