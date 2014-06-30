#ifndef AI_BULLETTRAILENTITY_HPP
#define AI_BULLETTRAILENTITY_HPP

#include <AIGame/Entities/Entity.hpp>
#include <AIGame/Entities/ActorEntity.hpp>

class BulletTrailEntity : public Entity {
public:
	// Constructors
	BulletTrailEntity(const std::string& _name, ActorEntity* _owner, const sf::Vector2f& _start, const sf::Vector2f& _end, float _staytime);
	virtual ~BulletTrailEntity();

	// Update
	virtual void Update(Game* _game, float _delta);

	// Draw
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

	// Get the start and end of the line
	const sf::Vector2f& GetStart();
	const sf::Vector2f& GetEnd();
	ActorEntity* GetOwner();

private:
	// Vars
	float totaltime;
	float timeleft;
	sf::VertexArray line;
	sf::Vector2f start, end;
	ActorEntity* owner;
};

#endif