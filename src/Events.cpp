// Hari Ganesan
// ghost-story events

#include "Render.hpp"

int MyGlWindow::handle(int event) {
	switch(event) {
		case KEY:
			if (event_key() == 'a') {
				std::cout << "hit a" << std::endl;
				return 1;
			} else {
				return 0;
			}
		default:
			return Fl_Gl_Window::handle(event);
	}

	return EXIT_FAILURE;
}