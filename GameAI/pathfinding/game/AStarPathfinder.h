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
class AStarPathfinder :public GridPathfinder
{
public:
	AStarPathfinder(Graph* pGraph);
	~AStarPathfinder();

	Path* findPath(Node* pFrom, Node* pTo);//make sure to delete the path when you are done!

	NodeRecordA smallestElement(std::vector<NodeRecordA> listToCheck);
	NodeRecordA findNode(Node* nodeToCheck, std::vector<NodeRecordA> listToCheck);
	bool contains(Node* nodeToCheck, std::vector<NodeRecordA> listToCheck);
	float heuristic(Node* nodeToCheck, Node* goalNode);
	void removeFromList(NodeRecordA nodeToRemove, std::vector<NodeRecordA> listToUse);

private:

};