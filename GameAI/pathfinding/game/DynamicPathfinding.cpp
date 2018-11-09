#include "DynamicPathfinding.h"
#include "GridGraph.h"

DynamicPathfinding::DynamicPathfinding(Graph * pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}

DynamicPathfinding::~DynamicPathfinding()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path * DynamicPathfinding::findPath(Node * pFrom, Node * pTo)
{
	return nullptr;
}
