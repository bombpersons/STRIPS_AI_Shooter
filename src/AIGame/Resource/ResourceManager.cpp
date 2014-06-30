#include <AIGame/Resource/ResourceManager.hpp>

// Constructors
ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

// Get resources
sf::Texture* ResourceManager::GetTexture(const std::string& _path) {
	// See if we have already loaded this.
	TextureMap::iterator iterator = textures.find(_path);
	if (iterator == textures.end()) {
		// Load the texture.
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(_path)) {
			delete texture;
			return NULL;
		}

		textures[_path] = texture;
	}

	// Get it from the map
	return textures[_path];
}

void ResourceManager::Clear() {
	// Delete all the resources
	for (TextureMap::iterator it = textures.begin(); it != textures.end(); ++it) {
		delete(it->second);
	}
	textures.clear();
}