#include <AIGame/Entities/Entity.hpp>
#include <AIGame/Game.hpp>

// Constructors
Entity::Entity(const std::string& _name) :
	position(0, 0),
	scale(1, 1),
	rotation(0),
	alive(true),
	name(_name) {
}

Entity::~Entity() {
}

// MapChange
void Entity::MapChange(Game* _game) {
}

// Draw
void Entity::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
}

// Update
void Entity::Update(Game* _game, float _delta) {
}

// Set's / Get's
void Entity::SetPosition(const sf::Vector2f& _pos) {
	position = _pos;
}
void Entity::SetPositionX(float _x) {
	position.x = _x;
}
void Entity::SetPositionY(float _y) {
	position.y = _y;
}
void Entity::SetScale(const sf::Vector2f& _scale) {
	scale = _scale;
}
void Entity::SetScaleX(float _x) {
	scale.x = _x;
}
void Entity::SetScaleY(float _y) {
	scale.y = _y;
}
void Entity::SetRotation(float _rot) {
	rotation = _rot;
}

sf::Vector2f Entity::GetPosition() {
	return position;
}
float Entity::GetPositionX() {
	return position.x;
}
float Entity::GetPositionY() {
	return position.y;
}
sf::Vector2f Entity::GetScale() {
	return scale;
}
float Entity::GetScaleX() {
	return scale.x;
}
float Entity::GetScaleY() {
	return scale.y;
}
float Entity::GetRotation() {
	return rotation;
}

void Entity::SetName(const std::string& _name) {
	name = _name;
}
const std::string& Entity::GetName() {
	return name;
}

void Entity::SetAlive(bool _v) {
	alive = _v;
}
bool Entity::GetAlive() {
	return alive;
}