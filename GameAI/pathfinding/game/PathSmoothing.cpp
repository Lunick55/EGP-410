#include "PathSmoothing.h"
#include "GridGraph.h"
PathSmoothing::PathSmoothing(Graph * pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif
}
PathSmoothing::~PathSmoothing()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path * PathSmoothing::findPath(Node * pFrom, Node * pTo)
{
	return nullptr;
}
