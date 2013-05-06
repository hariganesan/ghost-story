// Hari Ganesan 2/4/13
// zombie-roll: an rpg

#include "Render.hpp"


int main(int argc, char **argv) {
	// set up game
	Stage *s = new Stage(GRAVITY_DEF);
	s->mc = new Character(s, "MC", 50, POS_X_START, POS_Y_START, CHAR_W_DEF, CHAR_H_DEF);
	s->mcP = NULL;
	s->enemies[0] = new Character(s, "Enemy1", 20, 400, POS_Y_START, CHAR_W_DEF, CHAR_H_DEF);
	s->enemies[0]->isPossessible = true;

	// create window and run
	Fl_Window win(WINDOW_WIDTH, WINDOW_HEIGHT, "Ghost Story");
	MyGlWindow mygl(s, 10, 10, win.w()-20, win.h()-20);
	win.end();
	win.resizable(mygl);
	win.show();
	return (Fl::run());
}
