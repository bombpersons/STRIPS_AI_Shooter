#include <AIGame/Entities/BulletTrailEntity.hpp>
#include <AIGame/Game.hpp>

// Constructors
BulletTrailEntity::BulletTrailEntity(const std::string& _name, ActorEntity* _owner, const sf::Vector2f& _start, const sf::Vector2f& _end, float _staytime) :
	Entity(_name),
	timeleft(_staytime),
	totaltime(_staytime),
	line(sf::Lines, 2) {

	start = _start;
	end = _end;
	owner = _owner;

	// Create a line from the start to the end.
	line[0] = sf::Vertex(_start, sf::Color::White);
	line[1] = sf::Vertex(_end, sf::Color(255, 255, 255, 50));
}

BulletTrailEntity::~BulletTrailEntity() {
}

// Update
void BulletTrailEntity::Update(Game* _game, float _delta) {
	// Count down the timer.
	timeleft -= _delta;
	if (timeleft < 0) {
		// Kill ourselves.
		SetAlive(false);
	}

	// Fade the line out as time goes.
	line[0].color = sf::Color(255, 255, 255, (sf::Uint8)(255.0f * (timeleft / totaltime)));
	line[1].color = sf::Color(255, 255, 255, (sf::Uint8)(50.0f * (timeleft / totaltime)));
}

// Draw
void BulletTrailEntity::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	// Draw the lines
	_target->draw(line);
}

// Get the start and end
const sf::Vector2f& BulletTrailEntity::GetStart() {
	return start;
}
const sf::Vector2f& BulletTrailEntity::GetEnd() {
	return end;
}
ActorEntity* BulletTrailEntity::GetOwner() {
	return owner;
}