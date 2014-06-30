#ifndef AI_MAP_HPP
#define AI_MAP_HPP

#include <AIGame/Macros.hpp>
#include <AIGame/Graphics/TileDrawer.hpp>
#include <AIGame/Entities/EntityList.hpp>
#include <AIGame/Weapons/WeaponsList.hpp>
#include <AIGame/Entities/ActorEntity.hpp>

// TinyXML
#include <TinyXML2/tinyxml2.hpp>
using namespace tinyxml2;

class Map {
public:
	// Classes
	struct Layer {
		unsigned short* data;
		int width, height;
		bool collide;
	};

	// Constructor
	Map(const std::string& _filename);
	Map(int _tilewidth, int _tileheight, int _width, int _height, int _depth=1);
	virtual ~Map();

	// Load a map from a file
	bool LoadFromFile(const std::string& _filename);

	// Set the texture
	bool SetTexture(const std::string& _path);

	// Get the raw data for a layer
	Layer* GetLayer(int _index);

	// Get a specific tile
	unsigned short GetTile(int _index, int _x, int _y);
	int GetCollisionLayer(int _x, int _y);
	unsigned short GetCollisionTile(int _x, int _y);
	sf::Color GetColorAt(int _layer, int _x, int _y, const sf::Vector2f& _pos);

	// Check if a tile is colliding
	bool IsTileCollidable(int _x, int _y);
	
	// Get width, height, tilewidth and tileheight
	int GetTileWidth();
	int GetTileHeight();
	int GetWidth();
	int GetHeight();
	int GetDepth();
	sf::Vector2i GetTilePos(const sf::Vector2f& _pos);
	sf::Vector2f GetWorldPos(const sf::Vector2i& _pos);
	int GetCoverPoints(std::vector<sf::Vector2f>* _from, const sf::Vector2f& _current, float _range, sf::Vector2i* _points, int _maxpoints);
	sf::Vector2i GetClearTile();

	// Set a specific tile
	void SetTile(int _index, int _x, int _y, unsigned short _val);

	// Raycasting
	sf::Vector2f Raycast(const sf::Vector2f& _start, const sf::Vector2f& _end, sf::Vector2i* _hittile=NULL, ActorEntity** _hit=NULL, ActorEntity* _ignore=NULL);

	// Get the entity list
	EntityList* GetEntityList();

	// Get the list of possible weapons
	WeaponsList* GetWeaponList();

	// Draw the map with a given to a render target
	void DrawLayer(int _depth, sf::RenderTarget* _target);
	void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

	// Update
	void Update(Game* _game, float _delta);

	// Clear a map
	void Clear();
	
private:
	// XML parsing functions
	void SpawnObject(XMLElement* _object);

	// The tiledata for the map
	std::vector<Layer> layers;
	
	// The depth at which to draw entities
	int entitydepth;

	// The entities on the map.
	EntityList entitylist;

	// Width and height the map
	int tilewidth, tileheight;
	int width, height;

	// The tileset.
	sf::Texture texture;
	sf::Image image;
	TileDrawer tiledrawer;

	// A list of possible weapon descriptions
	WeaponsList weapons;

	// Don't allow copying
	UNCOPYABLE(Map)
};

#endif