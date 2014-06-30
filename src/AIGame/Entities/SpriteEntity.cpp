#include <AIGame/Entities/SpriteEntity.hpp>
#include <AIGame/Game.hpp>

// Constructors
SpriteEntity::SpriteEntity(const std::string& _name, const std::string& _texture) :
	Entity(_name),
	texture(_texture) {
	
	color = sf::Color::White;
}

SpriteEntity::~SpriteEntity() {
}

// Draw
void SpriteEntity::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	// Create a sprite.
	sf::Sprite sprite(*_game->GetResources()->GetTexture(texture));
	sprite.setPosition(GetPosition());
	sprite.setRotation(RADTODEG(GetRotation()));
	sprite.setScale(GetScale());
	sprite.setOrigin(sf::Vector2f(sprite.getTexture()->getSize().x / 2.0f, sprite.getTexture()->getSize().y / 2.0f));
	sprite.setColor(color);

	// Draw it.
	_target->draw(sprite);
}

// Get vars
const std::string& SpriteEntity::GetTexture() {
	return texture;
}

// Set the color
void SpriteEntity::SetColor(const sf::Color& _color) {
	color = _color;
}