#ifndef AI_ACTORENTITY_HPP
#define AI_ACTORENTITY_HPP

#include <AIGame/Entities/SpriteEntity.hpp>
#include <AIGame/Weapons/WeaponsList.hpp>
#include <AIGame/PathFinding/ConcurrentPathFinder.hpp>
#include <AIGame/GOAP/Planner.hpp>

class PlayerRespawnerEntity;

class ActorEntity : public SpriteEntity {
public:
	// Constructors
	ActorEntity(const std::string& _name, const std::string& _texture, PlayerRespawnerEntity* _spawner, const WeaponDesc& _weapon, bool _camerafollow);
	virtual ~ActorEntity();
	
	// Draw
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

	// Update
	virtual void Update(Game* _game, float _delta);

	// Control the actor
	void Accelerate(const sf::Vector2f& _dir, float _percentageAccel, float _delta);
	void SetVelocity(const sf::Vector2f& _dir, float _precentageMaxSpeed);
	void Aim(float _angle);
	bool Goto(Game* _game, const sf::Vector2f& _pos, float _delta, float _percentagespeed, float _threshold);
	void FollowPlan(Game* _game, ActorEntity* _enemy, float _delta, const WorldState& _state);

	// Let the actor shoot.
	void ShootAuto(Game* _game);
	void ShootManual(Game* _game);
	void Shoot(Game* _game);

	// Check we are guaranteed to hit an actor if we shoot
	bool WillHitActor(Game* _game, ActorEntity* _actor, float _accuracyPercentage);

	// Give the actor a gun
	void GiveGun(Game* _game, const std::string& _name);

	// Take damage
	bool TakeDamage(float _damage);
	void Heal(float _heal);

	// Get vars from this actor
	int GetAmmo();
	int GetMaxAmmo();
	const std::string& GetWeaponName();
	float GetRadius();
	float GetHealth();
	std::vector<sf::Vector2f>* GetPath();
	const sf::Vector2f& GetVelocity();
	std::vector<Action>* GetPlan();
	bool IsInvincible();
	bool IsFollowed();

	// Set things
	void SetMaxSpeed(float _speed);
	void SetDrawPath(bool _value);

protected:
	// Invincibility timer
	float invincible;
	bool camerafollow;
	
	// velocity / friction
	sf::Vector2f velocity;
	float friction;
	float acceleration;
	float maxspeed;

	// Dimensions
	float radius;

	// Stats
	float health;

	// Current weapon
	WeaponDesc weapon;
	std::string weaponname;
	float timesincelastshot;
	float ammo;

	// The accuracy cone.
	float accuracy;
	bool drawAccuracyCone;
	float accuracyConeSize;

	// Path that we are following
	std::vector<sf::Vector2f> path;
	sf::Vector2i goal;
	ConcurrentPathFinder pathfinder;
	bool drawpath; // Whether or not to draw the path

	// Planning functions
	WorldState GetWorldState(Game* _game, ActorEntity* _enemy);
	ImmutableWorldState GetImmutableWorldState(Game* _game, ActorEntity* _enemy);

	// Plan that we are following
	std::vector<Action> plan;
	Planner planner;
	WorldState goalstate;
	WorldState prevworldstate;
	ImmutableWorldState previousimmutableworldstate;
	bool planning;

	// Parent spawner
	PlayerRespawnerEntity* parent;
};

#endif