// Hari Ganesan
// ghost-story events

#include "Render.hpp"

int MyGlWindow::handle(int event) {
	switch(event) {
		case FL_KEYBOARD:
			if (Fl::event_key() == 'g') {
				s->mcG = s->mc->createGhost(s->mcP->getX(), s->mcP->getY());
				s->mcP->possessed = false;
				s->mcP = NULL;
				redraw();
				return 1;
			} else {
				return 0;
			}
		default:
			return Fl_Gl_Window::handle(event);
	}

	return EXIT_FAILURE;
}