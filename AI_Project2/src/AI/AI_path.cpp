#include "AI_path.h"
#include <iostream>
#include <assert.h>

void PrintSet(const std::string& name, const std::vector<PathNode*>& set)
{
	printf("%s { ", name.c_str());

	for (int i = 0; i < set.size(); i++)
	{
		printf("%c ", set[i]->name);
	}

	printf("}\n");
}

std::vector<PathNode*>::iterator GetPathNodeWithShortestDistanceFromStart(std::vector<PathNode*>& set)
{
	std::vector<PathNode*>::iterator findIt;
	std::vector<PathNode*>::iterator shortestIt;
	float shortestDistance = FLT_MAX;
	for (findIt = set.begin(); findIt != set.end(); ++findIt)
	{
		if ((*findIt)->distanceFromStart < shortestDistance)
		{
			shortestDistance = (*findIt)->distanceFromStart;
			shortestIt = findIt;
		}
	}

	return shortestIt;
}

AI_path::AI_path()
{
}

AI_path::~AI_path()
{
}

void AI_path::loadBITMAP(std::string fileName)
{
	m_theMap = new cAI_Map(fileName);

}

void AI_path::createGraph(cVAOManager* pVAOManager)
{
	for (int row = 0; row < m_theMap->numRows; row++)
	{
		for (int col = 0; col < m_theMap->numColumns; col++)
		{
			C24BitBMPpixel pixel = m_theMap->getPixelAtRowColumn(row, col);
			std::string instantname = "f_r" + std::to_string(row) + "c"+std::to_string(col);
			glm::vec3 pos = glm::vec3((col - int(m_theMap->numColumns/2)) * 5, 0, (row - int(m_theMap->numRows/2)) * 5);
			if (pixel.redPixel == 255 && pixel.greenPixel == 255 && pixel.bluePixel == 255)
			{
				pVAOManager->createNewMeshOBJ(instantname, "floorA", pos, glm::vec3(0.f), glm::vec3(0.01));
				createPathNode(pos, instantname);
			}
			if (pixel.redPixel == 0 && pixel.greenPixel == 0 && pixel.bluePixel == 0)
			{
				pVAOManager->createNewMeshOBJ(instantname, "floorA", pos, glm::vec3(0.f), glm::vec3(0.01, 1, 0.01));
			}
			if (pixel.redPixel == 255 && pixel.greenPixel == 0 && pixel.bluePixel == 0)
			{
				pVAOManager->createNewMeshOBJ(instantname, "floorA", pos, glm::vec3(0.f), glm::vec3(0.01));
				m_EndNode = createPathNode(pos, instantname);
			}
			if (pixel.redPixel == 0 && pixel.greenPixel == 255 && pixel.bluePixel == 0)
			{
				pVAOManager->createNewMeshOBJ(instantname, "floorA", pos, glm::vec3(0.f), glm::vec3(0.01));
				m_StartNode = createPathNode(pos, instantname);
			}
		}
	}

	checkAndCreateNeighbour();
}

PathNode* AI_path::createPathNode(const glm::vec3& pos, std::string name)
{
	PathNode* newPathNode = new PathNode(name);
	newPathNode->point = pos;
	newPathNode->Id = m_Graph.nodes.size();
	m_Graph.nodes.push_back(newPathNode);

	return newPathNode;
}

void AI_path::checkAndCreateNeighbour()
{
	for (int row = 0; row < m_theMap->numRows; row++)
	{
		for (int col = 0; col < m_theMap->numColumns; col++)
		{
			std::string Center		= "f_r" + std::to_string(row)   + "c" + std::to_string(col);
			PathNode* centerNode = m_Graph.getNodeByName(Center);
			if (centerNode == nullptr)
			{
				continue;
			}

			std::string North		= "f_r" + std::to_string(row-1) + "c" + std::to_string(col);
			PathNode* N_node = m_Graph.getNodeByName(North);
			if (N_node != nullptr)
			{
				bool found = false;
				for (int i = 0; i < centerNode->neighbours.size(); i++)
				{
					if (centerNode->neighbours[i]->Id == N_node->Id)
					{
						found = true;
					}
				}
				if (!found)
				{
					m_Graph.SetNeighbours(centerNode->Id, N_node->Id);
				}

			}

			std::string East		= "f_r" + std::to_string(row)   + "c" + std::to_string(col+1);
			PathNode* E_node = m_Graph.getNodeByName(East);
			if (E_node != nullptr)
			{
				bool found = false;
				for (int i = 0; i < centerNode->neighbours.size(); i++)
				{
					if (centerNode->neighbours[i]->Id == E_node->Id)
					{
						found = true;
					}
				}
				if (!found)
				{
					m_Graph.SetNeighbours(centerNode->Id, E_node->Id);
				}
			}


			std::string West		= "f_r" + std::to_string(row)   + "c" + std::to_string(col-1);
			PathNode* W_node = m_Graph.getNodeByName(West);
			if (W_node != nullptr)
			{
				bool found = false;
				for (int i = 0; i < centerNode->neighbours.size(); i++)
				{
					if (centerNode->neighbours[i]->Id == W_node->Id)
					{
						found = true;
					}
				}
				if (!found)
				{
					m_Graph.SetNeighbours(centerNode->Id, W_node->Id);
				}
			}

			std::string South		= "f_r" + std::to_string(row+1) + "c" + std::to_string(col);
			PathNode* S_node = m_Graph.getNodeByName(South);
			if (S_node != nullptr)
			{
				bool found = false;
				for (int i = 0; i < centerNode->neighbours.size(); i++)
				{
					if (centerNode->neighbours[i]->Id == S_node->Id)
					{
						found = true;
					}
				}
				if (!found)
				{
					m_Graph.SetNeighbours(centerNode->Id, S_node->Id);
				}
			}

			if (N_node != nullptr && E_node != nullptr)
			{
				std::string NorthEast = "f_r" + std::to_string(row - 1) + "c" + std::to_string(col + 1);
				PathNode* NE_node = m_Graph.getNodeByName(NorthEast);
				if (NE_node != nullptr)
				{
					bool found = false;
					for (int i = 0; i < centerNode->neighbours.size(); i++)
					{
						if (centerNode->neighbours[i]->Id == NE_node->Id)
						{
							found = true;
						}
					}
					if (!found)
					{
						m_Graph.SetNeighbours(centerNode->Id, NE_node->Id);
					}
				}
			}

			if (E_node != nullptr && S_node != nullptr)
			{
				std::string SouthEast	= "f_r" + std::to_string(row+1) + "c" + std::to_string(col+1);
				PathNode* SE_node = m_Graph.getNodeByName(SouthEast);
				if (SE_node != nullptr)
				{
					bool found = false;
					for (int i = 0; i < centerNode->neighbours.size(); i++)
					{
						if (centerNode->neighbours[i]->Id == SE_node->Id)
						{
							found = true;
						}
					}
					if (!found)
					{
						m_Graph.SetNeighbours(centerNode->Id, SE_node->Id);
					}
				}
			}

			if (S_node != nullptr && W_node != nullptr)
			{
				std::string SouthWest	= "f_r" + std::to_string(row+1) + "c" + std::to_string(col-1);
				PathNode* SW_node = m_Graph.getNodeByName(SouthWest);
				if (SW_node != nullptr)
				{
					bool found = false;
					for (int i = 0; i < centerNode->neighbours.size(); i++)
					{
						if (centerNode->neighbours[i]->Id == SW_node->Id)
						{
							found = true;
						}
					}
					if (!found)
					{
						m_Graph.SetNeighbours(centerNode->Id, SW_node->Id);
					}
				}
			}

			if (W_node != nullptr && N_node != nullptr)
			{
				std::string NorthWest = "f_r" + std::to_string(row - 1) + "c" + std::to_string(col - 1);
				PathNode* NW_node = m_Graph.getNodeByName(NorthWest);
				if (NW_node != nullptr)
				{
					bool found = false;
					for (int i = 0; i < centerNode->neighbours.size(); i++)
					{
						if (centerNode->neighbours[i]->Id == NW_node->Id)
						{
							found = true;
						}
					}
					if (!found)
					{
						m_Graph.SetNeighbours(centerNode->Id, NW_node->Id);
					}
				}
			}
		}
	}
}

bool AI_path::BreadthFirstSearch(Graph* graph, PathNode* start, PathNode* end)
{
	std::vector<PathNode*> open;
	std::vector<PathNode*> closed;
	open.push_back(start);
	printf("Added %c to the open set!\n", start->name);
	while (!open.empty())
	{
		
		PrintSet("open:   ", open);
		PrintSet("closed: ", closed);

		PathNode* X = open.back();
		open.pop_back();
		printf("Removed %c from the open set!\n", X->name);

		if (X == end)
			return true;

		closed.push_back(X);
		printf("Added %c to the closed set!\n", X->name);

		printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
		for (int i = 0; i < X->neighbours.size(); i++)
		{
			PathNode* n = X->neighbours[i];
			printf("Found neighbour %c!\n", n->name);
			if (std::find(open.begin(), open.end(), X->neighbours[i]) == open.end()
				&& std::find(closed.begin(), closed.end(), X->neighbours[i]) == closed.end())
			{
				open.insert(open.begin(), n);
				n->parent = X;
				printf("Added %c to the open set!\n", n->name);
			}
		}
	}

	return false;
}

bool AI_path::DepthFirstSearch(Graph* graph, PathNode* start, PathNode* end)
{
	std::vector<PathNode*> open;
	std::vector<PathNode*> closed;

	open.push_back(start);
	printf("Added %c to the open set!\n", start->name);
	while (!open.empty())
	{
		PrintSet("open:   ", open);
		PrintSet("closed: ", closed);

		std::vector<PathNode*>::iterator itX = GetPathNodeWithShortestDistanceFromStart(open);
		PathNode* X = *itX;
		open.erase(itX);

		printf("Removed %c from the open set!\n", X->name);

		if (X == end)
			return true;

		closed.push_back(X);
		printf("Added %c to the closed set!\n", X->name);

		printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
		for (int i = 0; i < X->neighbours.size(); i++)
		{
			PathNode* n = X->neighbours[i];
			printf("Found neighbour %c!\n", n->name);
			if (std::find(closed.begin(), closed.end(), n) == closed.end())
			{
				continue;
			}
			else if (std::find(open.begin(), open.end(), n) != open.end())
			{
				// Distance from n to it's parent + distance from parent to start
				float distanceFromStartToN = glm::distance(n->point, X->point) + X->distanceFromStart;
				if (distanceFromStartToN < n->distanceFromStart)
				{
					n->parent = X;
					n->distanceFromStart = distanceFromStartToN;
				}
			}
			else
			{
				open.insert(open.begin(), n);
				n->parent = X;
				printf("Added %c to the open set!\n", n->name);
			}
		}
	}
}

bool AI_path::DijkstraSearch(Graph* graph, PathNode* start, PathNode* end)
{
	std::vector<PathNode*> open;
	std::vector<PathNode*> closed;

	open.push_back(start);
	printf("Added %c to the open set!\n", start->name);
	while (!open.empty())
	{
		PrintSet("open:   ", open);
		PrintSet("closed: ", closed);

		std::vector<PathNode*>::iterator itX = GetPathNodeWithShortestDistanceFromStart(open);
		PathNode* X = *itX;
		open.erase(itX);

		printf("Removed %c from the open set!\n", X->name);

		if (X == end)
			return true;

		closed.push_back(X);
		printf("Added %c to the closed set!\n", X->name);

		printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
		for (int i = 0; i < X->neighbours.size(); i++)
		{
			PathNode* n = X->neighbours[i];
			printf("Found neighbour %c!\n", n->name);
			if (std::find(closed.begin(), closed.end(), n) == closed.end())
			{
				continue;
			}
			else if (std::find(open.begin(), open.end(), n) != open.end())
			{
				// Distance from n to it's parent + distance from parent to start
				float distanceFromStartToN = glm::distance(n->point, X->point) + X->distanceFromStart;
				if (distanceFromStartToN < n->distanceFromStart)
				{
					n->parent = X;
					n->distanceFromStart = distanceFromStartToN;
				}
			}
			else
			{
				open.insert(open.begin(), n);
				n->parent = X;
				printf("Added %c to the open set!\n", n->name);
			}
		}
	}
}

bool AI_path::AStarSearch(Graph* graph, PathNode* start, PathNode* end)
{
	//todo
	return false;
}

void Graph::SetNeighbours(int a, int b)
{
	
	assert(a >= 0);
	assert(b >= 0);
	assert(a < nodes.size());
	assert(b < nodes.size());

	nodes[a]->neighbours.push_back(nodes[b]);
	nodes[b]->neighbours.push_back(nodes[a]);
}

PathNode* Graph::getNodeByName(std::string name)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->name == name)
		{
			return nodes[i];
		}
	}
	return nullptr;
}