#include <AIGame/Entities/HealthPickupEntity.hpp>
#include <AIGame/Game.hpp>

// Constructors
HealthPickupEntity::HealthPickupEntity(const std::string& _name, const std::string& _texture, float _respawntime) :
	PickupEntity(_name, _texture, _respawntime) {
}
HealthPickupEntity::~HealthPickupEntity() {
}

// Pickup
void HealthPickupEntity::Pickup(Game* _game, ActorEntity* _actor) {
	_actor->Heal(1.0f);
}