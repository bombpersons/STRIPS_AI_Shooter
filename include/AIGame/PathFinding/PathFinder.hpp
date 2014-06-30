#ifndef AI_PATHFINDER_HPP
#define AI_PATHFINDER_HPP

#include <unordered_set>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Map;

class PathFinder {
public:
	// A Star
	static bool GetAStarPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, std::vector<sf::Vector2f>* _path);
	static void DrawPath(Map* _map, sf::RenderTarget* _target, std::vector<sf::Vector2f>* _path);

	// Theta Star
	static bool GetThetaStarPath(Map* _map, const sf::Vector2f& _start, const sf::Vector2f& _end, float _radius, std::vector<sf::Vector2f>* _path);
	static void DrawThetaStarPath(Map* _map, sf::RenderTarget* _target, std::vector<sf::Vector2f>* _path);

private:

	PathFinder() {}
	~PathFinder() {}
};

#endif