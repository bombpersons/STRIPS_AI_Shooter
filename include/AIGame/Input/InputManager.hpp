#ifndef AI_INPUTMANAGER_HPP
#define AI_INPUTMANAGER_HPP

#include <SFML/Window.hpp>

#include <AIGame/Macros.hpp>

class InputManager {
public:
	// Constructors
	InputManager();
	virtual ~InputManager();

	// Udpates
	void UpdateStart();
	void UpdateEvent(const sf::Event& _event);

	// JustPressed
	bool KeyboardJustPressed(sf::Keyboard::Key _key);
	bool MouseJustPressed(sf::Mouse::Button _button);

	// JustReleased
	bool KeyboardJustReleased(sf::Keyboard::Key _key);
	bool MouseJustReleased(sf::Mouse::Button _button);

	// IsPressed
	bool KeyboardPressed(sf::Keyboard::Key _key);
	bool MousePressed(sf::Mouse::Button _button);

private:
	// The state of keys and mousebuttons.
	struct InputState {
		bool keys[sf::Keyboard::KeyCount];
		bool mousebuttons[sf::Mouse::Button::ButtonCount];
	};
	InputState current;
	InputState previous;

	// Make sure we don't copy this thing.
	UNCOPYABLE(InputManager)
};

#endif