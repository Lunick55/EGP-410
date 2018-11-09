#pragma once

#include "GridPathfinder.h"
#include <vector>
#include <list>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;

using namespace std;

/*
Used to keep track of individual node values on the graph
*/
struct NodeRecordA : public Trackable
{
	NodeRecordA() { mpConnection = nullptr; mCostSoFar = 0; mpNode = nullptr; };

	Connection* mpConnection;

	float mCostSoFar;
	float mEstimatedTotalCost;
	Node* mpNode;
};

/*
Finds a path from a start to an end point using the A* pathfinding algorithm
*/
class PathSmoothing :public GridPathfinder
{
public:
	PathSmoothing(Graph* pGraph);
	~PathSmoothing();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

private:

};