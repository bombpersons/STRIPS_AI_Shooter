#include <AIGame/Map/Map.hpp>
#include <AIGame/Game.hpp>

#include <AIGame/Entities/PlayerEntity.hpp>
#include <AIGame/Entities/PickupEntity.hpp>
#include <AIGame/HUD/HUD.hpp>

#include <AIGame/GOAP/Planner.hpp>

#include <iostream>

int main(int _argc, char** _argv) {
	std::cout << "Hey there world!" << std::endl;

	// Load fonts
	sf::Font font;
	font.loadFromFile("data/fonts/font.ttf");

	// Create a test map.
	Map map("data/levels/testmap.tmx");

	// Create a HUD
	HUD hud(&font);

	// Create a game
	Game game(500, 500, "AI Game");
	game.SetMap(&map);
	game.SetHUD(&hud);
	game.Run();

	return 0;
}