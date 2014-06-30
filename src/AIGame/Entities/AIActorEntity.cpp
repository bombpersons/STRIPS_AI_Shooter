#include <AIGame/Entities/AIActorEntity.hpp>
#include <AIGame/Entities/ActorEntity.hpp>
#include <AIGame/PathFinding/PathFinder.hpp>
#include <AIGame/Game.hpp>
#include <AIGame/GOAP/State/WorldState.hpp>
#include <iostream>
#include <string>
#include <Misc/VectorMaths.hpp>
#include <AIGame/Entities/PlayerRespawnerEntity.hpp>

// Constructors
AIActorEntity::AIActorEntity(const std::string& _name, const std::string& _texture, PlayerRespawnerEntity* _spawner, const WeaponDesc& _weapon, bool _camerafollow) 
	: ActorEntity(_name, _texture, _spawner, _weapon, _camerafollow) {
	
	SetMaxSpeed(128);
}

AIActorEntity::~AIActorEntity() {
}

// Update
void AIActorEntity::Update(Game* _game, float _delta) {
	ActorEntity::Update(_game, _delta);
	
	// Generate plans

	// Just go for killing the player
	WorldState plan;

	// Get the closet enemy
	std::vector<ActorEntity*> enemies = _game->GetMap()->GetEntityList()->GetEntities<ActorEntity>();
	float lowestdist = 999999999999999.0f;
	ActorEntity* closestactor = NULL;
	for (int i = 0; i < enemies.size(); ++i) {
		if (enemies[i] == this)
			continue;

		float dist = VectorLength(GetPosition() - enemies[i]->GetPosition());
		if (dist < lowestdist) {
			lowestdist = dist;
			closestactor = enemies[i];
		}
	}

	if (closestactor == NULL)
		return;
	if (lowestdist < 64.0f) {
		// MAKE US RUN AWAY FROM THEM
		plan.SetData(WorldState::RUNNINGAWAY, WorldState::Data(true));
	} else {
		if (GetHealth() < 0.3f) {
			plan.SetData(WorldState::LOWHEALTH, WorldState::Data(false));
		} else {
			plan.SetData(WorldState::PLAYERALIVE, WorldState::Data(false));
		}
		if (GetHealth() < 0.8f) {
			plan.SetData(WorldState::UNDERFIRE, WorldState::Data(false));
		}
	}

	// Follow the plan
	FollowPlan(_game, closestactor, _delta, plan);
}

	// Draw
void AIActorEntity::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	ActorEntity::Draw(_game, _delta, _target);
}
