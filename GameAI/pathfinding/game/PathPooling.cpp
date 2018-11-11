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
	map<pair<NODE_ID, NODE_ID>, Path>::iterator it = pathRecord.find(make_pair(startNode->getId(), endNode->getId()));

	if (it == pathRecord.end())
	{ 
		//return;
		pathRecord[make_pair(startNode->getId(), endNode->getId())] = (*path);
	}
	//pathRecord[make_pair(startNode->getId(), endNode->getId())] = path;

}

Path* PathPooling::getPath(Node* startNode, Node* endNode)
{
	if (pathRecord.find(make_pair(startNode->getId(), endNode->getId())) != pathRecord.end())
	{
		return &pathRecord[make_pair(startNode->getId(), endNode->getId())];
	}
	return nullptr;
}

//bool operator==(const Node& lhs, const Node& rhs)
//{
//	if (lhs.getId() == rhs.getId())
//	{
//		return true;
//	}
//	return false;
//}

