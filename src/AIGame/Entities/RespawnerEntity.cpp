#include <AIGame/Entities/RespawnerEntity.hpp>
#include <AIGame/Game.hpp>

// Constructors
RespawnerEntity::RespawnerEntity(const std::string& _name) :
	Entity(_name),
	spawned(NULL),
	count(-1) {
}
RespawnerEntity::~RespawnerEntity() {
}

// Update
void RespawnerEntity::Update(Game* _game, float _delta) {
	if (spawned != NULL) {
		if (spawned->GetAlive() == false) {
			spawned = NULL;
		}
	}

	if (spawned == NULL) {
		Respawn(_game);
		count++;
	}
}

// Respawn
void RespawnerEntity::Respawn(Game* _game) {
}

// Getcount
int RespawnerEntity::GetCount() {
	return count;
}