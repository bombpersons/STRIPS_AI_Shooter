#ifndef AI_RESPAWNERENTITY_HPP
#define AI_RESPAWNERENTTIY_HPP

#include <AIGame/Entities/Entity.hpp>

class RespawnerEntity : public Entity {
public:
	// Constructors
	RespawnerEntity(const std::string& _name);
	virtual ~RespawnerEntity();

	// Call respawn if our instance is gone.
	virtual void Update(Game* _game, float _delta);

	// Respawn.
	virtual void Respawn(Game*_game);

	// Get count
	int GetCount();

protected:
	Entity* spawned;
	int count;

	// Kill and death count grid

};

#endif