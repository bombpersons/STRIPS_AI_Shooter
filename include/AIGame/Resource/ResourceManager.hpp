#ifndef AI_RESOURCEMANAGER_HPP
#define AI_RESOURCEMANAGER_HPP

#include <unordered_map>

#include <AIGame/Macros.hpp>

#include <SFML/Graphics.hpp>

class ResourceManager {
public:
	// Constructors
	ResourceManager();
	virtual ~ResourceManager();

	// Get a resource
	sf::Texture* GetTexture(const std::string& _path);

	// Clear the map
	void Clear();

private:
	// Dictionaries
	typedef std::unordered_map<std::string, sf::Texture*> TextureMap;
	TextureMap textures;

	// No copying of this class.
	UNCOPYABLE(ResourceManager)
};

#endif