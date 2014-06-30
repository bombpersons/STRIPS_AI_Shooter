#include <AIGame/Game.hpp>

#include <sstream>

// Constructors
Game::Game(int _width, int _height, const std::string& _title, sf::Uint32 _style, sf::ContextSettings _settings) :
	window(sf::VideoMode(_width, _height), _title, _style, _settings),
	clock(),
	map(NULL),
	hud(NULL),
	camera(),
	framecount(0),
	frametimer(0),
	averagefpsperiod(1.0f),
	maxDelta(1.0f / 60.0f) {
}

Game::~Game() {
}

// Update 
void Game::Update(float _delta) {
	// Increase the frame count
	framecount++;
	frametimer += _delta;
	if (frametimer > averagefpsperiod) {
		averagefps = 1.0f / (frametimer / framecount);
		frametimer = 0;
		framecount = 0;
	}

	// If there is a map, update it.
	if (map)
		map->Update(this, _delta);

	// Update the hud
	if (hud)
		hud->Update(this, _delta);

	// Update particles
	particles.Update(_delta);

	// Set the title to the frame rate
	std::stringstream title;
	title << "FPS: " << 1.0f / _delta << std::endl;
	window.setTitle(title.str());
}

// Draw
void Game::Draw(float _delta) {
	// Set the view
	window.setView(camera.GetView());
	
	// Clear
	window.clear(sf::Color::Blue);

	// If there is a map, draw it.
	if (map)
		map->Draw(this, _delta, &window);

	// If there is a hud, draw it
	if (hud)
		hud->Draw(this, _delta, &window);

	// Display window
	window.display();
}

// When the window is resized
void Game::Resized(int _width, int _height) {
	// Try and maintain aspect ratio.
	GetCamera()->SetSizeX((float)_width);
	GetCamera()->SetSizeY((float)_height);
}

// Pol l
void Game::PollEvents() {
	// Update the input manager.
	input.UpdateStart();

	sf::Event event;
	while (window.pollEvent(event)) {
		if ((event.type == sf::Event::Closed)) {
			window.close();
			break;
		}

		if (event.type == sf::Event::Resized) {
			Resized(event.size.width, event.size.height);
		}

		// Send events to input manager.
		input.UpdateEvent(event);
	}
}

// Run
void Game::Run() {
	while (window.isOpen()) {
		// Get the delta time
		float deltatime = clock.getElapsedTime().asSeconds();
		clock.restart();

		// Minimum delta
		if (deltatime > maxDelta)
			deltatime = maxDelta;

		// Update
		Update(deltatime);

		// Draw
		Draw(deltatime);

		// Poll Events
		PollEvents();
	}
}

// Get / Set Map
Map* Game::GetMap() {
	return map;
}
void Game::SetMap(Map* _map) {
	map = _map;
}

// Set the hud
void Game::SetHUD(HUD* _hud) {
	hud = _hud;
}

// Get fps
float Game::GetFPS() {
	return averagefps;
}

// Grab the camera
Camera* Game::GetCamera() {
	return &camera;
}

// Get the window
sf::RenderWindow* Game::GetWindow() {
	return &window;
}

// Get the input manager
InputManager* Game::GetInput() {
	return &input;
}

// Get the resource manager
ResourceManager* Game::GetResources() {
	return &resources;
}

// Get the particle manager.
ParticleManager* Game::GetParticles() {
	return &particles;
}