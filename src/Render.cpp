// Hari Ganesan 5/5/13
// ghost-story GL commands

#include "Render.hpp"

// OPENGL WINDOW CLASS
class MyGlWindow : public Fl_Gl_Window {
	double fg;                       // foreground brightness
	double bg;                       // background brightness
	// FIX OPENGL VIEWPORT
	//     Do this on init or when window's size is changed
	void FixViewport(int W,int H) {
		glLoadIdentity();
		glViewport(0,0,W,H);
		glOrtho(-W,W,-H,H,-1,1);
	}

	// DRAWING

	void draw () {
		if (!valid()) { valid(1); FixViewport(w(), h()); }      // first time? init

    // Clear screen to bg color
    glClearColor(bg, bg, bg, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glPushMatrix();
		// TODO: change to 0,1 for depth
		stringstream ss;
		Location location = Location();

		////////////////
		// BEGIN DRAWING
		////////////////


		// draw status bar
		glColor3ub(0,0,0);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(WINDOW_WIDTH, 0);
		glVertex2f(WINDOW_WIDTH, SBAR_HEIGHT);
		glVertex2f(0, SBAR_HEIGHT);
		glEnd();

		// HP
		location.x = WINDOW_WIDTH-110;
		location.y = SBAR_TEXT_HEIGHT;
		ss << "HP: " << s->mc->hitPoints << "/" << s->mc->maxHitPoints;
		//SDL_GL_RenderText(ss.str().c_str(), TEXT_WHITE, &location);
		ss.str("");

		// pills
		location.x = 20;
		location.y = SBAR_TEXT_HEIGHT;
		ss << s->mc->pillCount;
		//SDL_GL_RenderText(("Pills: " + ss.str()).c_str(), TEXT_WHITE, &location);		


		// draw MC
		if (s->mc->invincible % 2 == 1) {
			glColor3ub(0, 0, 200);
		} else {
			glColor3ub(255, 255, 255);
		}

		glBegin(GL_QUADS);
		glVertex2f(s->mc->getX(), s->mc->getY());
		glVertex2f(s->mc->getX()+s->mc->getW(), s->mc->getY());
		glVertex2f(s->mc->getX()+s->mc->getW(), s->mc->getY()+s->mc->getH());
		glVertex2f(s->mc->getX(), s->mc->getY()+s->mc->getH());
		glEnd();

		// draw other chars
		glColor3ub(200, 0, 0);
		for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
			Character *enemy = s->enemies[i];
			if (enemy) {
				glBegin(GL_QUADS);
				glVertex2f(enemy->getX(), enemy->getY());
				glVertex2f(enemy->getX()+enemy->getW(), enemy->getY());
				glVertex2f(enemy->getX()+enemy->getW(), enemy->getY()+enemy->getH());
				glVertex2f(enemy->getX(), enemy->getY()+enemy->getH());
				glEnd();
			}
		}
		
		// draw ghost
		if (s->mc->isGhost) {
			glColor3ub(125, 125, 125);
			glBegin(GL_QUADS);
			glVertex2f(s->mcG->getX(), s->mcG->getY());
			glVertex2f(s->mcG->getX()+s->mcG->getW(), s->mcG->getY());
			glVertex2f(s->mcG->getX()+s->mcG->getW(), s->mcG->getY()+s->mcG->getH());
			glVertex2f(s->mcG->getX(), s->mcG->getY()+s->mcG->getH());
			glEnd();
		}

		// draw ground
		glColor3ub(150, 50, 0);
		glBegin(GL_QUADS);
		glVertex2f(0, GROUND_HEIGHT);
		glVertex2f(WINDOW_WIDTH, GROUND_HEIGHT);
		glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
		glVertex2f(0, WINDOW_HEIGHT);
		glEnd();

		////////////////
		// END DRAWING
		////////////////

		glPopMatrix();
	}

	// HANDLE WINDOW RESIZING
	void resize(int X,int Y,int W,int H) {
		Fl_Gl_Window::resize(X,Y,W,H);
		FixViewport(W,H);
		redraw();
	}

public:
	// OPENGL WINDOW CONSTRUCTOR
	MyGlWindow(int X,int Y,int W,int H,const char*L=0) : Fl_Gl_Window(X,Y,W,H,L) {
		fg = 1.0;
		bg = 0.0;
		end();
	}

	void SetBrightness(double new_fg, double new_bg) {
		fg = new_fg; bg = new_bg; redraw(); 
	}
};
