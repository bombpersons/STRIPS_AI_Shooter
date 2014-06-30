#ifndef AI_SPRITEBATCH_HPP
#define AI_SPRITEBATCH_HPP

#include <AIGame/Macros.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class SpriteBatch {
public:
	// Constructors
	SpriteBatch(unsigned int _max=100);
	virtual ~SpriteBatch();

	// Functions for drawing.
	void Draw(sf::RenderTarget* _target, sf::Texture* _texture, sf::Vector2f _pos, sf::Vector2f _origin, sf::Vector2f _scale, float _rot, const sf::Color _color, sf::FloatRect* _rect=NULL);

	// Flush
	void Flush(sf::RenderTarget* _target);
private:
	// The target we are drawing to.
	sf::RenderTarget* target;

	// The current count of sprites stored in the buffer
	unsigned int count;

	// The maximum amount of sprites to store before flushing
	unsigned int max;

	// A list of the vertices stored.
	sf::Vertex* vertices;

	// The current texture we are using.
	sf::Texture* texture;

	// Make it so we can't copy this class.
	UNCOPYABLE(SpriteBatch)
};

#endif