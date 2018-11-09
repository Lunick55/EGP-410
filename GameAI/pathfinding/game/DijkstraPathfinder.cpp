#include "DijkstraPathfinder.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "Game.h"
#include <PerformanceTracker.h>
#include <list>
#include <queue>
//#include <vector>
#include <algorithm>

using namespace std;


DijkstraPathfinder::DijkstraPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

DijkstraPathfinder::~DijkstraPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* DijkstraPathfinder::findPath(Node* pFrom, Node* pTo)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");

	//create Path
	Path* pPath = new Path();

	NodeRecord currentNodeRec;
	bool toNodeAdded = false;

	//initialize record for start node
	NodeRecord startRecord;
	startRecord.mpNode = pFrom;
	startRecord.mpConnection = NULL;
	startRecord.mCostSoFar = 0;

	//initialize open and close lists
	//allocate nodes to visit list and place starting node in it

	//Karol use this boi: priority_queue <NodeRecord> openList;
	vector<NodeRecord> openList;
	//use push_front() boiiiiiiidfhjsdfhjksfhd
	openList.insert(openList.begin(),startRecord);
	vector<NodeRecord> closedList;

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
		//use .front()
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
			NodeRecord endNodeRec;
			Node* endNode = connections[i]->getToNode();
			float endNodeCost = currentNodeRec.mCostSoFar + connections[i]->getCost();

			//skip if node is closed
			if (contains(endNode, closedList))
			{
				continue;
			}
			else if (contains(endNode, openList))
			{
				endNodeRec = findNode(endNode, openList);

				if (endNodeRec.mCostSoFar <= endNodeCost)
				{
					continue;
				}
			}
			else
			{
				endNodeRec = NodeRecord();
				endNodeRec.mpNode = endNode;
			}

			endNodeRec.mCostSoFar = endNodeCost;
			endNodeRec.mpConnection = connections[i];

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
		Path* tempPath = new Path;

		tempPath->addNode(currentNodeRec.mpNode);

		//work back along the path
		//reverse path and return it
		while (currentNodeRec.mpNode != pFrom)
		{
			currentNodeRec = findNode(currentNodeRec.mpConnection->getFromNode(), closedList);
			tempPath->addNode(currentNodeRec.mpNode);
		}	

		for (int i = tempPath->getNumNodes()-1; i > -1; i--)
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

//change this to priority_queue
NodeRecord DijkstraPathfinder::smallestElement(vector<NodeRecord> listToCheck)
{
	//returns smallest Nodes costSoFar
	NodeRecord smallestNode;
	smallestNode = listToCheck.front();

	for (int i = 0; i < listToCheck.size(); i++)
	{
		if (listToCheck[i].mCostSoFar < smallestNode.mCostSoFar)
		{
			smallestNode = listToCheck[i];
		}
	}	

	return smallestNode;
}

//change this to priority_queueu
NodeRecord DijkstraPathfinder::findNode(Node* nodeToCheck, vector<NodeRecord> listToCheck)
{
	for (int i = 0; i < listToCheck.size(); i++)
	{
		if (nodeToCheck == listToCheck[i].mpNode)
		{
			return listToCheck[i];
		}
	}
}

bool DijkstraPathfinder::contains(Node* nodeToCheck, vector<NodeRecord> listToCheck)
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