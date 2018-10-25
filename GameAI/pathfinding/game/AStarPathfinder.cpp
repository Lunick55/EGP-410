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
	vector<NodeRecordA> openList;
	openList.insert(openList.begin(), startRecord);
	vector<NodeRecordA> closedList;

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
				openList.insert(openList.begin(), endNodeRec);
			}
		}

		//remove current from openList
		for (int i = 0; i < openList.size(); i++)
		{
			if (openList[i].mpNode == currentNodeRec.mpNode && openList[i].mpConnection == currentNodeRec.mpConnection && openList[i].mCostSoFar == currentNodeRec.mCostSoFar)
			{
				openList.erase(openList.begin() + i);
			}
		}
		//add current to closed list
		closedList.push_back(currentNodeRec);

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
		pPath->addNode(currentNodeRec.mpNode);

		//work back along the path
		//reverse path and return it
		while (currentNodeRec.mpNode != pFrom)
		{
			currentNodeRec = findNode(currentNodeRec.mpConnection->getFromNode(), closedList);
			pPath->addNode(currentNodeRec.mpNode);
		}
	}

	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mpPath = pPath;
#endif

	return pPath;
}

void AStarPathfinder::removeFromList(NodeRecordA nodeToRemove, std::vector<NodeRecordA> listToUse)
{
	for (int i = 0; i < listToUse.size(); i++)
	{
		if (listToUse[i].mpNode == nodeToRemove.mpNode && listToUse[i].mpConnection == nodeToRemove.mpConnection && listToUse[i].mCostSoFar == nodeToRemove.mCostSoFar)
		{
			listToUse.erase(listToUse.begin() + i);
		}
	}
}

NodeRecordA AStarPathfinder::smallestElement(vector<NodeRecordA> listToCheck)
{
	//returns smallest Nodes costSoFar
	NodeRecordA smallestNode;
	smallestNode = listToCheck.front();

	for (int i = 0; i < listToCheck.size(); i++)
	{
		if (listToCheck[i].mEstimatedTotalCost < smallestNode.mEstimatedTotalCost)
		{
			smallestNode = listToCheck[i];
		}
	}

	return smallestNode;
}

NodeRecordA AStarPathfinder::findNode(Node* nodeToCheck, vector<NodeRecordA> listToCheck)
{
	for (int i = 0; i < listToCheck.size(); i++)
	{
		if (nodeToCheck == listToCheck[i].mpNode)
		{
			return listToCheck[i];
		}
	}
}

bool AStarPathfinder::contains(Node* nodeToCheck, vector<NodeRecordA> listToCheck)
{
	for (int i = 0; i < listToCheck.size(); i++)
	{
		if (nodeToCheck == listToCheck[i].mpNode)
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
