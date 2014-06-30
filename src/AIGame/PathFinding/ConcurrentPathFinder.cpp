#include <AIGame/PathFinding/ConcurrentPathFinder.hpp>
#include <AIGame/Map/Map.hpp>
#include <Misc/Geometry.hpp>

// Constructors
ConcurrentPathFinder::ConcurrentPathFinder() :
	thread(NULL),
	done(true) {

}

ConcurrentPathFinder::~ConcurrentPathFinder() {
}


// Request a path
void ConcurrentPathFinder::RequestPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, float _radius, std::vector<sf::Vector2f>* _path, std::vector<sf::Vector2f>* _enemies) {
	// Set this to be the current request
	request.map = _map;
	request.start = _start;
	request.end = _end;
	request.radius = _radius;
	request.path = _path;
	request.enemies = *_enemies;
	request.finder = this;

	// Check if there isn't a thread running at the moment
	if (!Finding()) {
		done = false;
		thread = new std::thread(ThreadedGetPath, request);
	}
}

// Check if the path has been found
bool ConcurrentPathFinder::Finding() {
	if (thread != NULL) {
		if (done) {
			thread->join();
			delete thread;
			thread = NULL;
			return false;
		} else {
			return true;
		}
	}
	return false;
}

// Get items
ConcurrentPathFinder::PathNode* ConcurrentPathFinder::GetGrid(int _x, int _y, int _width) {
	return &nodegrid[_x + _y * _width];
}
ConcurrentPathFinder::PathNode* ConcurrentPathFinder::GetGrid(const sf::Vector2i& _pos, int _width) {
	return GetGrid(_pos.x, _pos.y, _width);
}

// Get the cost for traveling between two points
float ConcurrentPathFinder::GetCost(Map* _map, std::vector<sf::Vector2f>* _enemies, const sf::Vector2i& _a, const sf::Vector2i& _b) {
	// Get the player and see increase the cost if the path intersects with a radius of the player
	float dist = VectorLength(_a - _b);
	float total = dist;
	for (int i = 0; i < _enemies->size(); ++i) {
		if (LineIntersectsCircle(_map->GetWorldPos(_a), _map->GetWorldPos(_b), _enemies->at(i), 64.0f)) {
			total += dist * 2.0f;
		}
	}

	return total;
}

// ThreadedGetPath
void ConcurrentPathFinder::ThreadedGetPath(Request _request) {
	_request.finder->GetThetaStarPath(_request.map,
									  _request.start,
									  _request.end,
									  _request.radius,
									  _request.path,
									  &_request.enemies);
	_request.finder->done = true;
}

// Get postions that are passable from a location
void ConcurrentPathFinder::GetPassableNeighbours(Map* _map, const sf::Vector2i& _pos, std::vector<sf::Vector2i>* _neighbours) {
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

bool ConcurrentPathFinder::GetThetaStarPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, float _radius, std::vector<sf::Vector2f>* _path, std::vector<sf::Vector2f>* _enemies) {
	// Get the width height and tilesize of the map
	int mapwidth = _map->GetWidth();
	int mapheight = _map->GetHeight();
	int tilewidth = _map->GetTileWidth();
	int tileheight = _map->GetTileHeight();
	
	// See if we have a big enough grid
	if (mapwidth * mapheight > NODEGRIDSIZE *  NODEGRIDSIZE)
		return false; // We don't have a big enough grid.

	// Clear the grid for the map
	memset(nodegrid, 0, sizeof(ConcurrentPathFinder::PathNode) * mapwidth * mapheight);

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
	ConcurrentPathFinder::PriorityQueue openlist;

	// List of neighbours
	std::vector<sf::Vector2i> neighbours;

	// Put the starting square into the openlist
	ConcurrentPathFinder::PathNode* startnode = GetGrid(start, mapwidth);
	startnode->open = true;
	startnode->parent = start;
	startnode->H = MannhattonDistance(start - end);
	openlist.Push(start, startnode->H + startnode->G);

	// While there is stuff on the open list
	while (openlist.Size() != 0) {
		// Get the node on the openlist with the lowest F score.
		sf::Vector2i nodepos = openlist.Pop();
		ConcurrentPathFinder::PathNode* node = GetGrid(nodepos, mapwidth);

		// Removed from the open list, added to the closed.
		node->closed = true;
		node->open = false;

		// If this is the end node, then we found our path
		if (nodepos == end) {
			// Push the positions onto the path
			sf::Vector2i currentpos = nodepos;
			ConcurrentPathFinder::PathNode* currentnode = node;
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
			ConcurrentPathFinder::PathNode* neighbournode = GetGrid(neighbours[i], mapwidth);
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
				ConcurrentPathFinder::PathNode* parent = GetGrid(node->parent, mapwidth);
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
					float cost = GetCost(_map, _enemies, parentpos, neighbours[i]);
					if (parent->G + cost < neighbournode->G) {
						neighbournode->parent = parentpos;
						neighbournode->G = parent->G + cost;
					}
				} else {
					/* Path 1 */
					float cost = GetCost(_map, _enemies, nodepos, neighbours[i]);
					if (node->G + cost < neighbournode->G) {
						neighbournode->parent = nodepos;
						neighbournode->G = node->G + cost;
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

void ConcurrentPathFinder::DrawPath(Map* _map, sf::RenderTarget* _target, std::vector<sf::Vector2f>* _path) {
	sf::VertexArray lines(sf::LinesStrip, _path->size());
	for (int i = 0; i < (int)_path->size(); ++i) {
		lines[i] = sf::Vertex(_path->at(i),
							  sf::Color::Red);
	}
	_target->draw(lines);
}