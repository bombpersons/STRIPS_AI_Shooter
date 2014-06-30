#include <AIGame/Entities/PickupEntity.hpp>
#include <AIGame/Game.hpp>

#include <Misc/VectorMaths.hpp>

// Constructors
PickupEntity::PickupEntity(const std::string& _name, const std::string& _texture, float _respawntime) :
	SpriteEntity(_name, _texture),
	respawntime(_respawntime),
	spawntimer(_respawntime) {
}

PickupEntity::~PickupEntity() {
}

// Update
void PickupEntity::Update(Game* _game, float _delta) {
	// If we are currently not spawned don't do anything.
	spawntimer += _delta;
	if (spawntimer < respawntime)
		return;

	// Get the radius
	radius = _game->GetResources()->GetTexture(GetTexture())->getSize().x / 2;

	// Search for actors that could pick us up.
	std::vector<ActorEntity*> entities = _game->GetMap()->GetEntityList()->GetEntities<ActorEntity>();
	for (int i = 0; i < (int)entities.size(); ++i) {
		if (VectorLength(entities[i]->GetPosition() - GetPosition()) < radius) {
			Pickup(_game, entities[i]);

			// Dissapear until respawn
			spawntimer = 0;
		}
	}
}

void PickupEntity::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	if (spawntimer < respawntime)
		return;

	SpriteEntity::Draw(_game, _delta, _target);
}

// Pickup
void PickupEntity::Pickup(Game* _game, ActorEntity* _actor) {
}

// Whether or not the pickup is available
bool PickupEntity::IsAvailable() {
	return !(spawntimer < respawntime);
}