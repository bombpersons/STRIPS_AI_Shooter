#ifndef AI_PICKUPENTITY_HPP
#define AI_PICKUPENTITY_HPP

#include <AIGame/Entities/SpriteEntity.hpp>
#include <AIGame/Entities/ActorEntity.hpp>

class PickupEntity : public SpriteEntity {
public:
	// Constructors
	PickupEntity(const std::string& _name, const std::string& _texture, float _respawntime=10.0f);
	virtual ~PickupEntity();

	// Update
	virtual void Update(Game* _game, float _delta);

	// Draw
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

	// When an actor picks us up.
	virtual void Pickup(Game* _game, ActorEntity* _actor);

	// Whether or not the pickup is available
	bool IsAvailable();

private:
	// How long to take to respawn.
	float respawntime;
	float spawntimer;

	// The pickup radius.
	float radius;
};

#endif