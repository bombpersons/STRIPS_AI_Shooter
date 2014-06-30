#ifndef AI_IMMUTABLEWORLDSTATE_HPP
#define AI_IMMUTABLEWORLDSTATE_HPP

#include <SFML/Graphics.hpp>

struct ImmutableWorldState {
	ImmutableWorldState() {
		weaponcount = 0;
		medkitcount = 0;
	}

	// Classes
	struct WeaponInfo {
		sf::Vector2i pos;
		int range;
	};

	// Positions of interest
	WeaponInfo weapons[30];
	int weaponcount;
	sf::Vector2i medkits[30];
	int medkitcount;
	sf::Vector2i cover[30];
	int covercount;
	float health;
};

#endif