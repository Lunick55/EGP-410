#include "PathSmoothing.h"
#include "GridGraph.h"
#include "GameApp.h"
#include "Grid.h"
#include "Vector2D.h"
PathSmoothing::PathSmoothing(Graph * pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
	mPath = new Path;
}
PathSmoothing::~PathSmoothing()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
	delete mPath;
}

Path * PathSmoothing::findPath(Node * pFrom, Node * pTo)
{
	return nullptr;
}

Path* PathSmoothing::smoothPath(Path * inputPath)
{
	int inputIndex = 2;

	if (inputPath->getNumNodes() == 2)
	{
		return inputPath;
	}

	mPath->addNode(inputPath->peekNode(0));

	while (inputIndex < inputPath->getNumNodes() - 1)
	{
		if (!rayClear(mPath->peekNode(mPath->getNumNodes() - 1), inputPath->peekNode(inputIndex)))
		{
			mPath->addNode(inputPath->peekNode(inputIndex - 1));
		}
		inputIndex++;

	}
	mPath->addNode(inputPath->peekNode(inputPath->getNumNodes() - 1));
	return mPath;

}

bool PathSmoothing::rayClear(Node * outputPath, Node * inputPath)
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	
	if (pGame != NULL)
	{
		Grid *pGrid = pGame->getGrid();

		Vector2D from = pGrid->getULCornerOfSquare(outputPath->getId());
		Vector2D to = pGrid->getULCornerOfSquare(inputPath->getId());

		Vector2D diff;

		diff = to - from;

		int diffY = (int)diff.getY();
		int diffX = (int)diff.getX();
		

		int distance = sqrt((diffX * diffX) + (diffY * diffY));
		int amountChecks = 50;

		int posX = (int)from.getX();
		int posY = (int)from.getY();

		for (int i = 0; i < amountChecks; i++)
		{
			if (pGame->getGrid()->getValueAtPixelXY((int)posX, (int)posY) == BLOCKING_VALUE)
			{
				return false;
			}

			posX = posX + (diffX / amountChecks);
			posY = posY + (diffY / amountChecks);
		}
		return true;
	}
	return false;
}
