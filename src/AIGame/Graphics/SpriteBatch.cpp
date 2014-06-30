#include <AIGame/Graphics/Spritebatch.hpp>

// Maths
#include <cmath>

// Constructors
SpriteBatch::SpriteBatch(unsigned int _max) :
	count(0),
	max(_max),
	vertices(NULL),
	texture(NULL) {

		// Create the vertices array
		vertices = new sf::Vertex[max * 4];
}

SpriteBatch::~SpriteBatch() {
	// Delete the array if it had been allocated
	delete [] vertices;
}

// Draw
void SpriteBatch::Draw(sf::RenderTarget* _target, sf::Texture* _texture, sf::Vector2f _pos, sf::Vector2f _origin, sf::Vector2f _scale, float _rot, const sf::Color _color, sf::FloatRect* _rect) {
	// If there is a texture
	// If the rect is NULL, then set it to be a rectangle the full size of the texture
	sf::FloatRect fullrect;
	if (_texture != NULL) {
		fullrect.left = 0;
		fullrect.top = 0;
		fullrect.width = (float)_texture->getSize().x;
		fullrect.height = (float)_texture->getSize().y;
	} else {
		fullrect.left = 0;
		fullrect.top = 0;
		fullrect.width = 1.0f;
		fullrect.height = 1.0f;
	}

	if (_rect == NULL) {
		_rect = &fullrect;
	}

	// Check that we have space and that we are using the same texture we were previously (otherwise we need to flush the buffer).
	if (count > max - 4 || _texture != texture) {
		Flush(_target);
	}

	// Set the current texture
	texture = _texture;

    // Most of this code is taken from http://pastebin.com/y7ptUAkQ (Anonymous author)
    float sin = sinf(_rot);
    float cos = cosf(_rot);

    // Modify the origin point by the scale.
    _origin.x *= _scale.x;
    _origin.y *= _scale.y;
    _scale.x *= _rect->width;
    _scale.y *= _rect->height;

    // Create the vertices.
    sf::Vertex v;
    v.color = _color;

    float pX, pY;

    pX = -_origin.x;
    pY = -_origin.y;
    v.position.x = pX * cos - pY * sin + _pos.x;
    v.position.y = pX * sin + pY * cos + _pos.y;
    v.texCoords.x = _rect->left;
    v.texCoords.y = _rect->top;
    vertices[count + 0] = v;

    pX += _scale.x;
    v.position.x = pX * cos - pY * sin + _pos.x;
    v.position.y = pX * sin + pY * cos + _pos.y;
    v.texCoords.x += _rect->width;
    vertices[count + 1] = v;

    pY += _scale.y;
    v.position.x = pX * cos - pY * sin + _pos.x;
    v.position.y = pX * sin + pY * cos + _pos.y;
    v.texCoords.y += _rect->height;
    vertices[count + 2] = v;

    pX -= _scale.x;
    v.position.x = pX * cos - pY * sin + _pos.x;
    v.position.y = pX * sin + pY * cos + _pos.y;
    v.texCoords.x -= _rect->width;
    vertices[count + 3] = v;

    // Increase the count
    count += 4;
}

// Flush
void SpriteBatch::Flush(sf::RenderTarget* _target) {
	// Draw the vertices as quads.
	sf::RenderStates state = sf::RenderStates::Default;
	state.texture = texture;
	_target->draw(vertices, count, sf::PrimitiveType::Quads, state);

	// Reset the count
	count = 0;
}