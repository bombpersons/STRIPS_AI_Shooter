#include <AIGame/Weapons/WeaponsList.hpp>

// Constructors
WeaponsList::WeaponsList() {
	AddDefaultWeapons();
}

WeaponsList::~WeaponsList() {
}

// Add a weapon
void WeaponsList::AddWeapon(const WeaponDesc& _desc, const std::string& _name) {
	weapons[_name] = _desc;
	weapons[_name].canShoot = true;
}

// Get a weapon
const WeaponDesc* WeaponsList::GetWeapon(const std::string& _name) {
	// Try and find that weapon in the map.
	std::unordered_map<std::string, WeaponDesc>::iterator it = weapons.find(_name);
	if (it == weapons.end())
		return NULL;
	return &(it->second);
}

void WeaponsList::AddDefaultWeapons() {
	// Create default weapons
	WeaponDesc pistol;
	pistol.dmgperbullet = 0.1f;
	pistol.dmgrangedropoff = 200.0f;
	pistol.dmgperbulletlongrange = 0.2f;
	pistol.minimumaccuracy = 0.025f;
	pistol.accuracydecreasemovment = 0.4f;
	pistol.accuracydecreasepershot = 0.05f;
	pistol.accuracydecreaserotation = 0.1f;
	pistol.accuracyincreasepersecond = 0.1f;
	pistol.bulletspershot = 1;
	pistol.fireperiod = 0.2f;
	pistol.range = 800;
	pistol.automatic = false;
	pistol.knockback = 2.0f;
	pistol.ammorechargepersecond = 0.0f;
	pistol.maxammo = 16;
	pistol.rangeguess = 2;
	AddWeapon(pistol, "pistol");

	WeaponDesc shotgun;
	shotgun.dmgperbullet = 0.1f;
	shotgun.dmgrangedropoff = 150.0f;
	shotgun.dmgperbulletlongrange = 0.1f;
	shotgun.minimumaccuracy = 0.1f;
	shotgun.accuracydecreasemovment = 0.1f;
	shotgun.accuracydecreasepershot = 0.4f;
	shotgun.accuracydecreaserotation = 0.1f;
	shotgun.accuracyincreasepersecond = 0.2f;
	shotgun.bulletspershot = 8;
	shotgun.fireperiod = 0.7f;
	shotgun.range = 300;
	shotgun.automatic = false;
	shotgun.knockback = 64.0f;
	shotgun.ammorechargepersecond = 0.0f;
	shotgun.maxammo = 8.0f;
	shotgun.rangeguess = 1;
	AddWeapon(shotgun, "shotgun");

	WeaponDesc uzi;
	uzi.dmgperbullet = 0.08f;
	uzi.dmgrangedropoff = 200.0f;
	uzi.dmgperbulletlongrange = 0.2f;
	uzi.minimumaccuracy = 0.05f;
	uzi.accuracydecreasemovment = 0.4f;
	uzi.accuracydecreasepershot = 0.02f;
	uzi.accuracydecreaserotation = 0.1f;
	uzi.accuracyincreasepersecond = 0.2f;
	uzi.bulletspershot = 1;
	uzi.fireperiod = 0.1f;
	uzi.range = 600;
	uzi.automatic = true;
	uzi.knockback = 2.0f;
	uzi.ammorechargepersecond = 0.0f;
	uzi.maxammo = 32;
	uzi.rangeguess = 1;
	AddWeapon(uzi, "uzi");

	WeaponDesc sniper;
	sniper.dmgperbullet = 0.5f;
	sniper.dmgrangedropoff = 2000.0f;
	sniper.dmgperbulletlongrange = 0.2f;
	sniper.minimumaccuracy = 0.001f;
	sniper.accuracydecreasemovment = 1.0f;
	sniper.accuracydecreasepershot = 0.1f;
	sniper.accuracydecreaserotation = 1.0f;
	sniper.accuracyincreasepersecond = 0.05f;
	sniper.bulletspershot = 1;
	sniper.fireperiod = 0.7f;
	sniper.range = 3000.0f;
	sniper.automatic = false;
	sniper.knockback = 30.0f;
	sniper.ammorechargepersecond = 0.0f;
	sniper.maxammo = 8;
	sniper.rangeguess = 3;
	AddWeapon(sniper, "sniper");
}