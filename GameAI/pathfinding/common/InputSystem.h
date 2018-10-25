//Used from Karim Elzanaty's and Walter Hill's Game Arch Final Project.

#ifndef INPUT_SYS
#define INPUT_SYS

#include "Defines.h"
#include <cassert>

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
		const Uint8 *mKeyState;

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
		void getMousePos(int &x, int &y);
		//Call mouse and key events
		void update();
};

#endif // !INPUT_SYS
