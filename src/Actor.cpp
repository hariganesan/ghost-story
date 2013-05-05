// Hari Ganesan
// Actor functions

#include "Actor.hpp"

bool DynamicActor::moveLeft() {
	x -= MOTION_DIM;

	return true;
}

bool DynamicActor::moveRight() {
	x += MOTION_DIM;

	return true;
}

bool DynamicActor::moveLeft(int delta) {
	x -= delta;

	return true;
}

bool DynamicActor::moveRight(int delta) {
	x += delta;

	return true;
}

bool Ghost::moveUp() {
	if (y-MOTION_DIM < SBAR_HEIGHT)
		return false;

	y -= MOTION_DIM;

	return true;
}

bool Ghost::moveDown() {
	if (y+MOTION_DIM+h > GROUND_HEIGHT)
		return false;

	y += MOTION_DIM;

	return true;
}

bool Character::jump() {
	if (isJumping) {
		if (++jumpCount*s->gravity > 2*VEL_INIT_DEF) {
			return (isJumping = false);
		}

		y = (y+jumpFn(VEL_INIT_DEF, (jumpCount-1))) - 
				jumpFn(VEL_INIT_DEF, jumpCount);

		return false;
	}

	jumpCount = 0;
	return (isJumping = true);
}

// x(t) = v(0)t + 1/2*a*t^2
int Character::jumpFn(double initialV, int timeElapsed) {
	return initialV*timeElapsed - s->gravity*timeElapsed*timeElapsed/2;
}

Ghost *Character::createGhost(int newX, int newY) {
	ghost = new Ghost(id + "_ghost", 
										newX+(w/2*(1-GHOST_SHRINK)), newY+(h/2*(1-GHOST_SHRINK)), 
										w*GHOST_SHRINK, h*GHOST_SHRINK);
	isGhost = true;
	pillCount--;

	return ghost;
}

void Character::destroyGhost() {
	delete ghost;
	isGhost = false;
	//pillCount--;
}

void Character::takeDamage(int damage) {
	if (invincible <= 0) {
		hitPoints -= damage;

		invincible = 50;
	}
}

void Stage::popUp(string msg) {
	cerr << msg << endl;
}

// 100 percent of c1 in c2
bool Stage::fullOverlap(DynamicActor *c1, DynamicActor *c2) {
	if (c1->getX() > c2->getX() && c1->getX()+c1->getW() < c2->getX()+c2->getW()
	 && c1->getY() > c2->getY() && c1->getY()+c1->getH() < c2->getY()+c2->getH())
		return true;

	return false;
}

