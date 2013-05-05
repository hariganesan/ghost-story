// Hari Ganesan
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
					}
					redraw(); return 1;
				case 'a':
					if (s->mc->isGhost) {
						s->mcG->moveLeft();
					} else if (!s->mc->isGhost) {
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
					} else if (!s->mc->isGhost) {
						s->mc->moveRight();
					}
					redraw(); return 1;

				// ghost
				case 'g':
					if (!s->mc->isGhost) {
						s->mcG = s->mc->createGhost(s->mcP->getX(), s->mcP->getY());
						s->mcP->possessed = false;
						s->mcP = NULL;
						redraw(); return 1;
					}
			}
	}

	return Fl_Gl_Window::handle(event);
}