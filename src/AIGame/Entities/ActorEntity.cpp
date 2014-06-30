#include <AIGame/Entities/ActorEntity.hpp>
#include <AIGame/Game.hpp>
#include <AIGame/Entities/BulletTrailEntity.hpp>
#include <AIGame/PathFinding/PathFinder.hpp>
#include <AIGame/Entities/PlayerEntity.hpp>
#include <AIGame/Entities/WeaponPickupEntity.hpp>
#include <AIGame/Entities/HealthPickupEntity.hpp>
#include <AIGame/Entities/PlayerRespawnerEntity.hpp>

#include <Misc/Geometry.hpp>
#include <Misc/VectorMaths.hpp>
#include <Misc/Random.hpp>
#include <Misc/Misc.hpp>

#include <SFML/System.hpp>

#include <iostream>

// Constructors
ActorEntity::ActorEntity(const std::string& _name, const std::string& _texture, PlayerRespawnerEntity* _spawner, const WeaponDesc& _weapon, bool _camerafollow) :
	SpriteEntity(_name, _texture),
	health(1),
	acceleration(1000),
	maxspeed(128),
	radius(10),
	friction(0.001f),
	timesincelastshot(0.0f),
	drawAccuracyCone(true),
	accuracyConeSize(32.0f),
	ammo(0),
	parent(_spawner),
	camerafollow(_camerafollow),
	drawpath(false) {

	// Zero the weapon
	weapon = _weapon;
	ammo = weapon.maxammo;
	
	// Invincibility
	invincible = 3.0f;

	// Debug
	if (camerafollow)
		drawpath = true;
}

ActorEntity::~ActorEntity() {
	// Wait for the path finder
	while (pathfinder.Finding()) {}
}

// Draw
void ActorEntity::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	// When we have low health.
	SetColor(sf::Color(255, 255.0f * (health), 255.0f * (health), 255.0f));
	if (invincible > 0) {
		SetColor(sf::Color(255, 255, 255, 100));
	}

	SpriteEntity::Draw(_game, _delta, _target);

	// Draw the accuracy cone if we need to.
	if (drawAccuracyCone) {
		// The color to draw the cone.
		sf::Color color(255, 30, 30, 50); // Transparent red.

		// Create the array
		sf::VertexArray array(sf::PrimitiveType::Triangles, 3);
		array[0] = sf::Vertex(GetPosition(), color);
		
		// Calculate the other positions
		float anglerange = accuracy * M_PI;
		float minangle = GetRotation() - (anglerange / 2.0f);
		float maxangle = GetRotation() + (anglerange / 2.0f);

		// Calculate the vectors
		sf::Vector2f minvector = VectorOfAngle(minangle);
		sf::Vector2f maxvector = VectorOfAngle(maxangle);

		// Set the vertices
		array[1] = sf::Vertex(GetPosition() + minvector * accuracyConeSize, color);
		array[2] = sf::Vertex(GetPosition() + maxvector * accuracyConeSize, color);

		// Draw it
		_target->draw(array);
	}

	// Draw the path
	if (drawpath) {
		if (!pathfinder.Finding()) {
			pathfinder.DrawPath(_game->GetMap(), _target, &path);
		}

		// Draw cover positions
		sf::CircleShape shape(16);
		shape.setOrigin(shape.getRadius(), shape.getRadius());
		shape.setFillColor(sf::Color(255, 0, 0, 50));
		for (int i = 0; i < previousimmutableworldstate.covercount; ++i) {
			shape.setPosition(_game->GetMap()->GetWorldPos(previousimmutableworldstate.cover[i]));
			_target->draw(shape);
		}
	}
}

// Update
void ActorEntity::Update(Game* _game, float _delta) {
	SpriteEntity::Update(_game, _delta);

	// Dec invincibility
	invincible -= _delta;

	// Weapon timer
	timesincelastshot += _delta;

	// The accuracy of the weapon.
	if (weapon.canShoot) {
		accuracy += weapon.accuracydecreasemovment * (VectorLength(velocity) / maxspeed) * _delta;
		accuracy *= powf(weapon.accuracyincreasepersecond, _delta);
		if (accuracy < weapon.minimumaccuracy)
			accuracy = weapon.minimumaccuracy;
		if (accuracy > 1.0f)
			accuracy = 1.0f;
	}

	// Recharge ammo
	ammo += weapon.ammorechargepersecond * _delta;
	if (ammo > weapon.maxammo)
		ammo = weapon.maxammo;

	// Friction
	velocity *= powf(friction, _delta);

	// Limit speed if it is too fast.
	float tempmaxspeed = IsInvincible() ? maxspeed * 1.5 : maxspeed;
	if (VectorLength(velocity) > tempmaxspeed)
		velocity = VectorNormalize(velocity) * tempmaxspeed;

	// Move by the velocity.
	sf::Vector2f moved = GetPosition() + velocity * _delta;

	// Check for collision with the map.
	
	// Save the tilewidth and height to avoid calling functions too much
	int tilewidth = _game->GetMap()->GetTileWidth();
	int tileheight = _game->GetMap()->GetTileHeight();

	// Get a box of tiles that we could potentially be colliding with.
	sf::Vector2f minpos = moved - sf::Vector2f(radius, radius);
	sf::Vector2f maxpos = moved + sf::Vector2f(radius, radius);
	sf::Vector2i mintile, maxtile;
	mintile.x = (int)(minpos.x / tilewidth) - 1;
	mintile.y = (int)(minpos.y / tileheight) - 1;
	maxtile.x = (int)(maxpos.x / tilewidth) + 1;
	maxtile.y = (int)(maxpos.y / tileheight) + 1;

	// Loop through each of these
	for (int x = mintile.x; x <= maxtile.x; ++x) {
		for (int y = mintile.y; y <= maxtile.y; ++y) {
			// Check if the tile is collidable
			if (_game->GetMap()->IsTileCollidable(x, y)) {
				// Do seperate passes for each axis.

				// The limits of this tile.
				sf::Vector2f topleft((float)x * tilewidth, (float)y * tileheight);
				sf::Vector2f bottomright(topleft.x + tilewidth, topleft.y + tileheight);

				// The limits of the actor
				sf::Vector2f actortopleftx(minpos.x, GetPositionY() - radius);
				sf::Vector2f actorbottomrightx(maxpos.x, GetPositionY() + radius);
				sf::Vector2f actortoplefty(GetPositionX() - radius, minpos.y);
				sf::Vector2f actorbottomrighty(GetPositionX() + radius, maxpos.y);

				// Check for collision
				// RECTANGLE COLLISION
				if (RectangleIntersectRectangle(actortopleftx, actorbottomrightx, topleft, bottomright)) {
					moved.x = GetPositionX();
					velocity.x = 0; // Reset velocity on this axis.
					std::cout << "X" << std::endl;
				}
				if (RectangleIntersectRectangle(actortoplefty, actorbottomrighty, topleft, bottomright)) {
					moved.y = GetPositionY();
					velocity.y = 0; // Reset velocity on this axis.
					std::cout << "Y" << std::endl;
				}

				// RAYCAST COLLISION
				// TODO

				/* CIRCLE COLLISION
				// X Axis
				if (CircleIntersectsRectangle(sf::Vector2f(moved.x, GetPositionY()), radius, topleft, bottomright)) {
					moved.x = GetPositionX();
					std::cout << "X" << std::endl;
				}

				// Y Axis
				if (CircleIntersectsRectangle(sf::Vector2f(GetPositionX(), moved.y), radius, topleft, bottomright)) {
					moved.y = GetPositionY();
					std::cout << "Y" << std::endl;
				}
				*/
			}
		}
	}

	// Move by that amount
	SetPosition(moved);

	// Lock the camera to the player.
	if (camerafollow) {
		sf::Vector2f lookat = GetPosition();
		lookat = VectorLerp(_game->GetCamera()->GetPosition(), lookat, 16.0f * _delta);
		_game->GetCamera()->SetPosition(lookat);
	}
}

// Controls
void ActorEntity::Accelerate(const sf::Vector2f& _dir, float _percentageAccel, float _delta) {
	// Check if the dir is 0
	if (_dir.x == 0 && _dir.y == 0)
		return;

	// Normalize the direction
	float distance = sqrt(_dir.x * _dir.x + _dir.y * _dir.y);
	sf::Vector2f normalized;
	normalized.x = _dir.x / distance;
	normalized.y = _dir.y / distance;

	// Set our velocity
	velocity += normalized * _percentageAccel * acceleration * _delta;
}

void ActorEntity::SetVelocity(const sf::Vector2f& _dir, float _percentageMaxSpeed) {
	if (_dir.x == 0 && _dir.y == 0)
		return;

	velocity = VectorNormalize(_dir) * _percentageMaxSpeed * maxspeed;
}
void ActorEntity::Aim(float _angle) {
	// Get the difference between our current aim and this
	float diff = fabs(AngleDelta(GetRotation(), _angle));

	// Calculate the decrease in accuracy resulting from this re-aim.
	accuracy += (diff / M_PI) * weapon.accuracydecreaserotation;

	// Set our angle
	SetRotation(_angle);
}

// Goto a location
bool ActorEntity::Goto(Game* _game, const sf::Vector2f& _pos, float _delta, float _percentagespeed, float _threshold) {
	// Wait for the pathfinder to find the path
	if (pathfinder.Finding())
		return false;

	// Convert the position into tile position
	sf::Vector2i pos = _game->GetMap()->GetTilePos(_pos);

	// If we don't have a path, or the goal is different generate a new path
	if (path.size() == 0 || goal != pos) {
		// Grab enemy positions
		std::vector<ActorEntity*> actors = _game->GetMap()->GetEntityList()->GetEntities<ActorEntity>();
		std::vector<sf::Vector2f> positions;
		for (int i = 0; i < actors.size(); ++i) {
			if (actors[i] == this)
				continue;

			positions.push_back(actors[i]->GetPosition());
		}

		// Create new path
		path.clear();
		pathfinder.RequestPath(_game->GetMap(), GetPosition(), _pos, radius, &path, &positions);

		// Set the goal
		goal = pos;

		// Return and wait for the pathfinder to return path
		return false;
	} else if (path.size() == 1) {
		// Stop if we are at the goal
		if (VectorLength(GetPosition() - path[0]) < _threshold)
			return true;
	}

	// Get the next point to move to.
	sf::Vector2f point = path[path.size() - 1];
	if (path.size() >= 2) {
		// Get the perpendicular vector to the direction we are moving
		sf::Vector2f diff = VectorNormalize(GetPosition() - path[path.size() - 2]);
		sf::Vector2f normal(diff.y, -diff.x);

		// Check if we can pass to the next point.
		if (_game->GetMap()->Raycast(GetPosition() + normal * radius, path[path.size() - 2] + normal * radius) == path[path.size() - 2] + normal * radius &&
			_game->GetMap()->Raycast(GetPosition() - normal * radius, path[path.size() - 2] - normal * radius) == path[path.size() - 2] - normal * radius) {

			// Remove the first point
			path.erase(path.end() - 1);

			// Next point
			point = path[path.size() - 1];
		}
	}
	
	// Move towards it.
	Accelerate(point - GetPosition(), _percentagespeed, _delta);
	
	return false;
}

// Follow a plan
void ActorEntity::FollowPlan(Game* _game, ActorEntity* _enemy, float _delta, const WorldState& _state) {
	// Decide whether we need to re-plan
	WorldState state = GetWorldState(_game, _enemy);
	ImmutableWorldState immutablestate = GetImmutableWorldState(_game, _enemy);
	if (memcmp(state.data, prevworldstate.data, sizeof(state.data)) != 0 ||
		memcmp(_state.data, goalstate.data, sizeof(_state.data) != 0) ||
		memcmp(&immutablestate, &previousimmutableworldstate, sizeof(ImmutableWorldState)) != 0) {
		// Replan

		// Actually get a plan
		plan.clear();
		planner.GetPlan(immutablestate, state, _state, &plan);
	}
	prevworldstate = state;
	previousimmutableworldstate = immutablestate;
	goalstate = _state;

	// Get the first item
	if (plan.size() != 0) {
		Action action = plan[0];
		bool next = false;

		switch (action.GetActionId()) {
		case Action::GOTO:
		case Action::PICKUPWEAPON:
		case Action::PICKUPMEDKIT:
			if (Goto(_game, sf::Vector2f(action.GetPos().x * _game->GetMap()->GetTileWidth() + _game->GetMap()->GetTileWidth() / 2.0f,
										 action.GetPos().y * _game->GetMap()->GetTileHeight() + _game->GetMap()->GetTileHeight() / 2.0f), _delta, 1.0f, 6.0f)) {
				next = true;
			}

			if (_enemy) {
				// Aim at the player
				sf::Vector2f dir = (_enemy->GetPosition() - GetPosition());
				float dist = VectorLength(dir);
				Aim(AngleOfVector(dir));

				// If we have a line of sight to the player, try shooting them
				if (WillHitActor(_game, _enemy, 0.05f)) {
					Shoot(_game);
				}
			}

			break;
		case Action::TAKECOVER:
			if (Goto(_game, sf::Vector2f(action.GetPos().x * _game->GetMap()->GetTileWidth() + _game->GetMap()->GetTileWidth() / 2.0f,
										 action.GetPos().y * _game->GetMap()->GetTileHeight() + _game->GetMap()->GetTileHeight() / 2.0f), _delta, 1.0f, 6.0f)) {
				next = true;
			}
			break;
		case Action::RUNAWAY:
			// Go in the opposite direction of the player.
			Accelerate(VectorNormalize(GetPosition() - _enemy->GetPosition()), 1.0f, _delta);
			break;
		case Action::SHOOTENEMY:
			{
				if (_enemy) {
					// Aim at the player
					sf::Vector2f dir = (_enemy->GetPosition() - GetPosition());
					float dist = VectorLength(dir);
					Aim(AngleOfVector(dir));

					// If we have a line of sight to the player, try shooting them
					if (WillHitActor(_game, _enemy, 0.05f)) {
						Shoot(_game);
						if (dist > 32.0f)
							Goto(_game, _enemy->GetPosition(), _delta, 0.2f, 16.0f);
					} else {
						Goto(_game, _enemy->GetPosition(), _delta, 1.0f, 16.0f);
					}
				}
			}
			break;
		default:
			break;
		}

		// Get rid of first item if we finished it
		if (next) {
			plan.erase(plan.begin());
		}
	}
}

// Shoot the current weapon
void ActorEntity::ShootAuto(Game* _game) {
	// Check if this weapon can shoot as an auto
	if (!weapon.automatic)
		return;

	// Shoot
	Shoot(_game);
}
void ActorEntity::ShootManual(Game* _game) {
	Shoot(_game);
}
void ActorEntity::Shoot(Game* _game) {
	// Check if we can shoot
	if (timesincelastshot < weapon.fireperiod)
		return;
	if (!weapon.canShoot)
		return;
	if (ammo < 1)
		return;
	if (IsInvincible())
		return;

	timesincelastshot = 0;

	// Normalize
	sf::Vector2f dir = VectorNormalize(VectorOfAngle(GetRotation()));

	// Whether or not we killed with this shot
	bool kill = false;

	// Shoot all the bullets
	for (int i = 0; i < weapon.bulletspershot; ++i) {
		// Calculate a random angle
		float anglerange = accuracy * M_PI;
		float angle = Clamp(NormalDistribution(0.0f, anglerange / 8.0f), -anglerange / 2.0f, anglerange / 2.0f);
		dir = VectorRotate(dir, angle);

		// Do a ray trace and see if we hit anything.
		ActorEntity* hit = NULL;
		sf::Vector2i tilehit(-1, -1);
		sf::Vector2f maxrangepoint = GetPosition() + dir * weapon.range;
		sf::Vector2f endpoint = _game->GetMap()->Raycast(GetPosition() + dir * GetRadius() * 2.0f, maxrangepoint, &tilehit, &hit, this);

		// Do damage to the enemy if we hit one
		if (hit != NULL) {
			// Calculate the damage
			if (hit->TakeDamage(VectorLength(GetPosition() - endpoint) < weapon.dmgrangedropoff ? weapon.dmgperbullet : weapon.dmgperbulletlongrange)) {
				kill = true;
			}
		}

		// Create a bullet trail.
		BulletTrailEntity* trail = new BulletTrailEntity("BulletTrail", this, GetPosition() + dir * GetRadius() * 2.0f, endpoint, 1.0f);
		_game->GetMap()->GetEntityList()->AddEntity(trail);

		// Calculate the speed of the particles
		float rangeratio = 1.0f - VectorLength(GetPosition() - endpoint) / weapon.range;

		// Make a particles appear
		if (hit != NULL) {
			if (!hit->IsInvincible()) {
				// Blood splash
				{
					ParticleManager::Particle particle;
					particle.position = endpoint;
					particle.size = sf::Vector2f(2, 2);
					particle.alphachange = -(1.0f / particle.lifetime);
					particle.color = sf::Color(255, 10, 10, 255);

					_game->GetParticles()->ReleaseParticlesRandomVelocity(particle,
																		  rangeratio * 6.0f, 
																		  rangeratio * 14.0f,
																		  -dir,
																		  -M_PI/4.0f, M_PI/4.0f,
																		  rangeratio * 32.0f, rangeratio * 512.0f);
				}

				// Create a blood pool.
				{
					ParticleManager::Particle particle;
					particle.position = endpoint;
					particle.size = sf::Vector2f(1, 1);
					particle.sizeincrease = sf::Vector2f(10.0f, 10.0f);
					particle.lifetime = 4.0f;
					particle.color = sf::Color(255, 10, 10, 255);
					particle.alphachange = -(1.0f / particle.lifetime);

					// Release the particles
					_game->GetParticles()->AddParticle(particle);
				}
			} else {
					ParticleManager::Particle particle;
					particle.position = endpoint;
					particle.size = sf::Vector2f(2, 2);
					particle.alphachange = -(1.0f / particle.lifetime);
					particle.color = sf::Color(255, 255, 0, 255);

					_game->GetParticles()->ReleaseParticlesRandomVelocity(particle,
																		  rangeratio * 6.0f, 
																		  rangeratio * 14.0f,
																		  -dir,
																		  -M_PI/4.0f, M_PI/4.0f,
																		  rangeratio * 32.0f, rangeratio * 512.0f);
			}

		} else if (endpoint != maxrangepoint) { // Hit a tile
			ParticleManager::Particle particle;
			particle.position = endpoint;
			particle.size = sf::Vector2f(2, 2);
			particle.alphachange = -(1.0f / particle.lifetime);

			// Get the average color of the tile hit.
			particle.color = _game->GetMap()->GetColorAt(_game->GetMap()->GetCollisionLayer(tilehit.x, tilehit.y),
														 tilehit.x, tilehit.y,
														 sf::Vector2f(RandRange(0, _game->GetMap()->GetTileWidth()), RandRange(0, _game->GetMap()->GetTileHeight())));

			_game->GetParticles()->ReleaseParticlesRandomVelocity(particle,
																  rangeratio * 6.0f, 
																  rangeratio * 14.0f,
																  -dir,
																  -M_PI/4.0f, M_PI/4.0f,
																  rangeratio * 32.0f, rangeratio * 512.0f);
		}
	}

	// Inc kill count;
	if (parent && kill)
		parent->SetKillCount(parent->GetKillCount() + 1);

	// Decrease our accuracy
	accuracy += weapon.accuracydecreasepershot;

	// Apply knockback
	velocity += -dir * weapon.knockback;

	// Decrease ammo
	ammo -= 1.0f;
}

// Check we are guaranteed to hit an actor if we shoot
bool ActorEntity::WillHitActor(Game* _game, ActorEntity* _actor, float _accuracyPercentage) {
	// Get the most extreme shot angle vectors
	float anglerange = accuracy * M_PI;
	sf::Vector2f dir = VectorNormalize(VectorOfAngle(GetRotation()));
	sf::Vector2f min = VectorRotate(dir, (anglerange / 2.0f) * _accuracyPercentage);
	sf::Vector2f max = VectorRotate(dir, -(anglerange / 2.0f) * _accuracyPercentage);

	// Cast rays
	sf::Vector2f minend = GetPosition() + min * weapon.range;
	sf::Vector2f maxend = GetPosition() + max * weapon.range;
	ActorEntity* minactor = NULL;
	ActorEntity* maxactor = NULL;
	sf::Vector2i tilehit;
	sf::Vector2f minendpoint = _game->GetMap()->Raycast(GetPosition() + min * GetRadius() * 2.0f, minend, &tilehit, &minactor, this);
	sf::Vector2f maxendpoint = _game->GetMap()->Raycast(GetPosition() + max * GetRadius() * 2.0f, maxend, &tilehit, &maxactor, this);

	// If the actor was hit by both rays, then if you shoot you should hit the target.
	return (minactor == _actor && maxactor == _actor);
}

// Give the actor a gun
void ActorEntity::GiveGun(Game* _game, const std::string& _name) {
	// Try and find the gun in the weapon list.
	const WeaponDesc* desc = _game->GetMap()->GetWeaponList()->GetWeapon(_name);
	if (desc != NULL) {
		weaponname = _name;
		weapon = *desc;
		ammo = weapon.maxammo;
	}
}

// Take damage
bool ActorEntity::TakeDamage(float _damage) {
	if (invincible > 0)
		return false;

	health -= _damage;
	return (health <= 0);
}
void ActorEntity::Heal(float _heal) {
	health += _heal;
	if (health > 1.0f)
		health = 1.0f;
}

// Get some vars
int ActorEntity::GetAmmo() {
	return (int)ammo;
}

int ActorEntity::GetMaxAmmo() {
	return weapon.maxammo;
}

const std::string& ActorEntity::GetWeaponName() {
	return weaponname;
}

float ActorEntity::GetRadius() {
	return radius;
}

float ActorEntity::GetHealth() {
	return health;
}

std::vector<sf::Vector2f>* ActorEntity::GetPath() {
	return &path;
}

const sf::Vector2f& ActorEntity::GetVelocity() {
	return velocity;
}

std::vector<Action>* ActorEntity::GetPlan() {
	return &plan;
}

bool ActorEntity::IsInvincible() {
	return (invincible > 0);
}

bool ActorEntity::IsFollowed() {
	return camerafollow;
}

// Set
void ActorEntity::SetMaxSpeed(float _speed) {
	maxspeed = _speed;
}

void ActorEntity::SetDrawPath(bool _value) {
	drawpath = _value;
}

// Worldstate
WorldState ActorEntity::GetWorldState(Game* _game, ActorEntity* _enemy) {
	// Get the actors position
	sf::Vector2i actorpos = _game->GetMap()->GetTilePos(GetPosition());

	// Make the state.
	WorldState state;
	if (_enemy) {
		sf::Vector2i playerpos = _game->GetMap()->GetTilePos(_enemy->GetPosition());
		state.data[WorldState::PLAYERPOSITIONX].iValue = playerpos.x;
		state.data[WorldState::PLAYERPOSITIONY].iValue = playerpos.y;
	}
	state.data[WorldState::ACTORPOSITIONX].iValue = actorpos.x;
	state.data[WorldState::ACTORPOSITIONY].iValue = actorpos.y;
	state.data[WorldState::HASGUN].iValue = ammo > 0 ? 1 : 0;
	state.data[WorldState::PLAYERALIVE].iValue = _enemy != NULL ? 1 : 0;
	state.data[WorldState::LOWHEALTH].iValue = health < 0.3f ? 1 : 0;
	if (ammo > 0) {
		state.data[WorldState::SHORTRANGEGUN].iValue = weapon.rangeguess == 1 ? 1 : 0;
		state.data[WorldState::MEDIUMRANGEGUN].iValue = weapon.rangeguess == 2 ? 1 : 0;
		state.data[WorldState::LONGRANGEGUN].iValue = weapon.rangeguess == 3 ? 1 : 0;
	}

	// Check if we are under fire
	std::vector<BulletTrailEntity*> bullets = _game->GetMap()->GetEntityList()->GetEntities<BulletTrailEntity>();
	for (int i = 0; i < bullets.size(); ++i) {
		// See if any of these bullets are going near us
		if (bullets[i]->GetOwner() != this) {
			if (LineIntersectsCircle(bullets[i]->GetStart(), bullets[i]->GetEnd(), GetPosition(), 64) ||
				VectorLength(bullets[i]->GetStart() - GetPosition()) < 64 && VectorLength(bullets[i]->GetEnd() - GetPosition()) < 64) {
				state.data[WorldState::UNDERFIRE].iValue = 1;
			}
		}
	}

	return state;
}
ImmutableWorldState ActorEntity::GetImmutableWorldState(Game* _game, ActorEntity* _enemy) {
	// Create the state
	ImmutableWorldState state;

	// Get all pickups
	{
		std::vector<WeaponPickupEntity*> pickups = _game->GetMap()->GetEntityList()->GetEntities<WeaponPickupEntity>();
		std::vector<WeaponPickupEntity*> availablepickups;
		for (int i = 0; i < pickups.size(); ++i) {
			if (pickups[i]->IsAvailable())
				availablepickups.push_back(pickups[i]);
		}

		state.weaponcount = availablepickups.size();
		for (int i = 0; i < state.weaponcount; ++i) {
			state.weapons[i].pos = _game->GetMap()->GetTilePos(availablepickups[i]->GetPosition());
			state.weapons[i].range = _game->GetMap()->GetWeaponList()->GetWeapon(availablepickups[i]->GetWeaponName())->rangeguess;
		}
	}
	{
		// Get all pickups
		std::vector<HealthPickupEntity*> pickups = _game->GetMap()->GetEntityList()->GetEntities<HealthPickupEntity>();
		std::vector<HealthPickupEntity*> availablepickups;
		for (int i = 0; i < pickups.size(); ++i) {
			if (pickups[i]->IsAvailable())
				availablepickups.push_back(pickups[i]);
		}

		// Create the state
		state.medkitcount = availablepickups.size();
		for (int i = 0; i < state.medkitcount; ++i) {
			sf::Vector2i pos = _game->GetMap()->GetTilePos(availablepickups[i]->GetPosition());
			state.medkits[i].x = pos.x;
			state.medkits[i].y = pos.y;
		}
	}
	
	// Calculate points that we can get cover from
	// Grab enemy positions
	std::vector<ActorEntity*> actors = _game->GetMap()->GetEntityList()->GetEntities<ActorEntity>();
	std::vector<sf::Vector2f> positions;
	for (int i = 0; i < actors.size(); ++i) {
		if (actors[i] == this)
			continue;

		positions.push_back(actors[i]->GetPosition());
	}
	if (positions.size() != 0) {
		state.covercount = _game->GetMap()->GetCoverPoints(&positions, GetPosition(), 100, state.cover, 30);
	} else {
		state.covercount = 0;
	}

	// Set health
	state.health = health;

	return state;
}