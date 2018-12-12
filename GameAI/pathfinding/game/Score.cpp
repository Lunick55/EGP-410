#include "Score.h"
#include "GraphicsBuffer.h"
#include "Game.h"
#include "GraphicsSystem.h"
#include "Color.h"
#include <string>

using namespace std;

Score::Score(const Vector2D & pos)
	:mPos(pos)
{
	mScore = 0;
}

Score::~Score()
{
}

void Score::addToScore(int addScore)
{
	mScore += addScore;
}

int Score::getCurrentScore()
{
	return mScore;
}

void Score::draw(GraphicsBuffer * pBuffer)
{
	mBuffer = pBuffer;
	string toDisplay = "Score: " + to_string(mScore);
	gpGame->getGraphicsSystem()->writeText(*mBuffer, *(gpGame->getFont()), mPos.getX(), mPos.getY(), toDisplay, WHITE_COLOR);
}

void Score::draw()
{
	string toDisplay = "Sorry you lose";
	gpGame->getGraphicsSystem()->writeText(*mBuffer, *(gpGame->getFont()), 32, 68, toDisplay, WHITE_COLOR);
	string toDisplay2 = "your score is: " + to_string(mScore);
	gpGame->getGraphicsSystem()->writeText(*mBuffer, *(gpGame->getFont()), 32, 100, toDisplay2, WHITE_COLOR);
	string toDisplay3 = "Press esc to exit!";
	gpGame->getGraphicsSystem()->writeText(*mBuffer, *(gpGame->getFont()), 32, 300, toDisplay3, WHITE_COLOR);
}
