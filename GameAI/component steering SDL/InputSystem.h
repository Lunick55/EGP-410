#ifndef INPUT_SYS
#define INPUT_SYS

#include "Defines.h"
#include <cassert>
//#include "allegro5\events.h"
//#include "allegro5\keyboard.h"
//#include "allegro5\mouse.h"
#include "EventSystem.h"
#include "Event.h"
#include "SDL.h"
/*
Object headers include Trackable in order
to use time and memory managment tool
*/

class InputSystem : public Trackable
{
	private:
		Event mKeyEvent, mMouseEvent;


		//ALLEGRO_KEYBOARD_STATE mKeyState;
		//ALLEGRO_MOUSE_STATE mMouseState;
		const Uint8 *mKeyState;

		//ALLEGRO_EVENT_QUEUE *mInputQueue;
		//ALLEGRO_EVENT mEvents;
		SDL_Event mEvent;


		bool mIsKeyDown; //For getting input

	public:
		InputSystem();
		~InputSystem();

		//Init and cleanup
		bool initInputSystem();
		void cleanupInputSystem();

		//Input and mouse position
		void getKeyEvents();
		void getMouseEvents();
		//ALLEGRO_MOUSE_STATE getMouse();

		//Call mouse and key events
		void update();
};

#endif // !INPUT_SYS
