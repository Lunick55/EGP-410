#include "InputSystem.h"
#include "../game/PathToMessage.h"
#include "Game.h"
#include "../game/GameApp.h"
#include "Grid.h"


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

				int x, y;

				if (SDL_GetMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
				{
					static Vector2D lastPos(0.0f, 0.0f);
					Vector2D pos(x,y);
					GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
					if ((lastPos.getX() != pos.getX() || lastPos.getY() != pos.getY()) && pGame->getGrid()->getValueAtPixelXY(x, y) != BLOCKING_VALUE)
					{
						EventSystem::getInstance()->fireEvent(PathToMessage(lastPos, pos));
						lastPos = pos;
					}

					mKeyEvent.setType(MOUSE_LEFT);
					EventSystem::getInstance()->fireEvent(mKeyEvent);
				}
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
				//cout << "InputSystem: D Key" << endl;
				mKeyEvent.setType(D_KEY);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_s)
			{
				//cout << "InputSystem: S Key" << endl;
				mKeyEvent.setType(S_KEY);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_a)
			{
				//cout << "InputSystem: A Key" << endl;
				mKeyEvent.setType(A_KEY);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_w)
			{
				//cout << "InputSystem: W Key" << endl;
				mKeyEvent.setType(W_KEY);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_UP)
			{
				//cout << "InputSystem: Up arrow" << endl;
				mKeyEvent.setType(UP_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_DOWN)
			{
				//cout << "InputSystem: Down arrow" << endl;
				mKeyEvent.setType(DOWN_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_LEFT)
			{
				//cout << "InputSystem: Left arrow" << endl;
				mKeyEvent.setType(LEFT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}
			if (mEvent.key.keysym.sym == SDLK_RIGHT)
			{
				//cout << "InputSystem: Right arrow" << endl;
				mKeyEvent.setType(RIGHT_ARROW);
				EventSystem::getInstance()->fireEvent(mKeyEvent);
			}

			break;
		}
	};
}


void InputSystem::getMousePos(int &x, int &y)
{
	SDL_GetMouseState(&x, &y);
}
