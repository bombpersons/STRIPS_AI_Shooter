#include <AIGame/Entities/WeaponPickupEntity.hpp>
#include <AIGame/Game.hpp>

// Constructors
WeaponPickupEntity::WeaponPickupEntity(const std::string& _name, const std::string& _texture, const std::string& _weapon, float _respawntime) :
	PickupEntity(_name, _texture, _respawntime),
	weaponname(_weapon) {
}
WeaponPickupEntity::~WeaponPickupEntity() {
}

// Pickup
void WeaponPickupEntity::Pickup(Game* _game, ActorEntity* _actor) {
	_actor->GiveGun(_game, weaponname);
}

const std::string& WeaponPickupEntity::GetWeaponName() {
	return weaponname;
}