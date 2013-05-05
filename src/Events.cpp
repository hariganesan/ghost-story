// Hari Ganesan
// ghost-story events

#include "Render.hpp"

int MyGlWindow::handle(int event) {
	switch(event) {
		case FL_FOCUS:
		case FL_UNFOCUS:
			return 1;

		case FL_KEYBOARD:
			if (Fl::event_key() == 'g') {
				//s->mcG = s->mc->createGhost(s->mcP->getX(), s->mcP->getY());
				//s->mcP->possessed = false;
				//s->mcP = NULL;
				//redraw();
				std::cout << "pressed g" << std::endl;
				return 1;
			} else {
				return 0;
			}
	}

	return Fl_Gl_Window::handle(event);
}