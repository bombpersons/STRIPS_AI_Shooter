#ifndef AI_PLAYERENTITY_HPP
#define AI_PLAYERENTITY_HPP

#include <AIGame/Entities/ActorEntity.hpp>

class PlayerEntity : public ActorEntity {
public:
	// Constructors
	PlayerEntity(const std::string& _name, const std::string& _texture, PlayerRespawnerEntity* _spawner, const WeaponDesc& _weapon, bool _camerafollow);
	virtual ~PlayerEntity();

	// Update
	virtual void Update(Game* _game, float _delta);

private:

};

#endif