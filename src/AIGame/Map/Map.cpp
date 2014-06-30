#include <AIGame/Map/Map.hpp>
#include <AIGame/Game.hpp>
#include <AIGame/Entities/PickupEntity.hpp>
#include <AIGame/Entities/WeaponPickupEntity.hpp>
#include <AIGame/Entities/HealthPickupEntity.hpp>
#include <AIGame/Entities/PlayerRespawnerEntity.hpp>

// Base64 decoder
#include <Base64/Base64.hpp>

// Debugging
#include <iostream>
#include <algorithm>

#include <Misc/VectorMaths.hpp>
#include <Misc/Geometry.hpp>

// Constructors
Map::Map(const std::string& _filename) :
	width(0),
	height(0),
	tilewidth(0),
	tileheight(0),
	entitydepth(0),
	texture() {

	LoadFromFile(_filename);
}

Map::Map(int _tilewidth, int _tileheight, int _width, int _height, int _depth) :
	width(_width),
	height(_height),
	tilewidth(_tilewidth),
	tileheight(_tileheight),
	entitydepth(0),
	texture() {

	// Create the layers
	for (int i = 0; i < _depth; ++i) {
		Map::Layer layer;
		layer.collide = false;
		layer.height = height;
		layer.width = width;
		layer.data = new unsigned short[_width * _height];

		// Add to the list.
		layers.push_back(layer);
	}
}

Map::~Map() {
	Clear();
}

// Load a map from a file
bool Map::LoadFromFile(const std::string& _filename) {
	// Open this xml file and validate it.
	XMLDocument doc;
	if (!(doc.LoadFile(_filename.c_str()) == XML_NO_ERROR)) {
		// There was an error loading the file...
		return false;
	}

	// Find the "map" tag.
	XMLElement* map = doc.FirstChildElement("map");
	if (map == NULL)
		return false;

	// Get the width and height of the map.
	this->width = map->IntAttribute("width");
	this->height = map->IntAttribute("height");
	this->tilewidth = map->IntAttribute("tilewidth");
	this->tileheight = map->IntAttribute("tileheight");

	// Grab the tileset image location.
	XMLElement* tilesetelement = map->FirstChildElement("tileset");
	if (tilesetelement == NULL)
		return false;
	XMLElement* tilesetimageelement = tilesetelement->FirstChildElement("image");
	if (tilesetimageelement == NULL)
		return false;

	// Extract the proper place of the file
	std::string path = "data/levels/";
	path.append(tilesetimageelement->Attribute("source"));

	// Now load the image
	if (!texture.loadFromFile(path))
		return false;
	texture.setSmooth(false);
	texture.setRepeated(true);
	image.loadFromFile(path);

	// Grab the layers
	XMLElement* layer = map->FirstChildElement("layer");
	if (layer == NULL)
		return false;
	do {
		// Get the data element
		XMLElement* data = layer->FirstChildElement("data");
		if (data == NULL)
			continue;

		// Create the data for the layer.
		Layer layerdata;
		layerdata.width = width;
		layerdata.height = height;
		layerdata.collide = false;
		layerdata.data = NULL;

		// Check the encoding.
		const char* encoding = data->Attribute("encoding");
		if (encoding == NULL)
			continue;
		if (strcmp(encoding, "base64") == 0) {
			// Uncompressed base64, we can do this one.
			std::string datatext = std::string(data->GetText());

			// Get rid of whitespace and newlines
			std::remove(datatext.begin(), datatext.end(), '\n');
			std::remove(datatext.begin(), datatext.end(), ' ');

			// Decode this.
			std::string decoded = base64_decode(datatext);
		
			// Allocate memory for the layer
			layerdata.data = new unsigned short[width * height];

			// Loop through and get the data.
			// This site has a good explanation of what's going on here http://sourceforge.net/apps/mediawiki/tiled/index.php?title=Examining_the_map_format
			for (int i = 0; i < width*height; i++) { // Each id is 4 bytes
				layerdata.data[i] = ((unsigned int*)decoded.data())[i];
			}
		} else {
			continue;
		}

		// Check for properties
		XMLElement* properties = layer->FirstChildElement("properties");

		// Check if we even have any.
		if (properties != NULL) {
			XMLElement* property = properties->FirstChildElement("property");
			for (; property != NULL; property = property->NextSiblingElement("property")) {
				const char* name = property->Attribute("name");
				const char* value = property->Attribute("value");

				if (name != NULL && value != NULL) {
					// Check for collidable property
					if (strcmp(name, "collidable") == 0) {
						layerdata.collide = (strcmp(value, "true") == 0);
					}
				}
			}
		}

		// Add to the layer list
		layers.push_back(layerdata);
	} while ((layer = layer->NextSiblingElement("layer")) != NULL);

	// Get any objects on the map.
	XMLElement* objectgroup = map->FirstChildElement("objectgroup");
	if (objectgroup != NULL) {
		do {
			// Find all the objects from within
			XMLElement* object = objectgroup->FirstChildElement("object");
			if (object != NULL) {
				do {
					// Send the object to the object spawning function
					SpawnObject(object);
				} while ((object = object->NextSiblingElement("object")) != NULL);
			}
		} while ((objectgroup = objectgroup->NextSiblingElement("objectgroup")) != NULL);
	}


	// Get any map properties
	XMLElement* properties = map->FirstChildElement("properties");
	if (properties != NULL) {
		XMLElement* property = properties->FirstChildElement("property");
		for (; property != NULL; property = property->NextSiblingElement("property")) {
			const char* name = property->Attribute("name");
			const char* value = property->Attribute("value");

			if (name != NULL && value != NULL) {
				// Check for collidable property
				if (strcmp(name, "entitylayer") == 0) {
					entitydepth = property->IntAttribute("value");
				}
			}
		}
	}

	return true;
}

// Spawn an object
void Map::SpawnObject(XMLElement* _object) {
	// Check for the type
	const char* type = _object->Attribute("type");
	if (type == NULL)
		return;

	// Get a dictionary of properties
	std::unordered_map<std::string, std::string> propertiesmap;
	XMLElement* properties = _object->FirstChildElement("properties");
	if (properties != NULL) {
		XMLElement* property = properties->FirstChildElement("property");
		do {
			const char* key = property->Attribute("name");
			const char* value = property->Attribute("value");
			propertiesmap[key] = value;
		} while ((property = property->NextSiblingElement("property")) != NULL);
	}

	// Position
	sf::Vector2f position;
	position.x = _object->FloatAttribute("x");
	position.y = _object->FloatAttribute("y");

	// Create the object
	if (strcmp(type, "weapon") == 0) {
		WeaponPickupEntity* entity = new WeaponPickupEntity("weapon", 
															propertiesmap["texturepath"], 
															propertiesmap["weaponname"],
															(float)atof(propertiesmap["respawntime"].c_str()));
		entity->SetPosition(position);
		GetEntityList()->AddEntity(entity);
	} else if (strcmp(type, "health") == 0) {
		HealthPickupEntity* entity = new HealthPickupEntity("health",
															propertiesmap["texturepath"],
															(float)atof(propertiesmap["respawntime"].c_str()));
		entity->SetPosition(position);
		GetEntityList()->AddEntity(entity);
	} else if (strcmp(type, "player") == 0) {
		// Check whether or not it is an AI player or a Human player
		Entity* entity = NULL;
		if (strcmp(propertiesmap["ai"].c_str(), "true") == 0) {
			// Make an ai
			entity = new PlayerRespawnerEntity("aiplayerspawner", true);
		} else {
			// Make a human player
			entity = new PlayerRespawnerEntity("playerspawner", false);
		}
		entity->SetPosition(position);
		GetEntityList()->AddEntity(entity);
	}
}

// Get a layer to draw.
Map::Layer* Map::GetLayer(int _index) {
	if (_index < 0 || _index >= (int)layers.size())
		return NULL;
	return &layers[_index];
}

// Get a tile
unsigned short Map::GetTile(int _index, int _x, int _y) {
	if (_index < 0 || _index >= (int)layers.size())
		return 0;
	if (_x < 0 || _x >= width || _y < 0 || _y >= height)
		return 0;
	return layers[_index].data[_x + _y * width];
}
int Map::GetCollisionLayer(int _x, int _y) {
	for (int i = 0; i < (int)layers.size(); ++i) {
		if (layers[i].collide && GetTile(i, _x, _y) != 0)
			return i;
	}
	return -1;
}
unsigned short Map::GetCollisionTile(int _x, int _y) {
	for (int i = 0; i < (int)layers.size(); ++i)
		if (layers[i].collide)
			return GetTile(i, _x, _y);
	return 0;
}
sf::Color Map::GetColorAt(int _layer, int _x, int _y, const sf::Vector2f& _pos) {
	// Get the tile
	unsigned short tileid = GetTile(_layer, _x, _y) - 1;

	// Get how many tiles on the x axis there are.
	int amounttilesx = (int)texture.getSize().x / tilewidth;
	int amounttilesy = (int)texture.getSize().y / tileheight;

	// If the tile is out of bounds return black
	if (tileid < 0 || tileid >= amounttilesx * amounttilesy)
		return sf::Color::Black;

	// Get the location on the texture.
	int tilex = (tileid % amounttilesx) * tilewidth;
	int tiley = (tileid / amounttilesx) * tileheight;

	// Get the actual location on the texture
	sf::Vector2f pos = sf::Vector2f(tilex, tiley) + _pos;

	// Get the pixel color
	return image.getPixel((int)pos.x, (int)pos.y);
}

// Get width, height, tilewidth and tileheight
int Map::GetTileWidth() {
	return tilewidth;
}
int Map::GetTileHeight() {
	return tileheight;
}
int Map::GetWidth() {
	return width;
}
int Map::GetHeight() {
	return height;
}
int Map::GetDepth() {
	return (int)layers.size();
}
sf::Vector2i Map::GetTilePos(const sf::Vector2f& _pos) {
	return sf::Vector2i(_pos.x / tilewidth, _pos.y / tileheight);
}
sf::Vector2f Map::GetWorldPos(const sf::Vector2i& _pos) {
	return sf::Vector2f(_pos.x * tilewidth + tilewidth / 2.0f,
						_pos.y * tileheight + tileheight / 2.0f);
}

int Map::GetCoverPoints(std::vector<sf::Vector2f>* _from, const sf::Vector2f& _current, float _range, sf::Vector2i* _points, int _maxpoints) {
	// Look for points that we can hide behind within the radius.
	std::vector<sf::Vector2i> points;

	// Get the bounds
	sf::Vector2i topleft = GetTilePos(_current - sf::Vector2f(_range, _range));
	sf::Vector2i bottomright = GetTilePos(_current + sf::Vector2f(_range, _range));

	// Do raycasts from the from position to each square and see if it is possible cover.
	for (int x = topleft.x; x < bottomright.x; ++x) {
		for (int y = topleft.y; y < bottomright.y; ++y) {
			// No good if the tile is solid
			if (IsTileCollidable(x, y))
				continue;

			// Get world coords
			sf::Vector2f pos = GetWorldPos(sf::Vector2i(x, y));
			bool safe = true;
			for (int i = 0; i < _from->size(); ++i) {
				if (Raycast(_from->at(i), pos) == pos) {
					safe = false;
				}
			}
			if (safe) {
				points.push_back(sf::Vector2i(x, y));
			}
		}
	}

	// Pick random points from our list
	if (points.size() > _maxpoints) {
		std::random_shuffle(points.begin(), points.end());
	}
	for (int i = 0; i < _maxpoints && i < points.size(); ++i) {
		_points[i] = points[i];
	}
	if (points.size() < _maxpoints)
		return points.size();
	return _maxpoints;
}

sf::Vector2i Map::GetClearTile() {
	// Pick a random location until we get a tile that is clear
	while (true) {
		int x = rand() % width;
		int y = rand() % height;
		if (!IsTileCollidable(x, y))
			return sf::Vector2i(x, y);
	}
}

// Check whether or not a tile is collidable
bool Map::IsTileCollidable(int _x, int _y) {
	if (_x < 0 || _x >= width || _y < 0 || _y >= height)
		return true; // Outside tiles are solid.
	
	for (int i = 0; i < (int)layers.size(); ++i) {
		if (GetLayer(i)->collide) {
			unsigned short id = GetTile(i, _x, _y);
			if (id != 0)
				return true;
		}
	}
	return false;
}

// Set a tile
void Map::SetTile(int _index, int _x, int _y, unsigned short _val) {
	if (_index < 0 || _index >= (int)layers.size())
		return;
	if (_x < 0 || _x >= width || _y < 0 || _y >= height)
		return;
	layers[_index].data[_x + _y * width] = _val;
}

// http://www.cse.yorku.ca/~amana/research/grid.pdf
// http://dev.mothteeth.com/2011/11/2d-ray-casting-on-a-grid-in-as3/
sf::Vector2f Map::Raycast(const sf::Vector2f& _start, const sf::Vector2f& _end, sf::Vector2i* _hittile, ActorEntity** _hit, ActorEntity* _ignore) {
	// First check if this hit any actors
	std::vector<ActorEntity*> actors;
	ActorEntity* closestActor = 0;
	float closestDistance = 9999999999999999999999.0f;
	sf::Vector2f closestHit;
	if (_hit != NULL) {
		actors = GetEntityList()->GetEntities<ActorEntity>();
		for (int i = 0; i < (int)actors.size(); ++i) {
			if (actors[i] == _ignore)
				continue;

			sf::Vector2f intersection;
			if (LineIntersectsCircle(_start, _end, actors[i]->GetPosition(), actors[i]->GetRadius(), &intersection)) {
				// Calculate the distance
				float dist = VectorLength(_start - intersection);
				if (dist < closestDistance) {
					closestDistance = dist;
					closestActor = actors[i];
					closestHit = intersection;
				}
			}
		}
	}
	
	// Get the points in terms of tiles
	sf::Vector2f start(_start.x / tilewidth, _start.y / tileheight);
	sf::Vector2f end(_end.x / tilewidth, _end.y / tileheight);

	// If the two points don't cross any boundaries, then there is no collision at all.
	if ((int)start.x == (int)end.x && (int)start.y == (int)end.y)
		return _end;

	// Initialize stepx and stepy
	int stepx = (end.x > start.x) ? 1 : -1;
	int stepy = (end.y > start.y) ? 1 : -1;
	sf::Vector2f dir = end - start;

	// tdeltax and y
	float ratiox = dir.x / dir.y;
	float ratioy = dir.y / dir.x;

	// deltay and x
	float deltax = fabs(end.y - start.y);
	float deltay = fabs(end.x - start.x);

	// Find the starting tile
	int testx = (int)start.x;
	int testy = (int)start.y;

	// If the this tile is collidable..
	if (IsTileCollidable(testx, testy))
		return _start;

	// maxx and maxy
	float maxx = (float)(deltax * ((stepx > 0) ? (1.0f - fmod(start.x, 1)) : fmod(start.x, 1)));
	float maxy = (float)(deltay * ((stepy > 0) ? (1.0f - fmod(start.y, 1)) : fmod(start.y, 1)));

	int endtilex = (int)end.x;
	int endtiley = (int)end.y;

	// Traversing
	sf::Vector2f collisionpoint;

	// Loop
	while (testx != endtilex || testy != endtiley) {
		if (maxx < maxy) {
			maxx += deltax;
			testx += stepx;

			if (IsTileCollidable(testx, testy)) {
				collisionpoint.x = (float)testx;
				if (stepx < 0) collisionpoint.x += 1.0f;
				collisionpoint.y = start.y + ratioy * (collisionpoint.x - start.x);
				collisionpoint.x *= tilewidth;
				collisionpoint.y *= tileheight;

				// If this is closer than the intersection with an actor, then use this value
				if (VectorLength(collisionpoint - _start) > closestDistance) {
					if (_hit != NULL)
						*_hit = closestActor;
					return closestHit;
				} else {
					if (_hittile != NULL) {
						_hittile->x = testx;
						_hittile->y = testy;
					}
					return collisionpoint;
				}
			}
		} else {
			maxy += deltay;
			testy += stepy;

			if (IsTileCollidable(testx, testy)) {
				collisionpoint.y = (float)testy;
				if (stepy < 0) collisionpoint.y += 1.0f;
				collisionpoint.x = start.x + ratiox * (collisionpoint.y - start.y);
				collisionpoint.x *= tilewidth;
				collisionpoint.y *= tileheight;

				// If this is closer than the intersection with an actor, then use this value
				if (VectorLength(collisionpoint - _start) > closestDistance) {
					if (_hit != NULL)
						*_hit = closestActor;
					return closestHit;
				} else {
					if (_hittile != NULL) {
						_hittile->x = testx;
						_hittile->y = testy;
					}
					return collisionpoint;
				}
			}
		}
	}

	// Didn't hit any terrain
	if (closestActor != NULL) {
		if (_hit != NULL)
			*_hit = closestActor;
		return closestHit;
	} 
	return _end;
}

// Get the list of entities
EntityList* Map::GetEntityList() {
	return &entitylist;
}

// Get a list of the possible weapons
WeaponsList* Map::GetWeaponList() {
	return &weapons;
}

// Draw
void Map::DrawLayer(int _depth, sf::RenderTarget* _target) {
	// Check if the depth is valid
	if (_depth < 0 || _depth >= (int)layers.size())
		return;

	// Calculate which tiles you can see on the screen
	sf::Vector2f topleft = _target->mapPixelToCoords(sf::Vector2i(0, 0), _target->getView());
	sf::Vector2f bottomright = _target->mapPixelToCoords(sf::Vector2i(_target->getSize().x, _target->getSize().y), _target->getView());

	int datax = (int)(topleft.x / tilewidth) - 1;
	int datay = (int)(topleft.y / tileheight) - 1;
	int datawidth = (int)((bottomright.x / tilewidth) + 1) - datax;
	int dataheight = (int)((bottomright.y / tileheight) + 1) - datay;

	tiledrawer.Draw(_target,
					&texture,
					tilewidth, tileheight,
					0, 0, (float)_target->getSize().x, (float)_target->getSize().y,
					layers[_depth].data,
					datax, datay, datawidth, dataheight,
					width, height);
}

void Map::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	// Draw the bottom layers first
	for (int i = 0; i <= entitydepth && i < (int)layers.size(); ++i) {
		DrawLayer(i, _target);
	}

	// Draw particles
	_game->GetParticles()->Draw(_target);

	// Draw the entities
	entitylist.Draw(_game, _delta, _target);

	// Draw the layers in front of the entities.
	if (entitydepth < (int)layers.size()) {
		for (int i = entitydepth+1; i < (int)layers.size(); ++i) {
			DrawLayer(i, _target);
		}
	}
}

// Update
void Map::Update(Game* _game, float _delta) {
	entitylist.Update(_game, _delta);
}

// Clear
void Map::Clear() {
	// Delete the layers if we need to.
	for (int i = 0; i < (int)layers.size(); ++i) {
		if (layers[i].data != NULL)
			delete [] layers[i].data;
	}

	// Clear the entitylist.
	entitylist.Clear();
}