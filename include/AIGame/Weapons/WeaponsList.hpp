#ifndef AI_WEAPON_HPP
#define AI_WEAPON_HPP

#include <string>
#include <unordered_map>

#include <AIGame/Macros.hpp>

struct WeaponDesc {
	float dmgperbullet; // How much dmg a single bullet does.
	float dmgrangedropoff; // How far it takes for damage to drop.
	float dmgperbulletlongrange; // Long range damage.
	float minimumaccuracy; // The minimum accuracy arc.
	float accuracydecreasemovment; // The amount of accuracy decrease as an actor moves.
								   // accuracy += fraction of maxspeed  * accuracydecreasemovement * delta
	float accuracydecreasepershot; // The amount of accuracy decrease per shot of gun.
	float accuracydecreaserotation; // The mount of accuracy that is lost by rotating.
	float accuracyincreasepersecond; // The amount of accuracy increased per second
									 // accuracy -= accuracyincreasepersecond * delta
	float fireperiod; // Time between shots.
	int bulletspershot; // How many bullets are fired every shot.
	bool automatic; // Whether or not holding the trigger down continues to shoot.
	float range; // How far bullets can travel.
	float knockback; // How much the weapon knocks you back (impulse)
	float ammorechargepersecond; // How much ammo recharges per second
	int maxammo; // The max amount of ammo
	bool canShoot; // Whether or not this can shoot.
	int rangeguess; // A guess at the reccomended usage of this gun (for ai)
};

class WeaponsList {
public:
	// Constructors
	WeaponsList();
	virtual ~WeaponsList();

	// Add a weapon.
	void AddWeapon(const WeaponDesc& _desc, const std::string& _name);

	// Get a weapon.
	const WeaponDesc* GetWeapon(const std::string& _name);

	// Add default weapons
	void AddDefaultWeapons();

private:
	// Map of the weapons.
	std::unordered_map<std::string, WeaponDesc> weapons;

	// Make it so we can't copy.
	UNCOPYABLE(WeaponsList)
};

#endif