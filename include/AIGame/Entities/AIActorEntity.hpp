#ifndef AI_AIACTORENTITY_HPP
#define AI_AIACTORENTITY_HPP

#include <AIGame/Entities/ActorEntity.hpp>

class AIActorEntity : public ActorEntity {
public:
	// Constructors
	AIActorEntity(const std::string& _name, const std::string& _texture, PlayerRespawnerEntity* _spawner, const WeaponDesc& _weapon, bool _camerafollow);
	virtual ~AIActorEntity();

	// Update
	virtual void Update(Game* _game, float _delta);

	// Draw
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

private:
};

#endif