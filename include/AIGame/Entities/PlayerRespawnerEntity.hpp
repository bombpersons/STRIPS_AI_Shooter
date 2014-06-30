#ifndef AI_PLAYERRESPAWNERENTITY_HPP
#define AI_PLAYERRESPAWNERENTITY_HPP

#include <AIGame/Entities/RespawnerEntity.hpp>

class PlayerRespawnerEntity : public RespawnerEntity {
public:
	// Constructors
	PlayerRespawnerEntity(const std::string& _name, bool _ai);
	virtual ~PlayerRespawnerEntity();

	// Update
	virtual void Update(Game* _game, float _delta);

	// Respawn
	virtual void Respawn(Game* _game);

	// Get ai
	bool GetAI();

	// Get kill count
	int GetKillCount();
	void SetKillCount(int _count);
	
private:
	bool ai;
	int killcount;
};

#endif