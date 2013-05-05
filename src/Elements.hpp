// Hari Ganesan
// Elements file
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

// window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int MARGIN = 10;
const int SBAR_HEIGHT = 50;
const int SBAR_TEXT_HEIGHT = 20;

// characters
const int CHAR_W_DEF = 80;
const int CHAR_H_DEF = 100;
const int MOTION_DIM = 20;
const int MAX_ENEMY_COUNT = 1;
const int POS_X_START = 100;
const int POS_Y_START = WINDOW_HEIGHT-50-CHAR_H_DEF;
const int VEL_INIT_DEF = 50;
const double GHOST_SHRINK = 0.65;

// environment
const int SKY_COLOR = 3;
const int GROUND_HEIGHT = -WINDOW_HEIGHT+50;
const int GRAVITY_DEF = 10;

// sdl
//const int SDL_FRAME_RATE = 30;

// events to be used to hold info for game logic
struct Keys {
	bool right;
	bool left;
	bool up;
	bool down;
	bool g;
};

struct Location {
	double x;
	double y;
};
