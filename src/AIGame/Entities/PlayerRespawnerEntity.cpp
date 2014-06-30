#include <AIGame/Entities/PlayerRespawnerEntity.hpp>
#include <AIGame/Entities/PlayerEntity.hpp>
#include <AIGame/Entities/AIActorEntity.hpp>
#include <AIGame/Game.hpp>
#include <AIGame/Map/Map.hpp>

// Constructors
PlayerRespawnerEntity::PlayerRespawnerEntity(const std::string& _name, bool _ai) :
	RespawnerEntity(_name),
	ai(_ai),
	killcount(0) {
}

PlayerRespawnerEntity::~PlayerRespawnerEntity() {
}

// Update
void PlayerRespawnerEntity::Update(Game* _game, float _delta) {
	if (spawned != NULL)
		if (((ActorEntity*)spawned)->GetHealth() <= 0) {
			spawned->SetAlive(false);

			{
				// Spawn some particles
				ParticleManager::Particle particle;
				particle.position = spawned->GetPosition();
				particle.size = sf::Vector2f(2, 2);
				particle.sizeincrease = sf::Vector2f(2.0f, 2.0f);
				particle.alphachange = -(1.0f / particle.lifetime);
				particle.color = sf::Color(255, 10, 10, 255);

				_game->GetParticles()->ReleaseParticlesRandomVelocity(particle,
																	  10.0f, 
																	  20.0f,
																	  sf::Vector2f(0, 1),
																	  -M_PI, M_PI,
																	  32.0f, 512.0f);
			}
			{
				ParticleManager::Particle particle;
				particle.position = spawned->GetPosition();
				particle.size = sf::Vector2f(5, 5);
				particle.sizeincrease = sf::Vector2f(20.0f, 20.0f);
				particle.lifetime = 4.0f;
				particle.color = sf::Color(255, 10, 10, 255);
				particle.alphachange = -(1.0f / particle.lifetime);

				// Release the particles
				_game->GetParticles()->AddParticle(particle);
			}
		}

	RespawnerEntity::Update(_game, _delta);
}

// Respawn
void PlayerRespawnerEntity::Respawn(Game* _game) {
	// Get the default weapon
	WeaponDesc weapon = *_game->GetMap()->GetWeaponList()->GetWeapon("pistol");

	if (!ai) {
		spawned = new AIActorEntity("player", "data/images/humanplayer.png", this, weapon, true);
	} else {
		spawned = new AIActorEntity("ai", "data/images/aiplayer.png", this, weapon, false);
	}
	spawned->SetPosition(_game->GetMap()->GetWorldPos(_game->GetMap()->GetClearTile()));
	_game->GetMap()->GetEntityList()->AddEntity(spawned);

	{
		// Spawn some particles
		ParticleManager::Particle particle;
		particle.position = spawned->GetPosition();
		particle.size = sf::Vector2f(20, 20);
		particle.sizeincrease = -sf::Vector2f(10.0f, 10.0f);
		particle.alphachange = -(1.0f / particle.lifetime);
		particle.color = sf::Color(200, 200, 200, 255);

		_game->GetParticles()->ReleaseParticlesRandomVelocity(particle,
																10.0f, 
																20.0f,
																sf::Vector2f(0, 1),
																-M_PI, M_PI,
																32.0f, 200.0f);
	}
	{
		ParticleManager::Particle particle;
		particle.position = spawned->GetPosition();
		particle.size = sf::Vector2f(5, 5);
		particle.sizeincrease = sf::Vector2f(80.0f, 80.0f);
		particle.lifetime = 1.0f;
		particle.color = sf::Color(200, 200, 200, 255);
		particle.alphachange = -(1.0f / particle.lifetime);

		// Release the particles
		_game->GetParticles()->AddParticle(particle);
	}
}

// Get whether or not it's ai
bool PlayerRespawnerEntity::GetAI() {
	return ai;
}

// Kill count
int PlayerRespawnerEntity::GetKillCount() {
	return killcount;
}
void PlayerRespawnerEntity::SetKillCount(int _count) {
	killcount = _count;
}