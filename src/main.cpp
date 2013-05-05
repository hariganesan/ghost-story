// Hari Ganesan 2/4/13
// zombie-roll: an rpg

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

using namespace std;

void makeWindow();
void runGame();
void render(Stage *s);

int main(int argc, char **argv) {
	makeWindow();

	//runGame();

	
	return Fl::run();
}

void makeWindow() {
	Fl_Window *win = new Fl_Window(600, 400, "Ghost Story");
	win->begin();
	win->end();
	win->show();
}

void runGame() {
	
	// game state
	bool isRunning = true;
	bool collision = false;
	bool moved = false;
	Keys keys = Keys();

	// set up game
	Stage *s = new Stage(GRAVITY_DEF);
	s->mc = new Character(s, "MC", 50, POS_X_START, POS_Y_START, CHAR_W_DEF, CHAR_H_DEF);
	s->mcP = NULL;
	s->enemies[0] = new Character(s, "Enemy1", 20, 400, POS_Y_START, CHAR_W_DEF, CHAR_H_DEF);
	s->enemies[0]->isPossessible = true;

	while (isRunning) {
		// EVENTS
		/*while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || 
				 (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_q)) {
				isRunning = false;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_g) {
				keys.g = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_LEFT) {
				keys.left = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN) {
				keys.down = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RIGHT) {
				keys.right = true;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP) {
				keys.up = true;
			}
		}*/

		// LOGIC
		// collision detection
		// between mc and enemies
		collision = false;

		// basic movement - MC
		moved = false;
		if (s->mc->isJumping) {
				s->mc->jump();
		}

		// if ghost
		if (s->mc->isGhost) {
			if (keys.left) {
				if (s->mcG->moveLeft()) {
					moved = true;
				}
				keys.left = false;
			} else if (keys.right) {
				if (s->mcG->moveRight()) {
					moved = true;
				}
				keys.right = false;
			} else if (keys.down) {
				if (s->mcG->moveDown()) {
					moved = true;
				}
				keys.down = false;		
			} else if (keys.up) {
				if (s->mcG->moveUp()) {
					moved = true;
				}
				keys.up = false;			
			}
			// if possessing other object
		} else if (s->mcP) {
			if (keys.left) {
				if (s->mcP->moveLeft()) {
					moved = true;
				}
				keys.left = false;
			} else if (keys.right) {
				if (s->mcP->moveRight()) {
					moved = true;
				}
				keys.right = false;
			}
		// if solid and no collision
		} else if (!collision) {
			if (keys.left) {
				if (s->mc->moveLeft()) {
					moved = true;
				}
				keys.left = false;
			} else if (keys.right) {
				if (s->mc->moveRight()) {
					moved = true;
				}
				keys.right = false;
			} else if (keys.up) {
				if (!(s->mc->isJumping) && s->mc->jump()) {
					moved = true;
				}
				keys.up = false;
			}

			keys.down = false;
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

		// ghost state

		if (keys.g) {
			if (!(s->mc->isGhost) && s->mc->pillCount > 0) {
				if (s->mcP) {
					s->mcG = s->mc->createGhost(s->mcP->getX(), s->mcP->getY());
					s->mcP->possessed = false;
					s->mcP = NULL;
				} else {
					s->mcG = s->mc->createGhost(s->mc->getX(), s->mc->getY());
				}
			} else if (s->mc->isGhost) {
				for (int i = 0; i < MAX_ENEMY_COUNT; i++) {
					if (s->fullOverlap(s->mcG, s->enemies[i])) {
						s->mcP = s->enemies[i];
						s->enemies[i]->possessed = true;
						s->enemies[i]->tracking = false;
					}
				}
				s->mc->destroyGhost();
			} else {
				s->popUp("no pills remaining!");
			}

			keys.g = false;
		}

		// invincible
		if (s->mc->invincible > 0)
			s->mc->invincible--;
		

		// RENDERING

		render(s);
	}
}

void render(Stage *s) {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	// TODO: change to 0,1 for depth
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1); // set matrix
	stringstream ss;
	location = Location();

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
	//SDL_GL_SwapBuffers();
	//SDL_Delay(1000/SDL_FRAME_RATE); // frame rate 30ms
	return;	
}
