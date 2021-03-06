// Hari Ganesan 5/5/13
// ghost-story Logic

#include "Render.hpp"

			// LOGIC
void MyGlWindow::logic() {
	// collision detection
	// between mc and enemies
	bool collision = false;

	// basic movement - MC
	bool moved = false;
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
	

}
