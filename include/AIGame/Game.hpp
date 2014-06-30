#ifndef AI_GAME_HPP
#define AI_GAME_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <AIGame/Map/Map.hpp>
#include <AIGame/Macros.hpp>
#include <AIGame/Camera.hpp>
#include <AIGame/Input/InputManager.hpp>
#include <AIGame/HUD/HUD.hpp>
#include <AIGame/Resource/ResourceManager.hpp>
#include <AIGame/Particles/ParticleManager.hpp>

#include <string>

class Game {
public:
	// Constructors
	Game(int _width, int _height, const std::string& _title, sf::Uint32 _style=sf::Style::Default, sf::ContextSettings _settings=sf::ContextSettings());
	virtual ~Game();

	// Update
	virtual void Update(float _delta);

	// Draw 
	virtual void Draw(float _delta);

	// When the window is resized.
	virtual void Resized(int _width, int _height);

	// Poll
	void PollEvents();

	// Run
	void Run();

	// Get / Set Map
	Map* GetMap();
	void SetMap(Map* _map);

	// Set the hud
	void SetHUD(HUD* _hud);

	// Get fps
	float GetFPS();

	// Grab the camera
	Camera* GetCamera();

	// Get the window
	sf::RenderWindow* GetWindow();

	// Get the input manager.
	InputManager* GetInput();

	// Get the resource manager
	ResourceManager* GetResources();

	// Get the particle manager.
	ParticleManager* GetParticles();

private:
	// The current map
	Map* map;

	// The current HUD
	HUD* hud;

	// The particle manager
	ParticleManager particles;

	// Resource manager
	ResourceManager resources;

	// A timer for calculating deltatime
	sf::Clock clock;

	// The window
	sf::RenderWindow window;

	// Input manager
	InputManager input;

	// The camera
	Camera camera;

	// For calculating average frame rate
	int framecount;
	float frametimer;
	float averagefpsperiod;
	float averagefps;

	// Max delta
	float maxDelta;

	// Make it so we can't accidentally copy this class.
	UNCOPYABLE(Game)
};

#endif