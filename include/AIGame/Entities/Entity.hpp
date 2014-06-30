#ifndef AI_ENTITY_HPP
#define AI_ENTITY_HPP

#include <AIGame/Macros.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>

// Forward declare game class so that we don't have any dependancy loops.
class Game;

class Entity {
public:
	// Constructor
	Entity(const std::string& _name);
	virtual ~Entity();

	// New map
	virtual void MapChange(Game* _game);

	// Draw the entity
	virtual void Draw(Game* _game, float _delta, sf::RenderTarget* _target);

	// Update
	virtual void Update(Game* _game, float _delta);

	// Get / Set position, etc
	void SetPosition(const sf::Vector2f& _pos);
	void SetPositionX(float _x);
	void SetPositionY(float _y);
	void SetScale(const sf::Vector2f& _scale);
	void SetScaleX(float _x);
	void SetScaleY(float _y);
	void SetRotation(float _rot);

	sf::Vector2f GetPosition();
	float GetPositionX();
	float GetPositionY();
	sf::Vector2f GetScale();
	float GetScaleX();
	float GetScaleY();
	float GetRotation();

	void SetName(const std::string& _name);
	const std::string& GetName();

	virtual void SetAlive(bool _v);
	bool GetAlive();

private:
	// Location and Rotation and Scale
	sf::Vector2f position;
	sf::Vector2f scale;
	float rotation;

	// Name of the entity.
	std::string name;

	// A flag for removing entities
	bool alive;

	// Make this class uncopyable.
	UNCOPYABLE(Entity)
};

#endif