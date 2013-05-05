// Hari Ganesan 2/4/13
// main.cpp for ghost-story SDL version

#include <iostream>
#include <sstream>
#include <string>
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_opengl.h"
#include "SDL_ttf/SDL_ttf.h"
#include "Actor.hpp"

using namespace std;

// font
TTF_Font *font;
const char *fontpath = "assets/fonts/chintzy.ttf";

void runGame();
void render(Stage *s);
void SDL_GL_RenderText(const char *text, SDL_Color color, SDL_Rect *location);

int main(int argc, char **argv) {
	// initialize sdl, ttf, and opengl
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// initialize window properties
	SDL_WM_SetCaption("Ghost Story", NULL);
	SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL);
	glClearColor(0, 0, SKY_COLOR, 1); // RGBA
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // viewable part of the window
	glShadeModel(GL_SMOOTH); // add a gradient
	glMatrixMode(GL_PROJECTION); // 2D drawing
	glLoadIdentity(); // save state
	glDisable(GL_DEPTH_TEST); // disable 3D drawing

  srand(time(NULL));

	runGame();

	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	return EXIT_SUCCESS;
}

void runGame() {
	SDL_Event event;
	font = TTF_OpenFont(fontpath, 16);

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
		while (SDL_PollEvent(&event)) {
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
		}

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
	SDL_Color TEXT_WHITE = {200, 200, 200};
	//SDL_Color TEXT_BLACK = {20, 20, 20};
	//SDL_Color TEXT_RED = {150, 0, 0};
	SDL_Rect location;
	stringstream ss;

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
	SDL_GL_RenderText(ss.str().c_str(), TEXT_WHITE, &location);
	ss.str("");

	// pills
	location.x = 20;
	location.y = SBAR_TEXT_HEIGHT;
	ss << s->mc->pillCount;
	SDL_GL_RenderText(("Pills: " + ss.str()).c_str(), TEXT_WHITE, &location);		


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
	SDL_GL_SwapBuffers();
	SDL_Delay(1000/SDL_FRAME_RATE); // frame rate 30ms
	return;	
}

void SDL_GL_RenderText(const char *text, SDL_Color color, SDL_Rect *location) {
	SDL_Surface *initial;
	SDL_Surface *intermediary;
	int w,h;
	GLuint texture;

	if (!text) {
		cerr << "text not displayed" << endl;
		return;
	}
	
	// Use SDL_TTF to render our text 
	initial = TTF_RenderText_Blended(font, text, color);
	
	// Convert the rendered text to a known format 
	w = initial->w;
	h = initial->h;
	
	intermediary = SDL_CreateRGBSurface(0, w, h, 32, 
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

	SDL_BlitSurface(initial, 0, intermediary, 0);
	
	// Tell GL about our new texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
			GL_UNSIGNED_BYTE, intermediary->pixels );
	
	// GL_NEAREST looks horrible, if scaled... 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

	// prepare to render our texture 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glColor3f(1.0f, 1.0f, 1.0f);
	
	// Draw a quad at location 
	glBegin(GL_QUADS);
		// Recall that the origin is in the lower-left corner
		//  That is why the TexCoords specify different corners
		//  than the Vertex coors seem to. 
	glTexCoord2f(0.0f, 0.0f); 
	glVertex2f(location->x    , location->y);
	glTexCoord2f(1.0f, 0.0f); 
	glVertex2f(location->x + w, location->y);
	glTexCoord2f(1.0f, 1.0f); 
	glVertex2f(location->x + w, location->y + h);
	glTexCoord2f(0.0f, 1.0f); 
	glVertex2f(location->x    , location->y + h);
	glEnd();
	
	// Bad things happen if we delete the texture before it finishes 
	glFinish();
	
	// return the deltas in the unused w,h part of the rect 
	location->w = initial->w;
	location->h = initial->h;
	
	// Clean up 
	SDL_FreeSurface(initial);
	SDL_FreeSurface(intermediary);
	glDeleteTextures(1, &texture);
}


