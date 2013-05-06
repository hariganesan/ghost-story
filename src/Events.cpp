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
					keys.up = true;
				case 'a':
					keys.left = true;
				case 's':
					keys.down = true;
				case 'd':
					keys.right = true;

				// ghost scenarios
				case 'g':
					keys.g = true;

				default: return 1;
			}
	}

	return Fl_Gl_Window::handle(event);
}