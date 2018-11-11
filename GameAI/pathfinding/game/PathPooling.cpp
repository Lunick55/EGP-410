#include "GridGraph.h"
#include "PathPooling.h"
#include "Path.h"

PathPooling::PathPooling()
{

}

PathPooling::~PathPooling()
{

}

void PathPooling::storePath(Node* startNode, Node* endNode, Path* path)
{	
	//make an iterator to go through the path
	map<pair<NODE_ID, NODE_ID>, Path>::iterator it = pathRecord.find(make_pair(startNode->getId(), endNode->getId()));

	//as long as it isnt already in the path
	if (it == pathRecord.end())
	{ 
		//store it inside of the map
		pathRecord[make_pair(startNode->getId(), endNode->getId())] = (*path);
	}

}

Path* PathPooling::getPath(Node* startNode, Node* endNode)
{
	//iterate through the map to try and find the path as long as it isnt equal to the end of it
	if (pathRecord.find(make_pair(startNode->getId(), endNode->getId())) != pathRecord.end())
	{
		//return the path from the map
		return &pathRecord[make_pair(startNode->getId(), endNode->getId())];
	}
	//otherwise return a nullptr meaning it isnt in the map
	return nullptr;
}


