// Hari Ganesan 2/4/13
// ghost-story main file

#include "Render.hpp"

int main(int argc, char **argv) {

	// set up game
	Stage *s = new Stage(GRAVITY_DEF);
	s->mc = new Character(s, "MC", 50, POS_X_START, POS_Y_START, CHAR_W_DEF, CHAR_H_DEF);
	s->mcP = NULL;
	s->enemies[0] = new Character(s, "Enemy1", 20, 400, POS_Y_START, CHAR_W_DEF, CHAR_H_DEF);
	s->enemies[0]->isPossessible = true;

	Fl_Window win(WINDOW_WIDTH, WINDOW_HEIGHT, "Ghost Story");
	MyGlWindow mygl(s, 10, 10, win.w()-20, win.h()-20);
	win.end();
	win.resizable(mygl);
	win.show();
	return (Fl::run());
}

/*
		// LOGIC
		// collision detection
		// between mc and enemies
		collision = false;

		// basic movement - MC
		moved = false;
		if (s->mc->isJumping) {
				s->mc->jump();
		}

		// basic movement - enemies (no collisions possible in x dim)
		for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
			if (s->enemies[i] && s->enemies[i]->tracking) {
				if (s->mc->getX()+s->mc->getW() < s->enemies[i]->getX()) {
					s->enemies[i]->moveLeft(2);
				} else if (s->mc->getX() > s->enemies[i]->getX()+s->enemies[i]->getW()){
					s->enemies[i]->moveRight(2);
				} else {
					s->mc->takeDamage(s->enemies[i]->attack);
				}
			}
		}


		// invincible
		if (s->mc->invincible > 0)
			s->mc->invincible--;
		
*/