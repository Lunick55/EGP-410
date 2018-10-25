#pragma once

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;
class Connection;

using namespace std;

/*
Used to keep track of individual node values on the graph
*/
struct NodeRecord : public Trackable
{
	NodeRecord(){mpConnection = nullptr; mCostSoFar = 0; mpNode = nullptr;};

	Connection* mpConnection;

	float mCostSoFar;
	Node* mpNode;
};

/*
Finds a path from a start to an end point using Dijkstra's pathfinding algorithm
*/
class DijkstraPathfinder :public GridPathfinder
{
public:
	DijkstraPathfinder(Graph* pGraph);
	~DijkstraPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

	NodeRecord smallestElement(std::vector<NodeRecord> listToCheck);
	NodeRecord findNode(Node* nodeToCheck, std::vector<NodeRecord> listToCheck);
	bool contains(Node* nodeToCheck, std::vector<NodeRecord> listToCheck);

private:

};