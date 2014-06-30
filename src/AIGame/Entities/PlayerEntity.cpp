#include <AIGame/Entities/PlayerEntity.hpp>
#include <AIGame/Game.hpp>

#include <Misc/VectorMaths.hpp>

PlayerEntity::PlayerEntity(const std::string& _name, const std::string& _texture, PlayerRespawnerEntity* _spawner, const WeaponDesc& _weapon, bool _camerafollow) 
	: ActorEntity(_name, _texture, _spawner, _weapon, _camerafollow) {
}

PlayerEntity::~PlayerEntity() {
}

// Update
void PlayerEntity::Update(Game* _game, float _delta) {
	ActorEntity::Update(_game, _delta);

	// Check for keypresses and stuff.
	sf::Vector2f movedir;
	if (_game->GetInput()->KeyboardPressed(sf::Keyboard::W))
		movedir.y -= 1;
	if (_game->GetInput()->KeyboardPressed(sf::Keyboard::S))
		movedir.y += 1;
	if (_game->GetInput()->KeyboardPressed(sf::Keyboard::A))
		movedir.x -= 1;
	if (_game->GetInput()->KeyboardPressed(sf::Keyboard::D))
		movedir.x += 1;

	// How fast to move
	float moverate = 1.0f;
	if (_game->GetInput()->KeyboardPressed(sf::Keyboard::LShift))
		moverate = 0.3f;

	// Check for shooting
	sf::Vector2f direction = _game->GetWindow()->mapPixelToCoords(sf::Mouse::getPosition(*_game->GetWindow()));
	direction -= GetPosition();
	direction = VectorNormalize(direction);
	Aim(AngleOfVector(direction));

	if (_game->GetInput()->MousePressed(sf::Mouse::Button::Left)) {
		// Get the mouse position in world coords
		ShootAuto(_game);
	}
	if (_game->GetInput()->MouseJustPressed(sf::Mouse::Button::Left)) {
		ShootManual(_game);
	}

	// Move
	Accelerate(movedir, moverate, _delta);
}