#ifndef AI_HUD_HPP
#define AI_HUD_HPP

#include <AIGame/Entities/ActorEntity.hpp>

class HUD {
public:
	// Constructors
	HUD(sf::Font* _font);
	virtual ~HUD();

	// Update
	virtual void Update(Game* _game, float _delta);

	// Draw
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

private:
	sf::Font* font;
};

#endif