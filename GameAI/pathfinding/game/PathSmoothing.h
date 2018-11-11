#pragma once

#include "GridPathfinder.h"
#include "Path.h"
#include <vector>
#include <list>

using namespace std;

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