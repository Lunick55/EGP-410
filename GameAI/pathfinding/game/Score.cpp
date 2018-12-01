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
	string toDisplay = "Score: " + to_string(mScore);
	gpGame->getGraphicsSystem()->writeText(*pBuffer, *(gpGame->getFont()), mPos.getX(), mPos.getY(), toDisplay, WHITE_COLOR);
}
