#include <AIGame/PathFinding/PathFinder.hpp>
#include <AIGame/Map/Map.hpp>
#include <AIGame/Macros.hpp>
#include <Misc/VectorMaths.hpp>
#include <set>

// Classes for pathing
struct PathNode {
	PathNode() {
		G = 0;
		H = 0;
		open = false;
		closed = false;
	}

	sf::Vector2i parent;
	float G;
	float H;
	bool open;
	bool closed;
};

// A large grid of nodes.
const int nodegridsize = 100;
PathNode nodegrid[nodegridsize * nodegridsize];

// Get items
PathNode* GetGrid(int _x, int _y, int _width) {
	return &nodegrid[_x + _y * _width];
}
PathNode* GetGrid(const sf::Vector2i& _pos, int _width) {
	return GetGrid(_pos.x, _pos.y, _width);
}

class PriorityQueue {
public:
	// Classes
	struct PriorityQueueItem {
		sf::Vector2i pos;
		float priority;

		PriorityQueueItem(const sf::Vector2i& _pos, float _priority) {
			pos = _pos;
			priority = _priority;
		}

		class PriorityQueueComparator {
		public:
			bool operator() (const PriorityQueueItem& _a, const PriorityQueueItem& _b) const {
				return _a.priority < _b.priority;
			}
		};
	};

	// Constructors
	PriorityQueue() {}
	virtual ~PriorityQueue() {}

	// Push
	void Push(const sf::Vector2i& _pos, float _priority) {
		// Add to the list
		list.insert(PriorityQueueItem(_pos, _priority));
	}

	// Pop
	sf::Vector2i Pop() {
		sf::Vector2i node = list.begin()->pos;
		list.erase(list.begin());
		return node;
	}

	void Remove(const sf::Vector2i& _pos) {
		for (ContainerType::iterator it = list.begin(); it != list.end(); ++it) {
			if (it->pos == _pos) {
				list.erase(it);
				break;
			}
		}
	}

	// Size
	unsigned int Size() {
		return list.size();
	}

private:
	typedef std::multiset<PriorityQueueItem, PriorityQueueItem::PriorityQueueComparator> ContainerType;
	ContainerType list;
};

// Get postions that are passable from a location
void GetPassableNeighbours(Map* _map, const sf::Vector2i& _pos, std::vector<sf::Vector2i>* _neighbours) {
	_neighbours->clear();

	if (!_map->IsTileCollidable(_pos.x + 1, _pos.y))
		_neighbours->push_back(sf::Vector2i(_pos.x + 1, _pos.y));
	if (!_map->IsTileCollidable(_pos.x - 1, _pos.y))
		_neighbours->push_back(sf::Vector2i(_pos.x - 1, _pos.y));
	if (!_map->IsTileCollidable(_pos.x, _pos.y + 1))
		_neighbours->push_back(sf::Vector2i(_pos.x, _pos.y + 1));
	if (!_map->IsTileCollidable(_pos.x, _pos.y - 1))
		_neighbours->push_back(sf::Vector2i(_pos.x, _pos.y - 1));

	// Diagonals
	// Top Right
	if (!_map->IsTileCollidable(_pos.x + 1, _pos.y - 1) &&
		!_map->IsTileCollidable(_pos.x + 1, _pos.y) &&
		!_map->IsTileCollidable(_pos.x, _pos.y - 1))
		_neighbours->push_back(sf::Vector2i(_pos.x + 1, _pos.y - 1));

	// Top Left
	if (!_map->IsTileCollidable(_pos.x - 1, _pos.y - 1) &&
		!_map->IsTileCollidable(_pos.x - 1, _pos.y) &&
		!_map->IsTileCollidable(_pos.x, _pos.y - 1))
		_neighbours->push_back(sf::Vector2i(_pos.x - 1, _pos.y - 1));

	// Bottom Left
	if (!_map->IsTileCollidable(_pos.x - 1, _pos.y + 1) &&
		!_map->IsTileCollidable(_pos.x - 1, _pos.y) &&
		!_map->IsTileCollidable(_pos.x, _pos.y + 1))
		_neighbours->push_back(sf::Vector2i(_pos.x - 1, _pos.y + 1));

	// Bottom Right
	if (!_map->IsTileCollidable(_pos.x + 1, _pos.y + 1) &&
		!_map->IsTileCollidable(_pos.x + 1, _pos.y) &&
		!_map->IsTileCollidable(_pos.x, _pos.y + 1))
		_neighbours->push_back(sf::Vector2i(_pos.x + 1, _pos.y + 1));
}

// Get a path
bool PathFinder::GetAStarPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, std::vector<sf::Vector2f>* _path) {
	// Get the width height and tilesize of the map
	int mapwidth = _map->GetWidth();
	int mapheight = _map->GetHeight();
	int tilewidth = _map->GetTileWidth();
	int tileheight = _map->GetTileHeight();
	
	// See if we have a big enough grid
	if (mapwidth * mapheight > nodegridsize *  nodegridsize)
		return false; // We don't have a big enough grid.

	// Clear the grid for the map
	memset(nodegrid, 0, sizeof(PathNode) * mapwidth * mapheight);

	// Get the start and end in tile coords
	sf::Vector2i start(_start.x / _map->GetTileWidth(), _start.y / _map->GetTileHeight());
	sf::Vector2i end(_end.x / _map->GetTileWidth(), _end.y / _map->GetTileHeight());

	// Create the open list
	PriorityQueue openlist;

	// List of neighbours
	std::vector<sf::Vector2i> neighbours;

	// Put the starting square into the openlist
	PathNode* startnode = GetGrid(start, mapwidth);
	startnode->open = true;
	startnode->parent = start;
	startnode->H = MannhattonDistance(start - end);
	openlist.Push(start, startnode->H + startnode->G);

	// While there is stuff on the open list
	while (openlist.Size() != 0) {
		// Get the node on the openlist with the lowest F score.
		sf::Vector2i nodepos = openlist.Pop();
		PathNode* node = GetGrid(nodepos, mapwidth);

		// Removed from the open list, added to the closed.
		node->closed = true;
		node->open = false;

		// If this is the end node, then we found our path
		if (nodepos == end) {
			// Push the positions onto the path
			sf::Vector2i currentpos = nodepos;
			PathNode* currentnode = node;
			for (; currentnode->parent != currentpos;) {
				_path->push_back(sf::Vector2f(currentpos.x * tilewidth + tilewidth / 2.0f,
											  currentpos.y * tileheight + tileheight / 2.0f));

				currentpos = currentnode->parent;
				currentnode = GetGrid(currentpos, mapwidth);
			}

			return true;
		}

		// Get neighbours
		GetPassableNeighbours(_map, nodepos, &neighbours);
		for (int i = 0 ; i < (int)neighbours.size(); ++i) {
			// Get this node
			PathNode* neighbournode = GetGrid(neighbours[i], mapwidth);
			if (neighbournode->closed)
				continue; // Ignore nodes on the closed list

			// Calculate G and H values
			float G = node->G + VectorLength(nodepos - neighbours[i]);
			float H = MannhattonDistance(neighbours[i] - end);

			// Add to the openlist if it isn't there
			if (!neighbournode->open) {
				neighbournode->open = true;
				neighbournode->G = G;
				neighbournode->H = H;
				neighbournode->parent = nodepos;

				openlist.Push(neighbours[i], G+H);
			} else {
				// If the node was alredy on the list, if we have a lower G + H score then replace it.
				if (G + H < neighbournode->G + neighbournode->H) {
					// Set the new values
					neighbournode->G = G;
					neighbournode->H = H;
					neighbournode->parent = nodepos;

					// Re-order it in the list
					openlist.Remove(neighbours[i]);
					openlist.Push(neighbours[i], G+H);
				}
			}
		}
	}
}

void PathFinder::DrawPath(Map* _map, sf::RenderTarget* _target, std::vector<sf::Vector2f>* _path) {
	sf::VertexArray lines(sf::LinesStrip, _path->size());
	for (int i = 0; i < (int)_path->size(); ++i) {
		lines[i] = sf::Vertex(_path->at(i),
							  sf::Color::Red);
	}
	_target->draw(lines);
}

// THETA STAR
bool PathFinder::GetThetaStarPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, float _radius, std::vector<sf::Vector2f>* _path) {
	// Get the width height and tilesize of the map
	int mapwidth = _map->GetWidth();
	int mapheight = _map->GetHeight();
	int tilewidth = _map->GetTileWidth();
	int tileheight = _map->GetTileHeight();
	
	// See if we have a big enough grid
	if (mapwidth * mapheight > nodegridsize *  nodegridsize)
		return false; // We don't have a big enough grid.

	// Clear the grid for the map
	memset(nodegrid, 0, sizeof(PathNode) * mapwidth * mapheight);

	// Get the start and end in tile coords
	sf::Vector2i start(_start.x / tilewidth, _start.y / tileheight);
	sf::Vector2i end(_end.x / tilewidth, _end.y / tileheight);

	// If there is a line of sight between the start and end, return that
	sf::Vector2f diff = VectorNormalize(_start - _end);
	sf::Vector2f perp(diff.y, -diff.x);
	if (_map->Raycast(_start + perp * _radius, _end + perp * _radius) == _end + perp * _radius &&
		_map->Raycast(_start - perp * _radius, _end - perp * _radius) == _end - perp * _radius) {
		_path->push_back(_end);
		_path->push_back(_start);
		return true;
	}

	// Create the open list
	PriorityQueue openlist;

	// List of neighbours
	std::vector<sf::Vector2i> neighbours;

	// Put the starting square into the openlist
	PathNode* startnode = GetGrid(start, mapwidth);
	startnode->open = true;
	startnode->parent = start;
	startnode->H = MannhattonDistance(start - end);
	openlist.Push(start, startnode->H + startnode->G);

	// While there is stuff on the open list
	while (openlist.Size() != 0) {
		// Get the node on the openlist with the lowest F score.
		sf::Vector2i nodepos = openlist.Pop();
		PathNode* node = GetGrid(nodepos, mapwidth);

		// Removed from the open list, added to the closed.
		node->closed = true;
		node->open = false;

		// If this is the end node, then we found our path
		if (nodepos == end) {
			// Push the positions onto the path
			sf::Vector2i currentpos = nodepos;
			PathNode* currentnode = node;
			_path->push_back(sf::Vector2f(currentpos.x * tilewidth + tilewidth / 2.0f,
										currentpos.y * tileheight + tileheight / 2.0f));

			for (; currentnode->parent != currentpos;) {
				currentpos = currentnode->parent;
				currentnode = GetGrid(currentpos, mapwidth);

				_path->push_back(sf::Vector2f(currentpos.x * tilewidth + tilewidth / 2.0f,
											  currentpos.y * tileheight + tileheight / 2.0f));
			}

			return true;
		}

		// Get neighbours
		GetPassableNeighbours(_map, nodepos, &neighbours);
		for (int i = 0 ; i < (int)neighbours.size(); ++i) {
			PathNode* neighbournode = GetGrid(neighbours[i], mapwidth);
			if (!neighbournode->closed) {
				if (!neighbournode->open) {
					neighbournode->G = 999999999999999999.0f; // Infinity
					neighbournode->H = MannhattonDistance(end - neighbours[i]);
					neighbournode->parent = neighbours[i];
				}

				// UPDATE VERTEX
				float G = neighbournode->G;

				// COMPUTE COST
				
				// Get the parent of the current node
				PathNode* parent = GetGrid(node->parent, mapwidth);
				sf::Vector2i parentpos = node->parent;

				// See if we have line of sight
				sf::Vector2f neighbourposf(neighbours[i].x * tilewidth + tilewidth/2.0f, neighbours[i].y * tileheight + tileheight/2.0f);
				sf::Vector2f parentposf(parentpos.x * tilewidth + tilewidth/2.0f, parentpos.y * tileheight + tileheight/2.0f);

				// Get the perpendicular
				sf::Vector2f diff = VectorNormalize(parentposf - neighbourposf);
				sf::Vector2f perp(diff.y, -diff.x);

				if (_map->Raycast(parentposf + perp * _radius, neighbourposf + perp * _radius) == neighbourposf + perp * _radius &&
					_map->Raycast(parentposf - perp * _radius, neighbourposf - perp * _radius) == neighbourposf - perp * _radius) {
					/* Path 2 */
					if (parent->G + VectorLength(parentpos - neighbours[i]) < neighbournode->G) {
						neighbournode->parent = parentpos;
						neighbournode->G = parent->G + VectorLength(parentpos - neighbours[i]);
					}
				} else {
					/* Path 1 */
					if (node->G + VectorLength(nodepos - neighbours[i]) < neighbournode->G) {
						neighbournode->parent = nodepos;
						neighbournode->G = node->G + VectorLength(nodepos - neighbours[i]);
					}
				}
				// END COMPUTE COST

				if (neighbournode->G < G) {
					if (neighbournode->open)
						openlist.Remove(neighbours[i]);
					
					// Insert
					openlist.Push(neighbours[i], neighbournode->G + neighbournode->H);
					neighbournode->open = true;
				}

				// END UPDATE VERTEX
			}
		}
	}
}

void PathFinder::DrawThetaStarPath(Map* _map, sf::RenderTarget* _target, std::vector<sf::Vector2f>* _path) {
	sf::VertexArray lines(sf::LinesStrip, _path->size());
	for (int i = 0; i < (int)_path->size(); ++i) {
		lines[i] = sf::Vertex(_path->at(i),
							  sf::Color::Green);
	}
	_target->draw(lines);
}