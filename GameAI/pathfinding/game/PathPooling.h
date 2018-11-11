#pragma once

#include "GridPathfinder.h"
#include <vector>
#include <list>

using namespace std;

/*
Saves refrencrs to used paths
*/
class PathPooling :public Trackable
{
public:
	PathPooling();
	~PathPooling();

	void storePath(Node* startNode, Node* endNode, Path* path);
	Path* getPath(Node* startNode, Node* endNode);

private:
	map<std::pair<NODE_ID, NODE_ID> , Path> pathRecord;
};


