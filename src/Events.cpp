// Hari Ganesan 5/5/13
// ghost-story events

#include "Render.hpp"

int MyGlWindow::handle(int event) {
	switch(event) {
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;

		case FL_KEYBOARD:
			switch (Fl::event_key()) {
				// WASD movement
				case 'w':
					if (s->mc->isGhost) {
						s->mcG->moveUp();
					} else if (!s->mc->isJumping) {
						s->mc->isJumping = true;
						s->mc->jump();
					}
					redraw(); return 1;
				case 'a':
					if (s->mc->isGhost) {
						s->mcG->moveLeft();
					} else {
						s->mc->moveLeft();
					}
					redraw(); return 1;
				case 's':
					if (s->mc->isGhost) {
						s->mcG->moveDown();
					}
					redraw(); return 1;
				case 'd':
					if (s->mc->isGhost) {
						s->mcG->moveRight();
					} else {
						s->mc->moveRight();
					}
					redraw(); return 1;

				// ghost scenarios
				case 'g':
					if (!s->mc->isGhost && s->mc->pillCount > 0) {
						if (s->mcP) {
							s->mcG = s->mc->createGhost(s->mcP->getX(), s->mcP->getY());
							s->mcP->possessed = false;
							s->mcP = NULL;
							redraw(); return 1;
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


			}
	}

	return Fl_Gl_Window::handle(event);
}