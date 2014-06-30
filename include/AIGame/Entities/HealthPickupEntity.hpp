#ifndef AI_HEALTHPICKUPENTITY_HPP
#define AI_HEALTHPICKUPENTITY_HPP

#include <AIGame/Entities/PickupEntity.hpp>

class HealthPickupEntity : public PickupEntity {
public:
	// Constructors
	HealthPickupEntity(const std::string& _name, const std::string& _texture, float _respawntime);
	virtual ~HealthPickupEntity();

	// Pickup
	virtual void Pickup(Game* _game, ActorEntity* _actor);

private:

};

#endif