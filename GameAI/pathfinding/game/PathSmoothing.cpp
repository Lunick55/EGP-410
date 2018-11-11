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

	//if the path is only two nodes long, that means we cant smooth it
	if (inputPath->getNumNodes() == 2)
	{
		return inputPath;
	}

	//compile an output path
	mPath->addNode(inputPath->peekNode(0));

	//loop until we find the last item in the input
	while (inputIndex < inputPath->getNumNodes() - 1)
	{
		//do the raycast thingy
		if (!rayClear(mPath->peekNode(mPath->getNumNodes() - 1), inputPath->peekNode(inputIndex)))
		{
			//the ray text failed, add the last node that passed to the output list
			mPath->addNode(inputPath->peekNode(inputIndex - 1));
		}
		//consider the next node
		inputIndex++;

	}
	//we've reached the end of the input path, add the end node to the output and return it
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

		//get the direction from the input path and the output path
		diff = to - from;

		int diffY = (int)diff.getY();
		int diffX = (int)diff.getX();
		
		//gets the number of checks between input/output path
		int distance = sqrt((diffX * diffX) + (diffY * diffY));
		int amountChecks = 50;

		int posX = (int)from.getX();
		int posY = (int)from.getY();

		//continues in between each node in order to see if there is a wall
		for (int i = 0; i < amountChecks; i++)
		{
			//if there is a wall
			if (pGame->getGrid()->getValueAtPixelXY((int)posX, (int)posY) == BLOCKING_VALUE)
			{
				return false;
			}
			//update the position
			posX = posX + (diffX / amountChecks);
			posY = posY + (diffY / amountChecks);
		}
		//we have found that there is no wall!
		return true;
	}
	return false;
}
