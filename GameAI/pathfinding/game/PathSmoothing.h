#pragma once

#include "GridPathfinder.h"
#include "Path.h"
#include <vector>
#include <list>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;

using namespace std;


/*
Finds a path from a start to an end point using the A* pathfinding algorithm
*/
class PathSmoothing :public GridPathfinder
{
public:
	PathSmoothing(Graph* pGraph);
	~PathSmoothing();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

	Path* smoothPath(Path* inputPath);

	bool rayClear(Node* outputPath, Node* inputPath);

private:
	Path *mPath;
};