#include <AIGame/Input/InputManager.hpp>

// Constructors
InputManager::InputManager() {
	// Set all keys to being not pressed.
	memset(&current, 0, sizeof(current));
	memset(&previous, 0, sizeof(previous));
}

InputManager::~InputManager() {
}

// Tick
void InputManager::UpdateStart() {
	// Set the current to be the previous state
	memcpy(&previous, &current, sizeof(current));
}

// Update
void InputManager::UpdateEvent(const sf::Event& _event) {
	// Keyboard
	if (_event.type == sf::Event::KeyPressed)
		current.keys[_event.key.code] = true;
	if (_event.type == sf::Event::KeyReleased)
		current.keys[_event.key.code] = false;

	// Mousebuttons
	if (_event.type == sf::Event::MouseButtonPressed)
		current.mousebuttons[_event.key.code] = true;
	if (_event.type == sf::Event::MouseButtonReleased)
		current.mousebuttons[_event.key.code] = false;
}

// JustPressed
bool InputManager::KeyboardJustPressed(sf::Keyboard::Key _key) {
	return (current.keys[_key] && !previous.keys[_key]);
}
bool InputManager::MouseJustPressed(sf::Mouse::Button _button) {
	return (current.mousebuttons[_button] && !previous.mousebuttons[_button]);
}

// JustReleased
bool InputManager::KeyboardJustReleased(sf::Keyboard::Key _key) {
	return (!current.keys[_key] && previous.keys[_key]);
}
bool InputManager::MouseJustReleased(sf::Mouse::Button _button) {
	return (!current.mousebuttons[_button] && previous.mousebuttons[_button]);
}

// IsPressed
bool InputManager::KeyboardPressed(sf::Keyboard::Key _key) {
	return current.keys[_key];
}
bool InputManager::MousePressed(sf::Mouse::Button _button) {
	return current.mousebuttons[_button];
}