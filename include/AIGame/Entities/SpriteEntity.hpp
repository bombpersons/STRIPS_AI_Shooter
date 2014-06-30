#ifndef AI_SPRITEENTITY_HPP
#define AI_SPRITEENTITY_HPP

#include <AIGame/Entities/Entity.hpp>

#include <SFML/Graphics.hpp>

class SpriteEntity : public Entity {
public:
	// Constructors
	SpriteEntity(const std::string& _name, const std::string& _texture);
	virtual ~SpriteEntity();

	// Draw
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

	// Get vars
	const std::string& GetTexture();

	// Set color
	void SetColor(const sf::Color& _color);

private:
	// The texture to use
	std::string texture;
	sf::Color color;
};

#endif