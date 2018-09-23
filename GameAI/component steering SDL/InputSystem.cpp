#include "InputSystem.h"
#include "PlayerMoveToMessage.h"

InputSystem::InputSystem()
{
	//mInputQueue = NULL;
	mIsKeyDown = false;
}


InputSystem::~InputSystem()
{
	cleanupInputSystem();
}


void InputSystem::cleanupInputSystem()
{

}


bool InputSystem::initInputSystem()
{
	std::cout << "Initialized InputSystem" << std::endl;

	return true;
}


void InputSystem::update()
{
		getKeyEvents();
		getMouseEvents();
}


void InputSystem::getKeyEvents()
{
	// Alegro event queue help @ https://www.youtube.com/watch?v=upgb6HFUep4 
	//al_get_next_event(mInputQueue, &mEvents);
	while (SDL_PollEvent(&mEvent))
	{
		switch (mEvent.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			if (mEvent.button.button == SDL_BUTTON_LEFT)
			{
				cout << "InputSystem: MOUSE LEFT" << endl;
				//mKeyEvent.setType(MOUSE_LEFT);

				int x, y;
				SDL_GetMouseState(&x, &y);
				EventSystem::getInstance()->fireEvent(PlayerMoveToMessage(Vector2D(x,y)));
			}
			break;
		case SDL_KEYDOWN:
			if (mEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				cout << "InputSystem: ESC" << endl;
				mKeyEvent.setType(ESC);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_d)
			{
				cout << "InputSystem: Space" << endl;
				mKeyEvent.setType(D_KEY);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_UP)
			{
				cout << "InputSystem: Up arrow" << endl;
				mKeyEvent.setType(UP_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_DOWN)
			{
				cout << "InputSystem: Down arrow" << endl;
				mKeyEvent.setType(DOWN_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_LEFT)
			{
				cout << "InputSystem: Left arrow" << endl;
				mKeyEvent.setType(LEFT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_RIGHT)
			{
				cout << "InputSystem: Right arrow" << endl;
				mKeyEvent.setType(RIGHT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}

			break;
		}
	};
}


void InputSystem::getMouseEvents()
{
	
}
