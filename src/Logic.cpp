// Hari Ganesan 5/5/13
// ghost-story Logic

#include "Render.hpp"

void MyGlWindow::logic() {
	if (s->mc->isJumping()) {
		s->mc->jump();
	}
}
