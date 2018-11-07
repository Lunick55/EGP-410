#include "AStarPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "Grid.h"
#include "GridGraph.h"
#include "Game.h"
#include "GameApp.h"
#include "Vector2D.h"
#include <PerformanceTracker.h>
#include <list>
//#include <vector>
#include <algorithm>

using namespace std;

AStarPathfinder::AStarPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

AStarPathfinder::~AStarPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* AStarPathfinder::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	//create Path
	Path* pPath = new Path();

	NodeRecordA currentNodeRec;
	bool toNodeAdded = false;

	//initialize record for start node
	NodeRecordA startRecord;
	startRecord.mpNode = pFrom;
	startRecord.mpConnection = NULL;
	startRecord.mCostSoFar = 0;
	startRecord.mEstimatedTotalCost = heuristic(startRecord.mpNode, pTo);

	//initialize open and close lists
	//allocate nodes to visit list and place starting node in it
	list<NodeRecordA> openList;
	//openList.insert(openList.begin(), startRecord);
	openList.push_front(startRecord);
	list<NodeRecordA> closedList;

	///I assume this can stay
#ifdef VISUALIZE_PATH
	delete mpPath;
	closedList.clear();
	mVisitedNodes.clear();
	closedList.push_back(startRecord);
	mVisitedNodes.push_back(pFrom);
#endif

	while (openList.size() > 0)
	{
		//find smallest element in openList
		currentNodeRec = smallestElement(openList);
		

		//if it is the goal node, then we're done here
		if (currentNodeRec.mpNode == pTo)
		{
			break;
		}

		//otherwise get its outgoing connections
		std::vector<Connection*> connections;
		connections = mpGraph->getConnections(currentNodeRec.mpNode->getId());

		//loop through the connections
		for (int i = 0; i < connections.size(); i++)
		{
			NodeRecordA endNodeRec;
			Node* endNode = connections[i]->getToNode();
			float endNodeCost = currentNodeRec.mCostSoFar + connections[i]->getCost();
			float endNodeHeuristic = 0;

			//if the node is closed, check for a better path
			if (contains(endNode, closedList))
			{
				endNodeRec = findNode(endNode, closedList);
				
				if (endNodeRec.mCostSoFar <= endNodeCost)
				{
					continue;
				}

				//if we find a better path, take node out of the closed list
				removeFromList(endNodeRec, closedList);

				///This could be calculated without calling the heuristic
				endNodeHeuristic = heuristic(endNodeRec.mpNode, pTo);
				
			}
			else if (contains(endNode, openList))
			{
				endNodeRec = findNode(endNode, openList);

				if (endNodeRec.mCostSoFar <= endNodeCost)
				{
					continue;
				}

				///This could be calculated without calling the heuristic
				endNodeHeuristic = heuristic(endNodeRec.mpNode, pTo);				
			}
			else //this is an all new node!
			{
				endNodeRec = NodeRecordA();
				endNodeRec.mpNode = endNode;

				endNodeHeuristic = heuristic(endNodeRec.mpNode, pTo);
			}

			endNodeRec.mCostSoFar = endNodeCost;
			endNodeRec.mpConnection = connections[i];
			endNodeRec.mEstimatedTotalCost = endNodeCost + endNodeHeuristic;

			if (!contains(endNode, openList))
			{
				//openList.insert(openList.begin(), endNodeRec);
				openList.push_front(endNodeRec);
			}
		}

		//remove current from openList
		for (list<NodeRecordA>::iterator iter = openList.begin(); iter != openList.end(); iter++)
		{
			if (iter->mpNode == currentNodeRec.mpNode && iter->mpConnection == currentNodeRec.mpConnection && iter->mCostSoFar == currentNodeRec.mCostSoFar)
			{
				openList.erase(iter);
				break;
			}
		}
		//removeFromList(currentNodeRec, openList);

		//add current to closed list
		closedList.push_front(currentNodeRec);

#ifdef VISUALIZE_PATH
		mVisitedNodes.push_back(currentNodeRec.mpNode);
#endif
	}

	if (currentNodeRec.mpNode != pTo)
	{
		return NULL;
	}
	else
	{
		//compile list into path
		Path* tempPath = new Path;

		tempPath->addNode(currentNodeRec.mpNode);

		//work back along the path
		//reverse path and return it
		while (currentNodeRec.mpNode != pFrom)
		{
			currentNodeRec = findNode(currentNodeRec.mpConnection->getFromNode(), closedList);
			tempPath->addNode(currentNodeRec.mpNode);
		}

		for (int i = tempPath->getNumNodes() - 1; i > -1; i--)
		{
			pPath->addNode(tempPath->peekNode(i));
		}

		delete tempPath;
	}

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mpPath = pPath;
#endif

	return pPath;
}

void AStarPathfinder::removeFromList(NodeRecordA nodeToRemove, std::list<NodeRecordA> listToCheck)
{
	for (list<NodeRecordA>::iterator iter = listToCheck.begin(); iter != listToCheck.end(); iter++)
	{
		if (iter->mpNode == nodeToRemove.mpNode && iter->mpConnection == nodeToRemove.mpConnection && iter->mCostSoFar == nodeToRemove.mCostSoFar)
		{
			listToCheck.erase(iter);
			break;
		}
	}
}

NodeRecordA AStarPathfinder::smallestElement(list<NodeRecordA> listToCheck)
{
	//returns smallest Nodes costSoFar
	NodeRecordA smallestNode;
	smallestNode = listToCheck.front();

	for (list<NodeRecordA>::iterator iter = listToCheck.begin(); iter != listToCheck.end(); iter++)
	{
		if (iter->mEstimatedTotalCost < smallestNode.mEstimatedTotalCost)
		{
			smallestNode = (*iter);
		}
	}
	return smallestNode;
}

NodeRecordA AStarPathfinder::findNode(Node* nodeToCheck, list<NodeRecordA> listToCheck)
{
	for (list<NodeRecordA>::iterator iter = listToCheck.begin(); iter != listToCheck.end(); iter++)
	{
		if (nodeToCheck == iter->mpNode)
		{
			return (*iter);
		}
	}
}

bool AStarPathfinder::contains(Node* nodeToCheck, list<NodeRecordA> listToCheck)
{
	for (list<NodeRecordA>::iterator iter = listToCheck.begin(); iter != listToCheck.end(); iter++)
	{
		if (nodeToCheck == iter->mpNode)
		{
			return true;
		}
	}

	return false;
}

float AStarPathfinder::heuristic(Node* nodeToCheck, Node* goalNode)
{
	Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();

	Vector2D diff = pGrid->getULCornerOfSquare(goalNode->getId()) - pGrid->getULCornerOfSquare(nodeToCheck->getId());

	return diff.getLengthSquared();
}
