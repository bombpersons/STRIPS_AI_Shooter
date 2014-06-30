#ifndef AI_CONCURRENTPATHFINDER_HPP
#define AI_CONCURRENTPATHFINDER_HPP

#include <SFML/Graphics.hpp>

#include <Misc/VectorMaths.hpp>
#include <thread>
#include <set>
#include <vector>

class Map;

class ConcurrentPathFinder {
public:
	// Constructors
	ConcurrentPathFinder();
	virtual ~ConcurrentPathFinder();

	// Request a path
	void RequestPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, float _radius, std::vector<sf::Vector2f>* _path, std::vector<sf::Vector2f>* _enemies);

	// Check if the path has been found
	bool Finding();

	// Draw the path
	void DrawPath(Map* _map, sf::RenderTarget* _target, std::vector<sf::Vector2f>* _path);

private:
	// Classes
	struct Request {
		Map* map;
		sf::Vector2f start;
		sf::Vector2f end;
		float radius;
		std::vector<sf::Vector2f>* path;
		std::vector<sf::Vector2f> enemies;
		ConcurrentPathFinder* finder;
	};

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

	// Priority queue for path finding.
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

	// The thread
	std::thread* thread;

	// The current request
	Request request;

	// Whether or not we are done
	bool done;

	// A large grid of nodes.
#define NODEGRIDSIZE 100
	PathNode nodegrid[NODEGRIDSIZE * NODEGRIDSIZE];

	// Get items
	PathNode* GetGrid(int _x, int _y, int _width);
	PathNode* GetGrid(const sf::Vector2i& _pos, int _width);

	// Get the cost for traveling between two points
	float GetCost(Map* _map, std::vector<sf::Vector2f>* _enemies, const sf::Vector2i& _a, const sf::Vector2i& _b);

	// The actual path request
	static void ThreadedGetPath(Request _request);

	// Get neighbours of a certain point.
	void GetPassableNeighbours(Map* _map, const sf::Vector2i& _pos, std::vector<sf::Vector2i>* _neighbours);

	// Theta Star
	bool GetThetaStarPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, float _radius, std::vector<sf::Vector2f>* _path, std::vector<sf::Vector2f>* _enemies);
};

#endif