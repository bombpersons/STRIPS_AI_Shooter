#ifndef AI_WEAPONPICKUPENTITY_HPP
#define AI_WEAPONPICKUPENTITY_HPP

#include <AIGame/Entities/PickupEntity.hpp>

class WeaponPickupEntity : public PickupEntity {
public:
	// Constructors
	WeaponPickupEntity(const std::string& _name, const std::string& _texture, const std::string& _weapon, float _respawntime=10.0f);
	virtual ~WeaponPickupEntity();

	// Pickup
	virtual void Pickup(Game* _game, ActorEntity* _actor);

	// Get weapon name
	const std::string& GetWeaponName();

private:
	std::string weaponname;
};

#endif