#include <AIGame/HUD/HUD.hpp>
#include <AIGame/Game.hpp>
#include <AIGame/Entities/PlayerRespawnerEntity.hpp>

#include <sstream>

// Constructors
HUD::HUD(sf::Font* _font) :
	font(_font) {
}

HUD::~HUD() {
}

// Update
void HUD::Update(Game* _game, float _delta) {
}

// Draw
void HUD::Draw(Game* _game, float _delta, sf::RenderTarget* _target) {
	// Get the player
	ActorEntity* actor = dynamic_cast<ActorEntity*>(_game->GetMap()->GetEntityList()->GetEntity("player"));
	if (actor == NULL)
		return;

	// Unset the view
	sf::View oldview = _target->getView();
	sf::View newview(sf::FloatRect(0, 0, _target->getSize().x, _target->getSize().y));
	_target->setView(newview);

	// Create strings
	std::stringstream ammostring;
	ammostring << "Ammo: " << actor->GetAmmo() << " / " << actor->GetMaxAmmo() << std::endl;
	std::stringstream weaponstring;
	weaponstring << actor->GetWeaponName() << std::endl;
	std::stringstream healthstring;
	healthstring << "HP: " << (int)(actor->GetHealth() * 100.0f) << " / " << "100" << std::endl;

	// FPS string
	std::stringstream fpsstring;
	fpsstring << "FPS: " << _game->GetFPS() << std::endl;

	// Get an AI
	ActorEntity* ai = dynamic_cast<ActorEntity*>(_game->GetMap()->GetEntityList()->GetEntity("player"));
	if (ai != NULL) {
		// Plan string
		std::stringstream planstring;
		std::vector<Action>* plan = ai->GetPlan();

		planstring << "Plan: " << std::endl;
		for (int i = 0; i < plan->size(); ++i) {
			switch (plan->at(i).GetActionId()) {
			case Action::GOTO:
				planstring << "Going to " << plan->at(i).GetPos().x << ", " << plan->at(i).GetPos().y;
				break;
			case Action::SHOOTENEMY:
				planstring << "Medium range shoot";
				break;
			case Action::CLOSESHOOTENEMY:
				planstring << "Short range shoot";
				break;
			case Action::FARSHOOTENEMY:
				planstring << "Long range shoot";
				break;
			case Action::PICKUPWEAPON:
				planstring << "Pick up weapon at " << plan->at(i).GetPos().x << ", " << plan->at(i).GetPos().y;
				break;
			case Action::PICKUPMEDKIT:
				planstring << "Pick up health at " << plan->at(i).GetPos().x << ", " << plan->at(i).GetPos().y;
				break;
			case Action::TAKECOVER:
				planstring << "Take cover at " << plan->at(i).GetPos().x << ", " << plan->at(i).GetPos().y;
				break;
			case Action::RUNAWAY:
				planstring << "Running away!";
				break;
			}

			planstring << std::endl;
		}

		sf::Text plantext(planstring.str(), *font, 20);
		plantext.setPosition(_target->getSize().x - plantext.getGlobalBounds().width - 50, 50);
		_target->draw(plantext);
	}

	// Draw them
	
	// FPS
	sf::Text fpstext(fpsstring.str(), *font, 30);
	fpstext.setPosition(50, 50);
	_target->draw(fpstext);

	// Lives
	std::stringstream deathstring;
	std::vector<PlayerRespawnerEntity*> respawners = _game->GetMap()->GetEntityList()->GetEntities<PlayerRespawnerEntity>();
	for (int i = 0; i < respawners.size(); ++i) {
		if (respawners[i]->GetAI() == false) {
			deathstring << "Player k/d: " << respawners[i]->GetKillCount() << " / " << respawners[i]->GetCount() << std::endl;
		} else {
			deathstring << "Bot k/d: " <<  respawners[i]->GetKillCount() << " / " << respawners[i]->GetCount() << std::endl;
		}
	}

	sf::Text deathtext(deathstring.str(), *font, 20);
	deathtext.setPosition(fpstext.getPosition() + sf::Vector2f(0, + 50));
	_target->draw(deathtext);

	// Ammo
	sf::Text ammotext(ammostring.str(), *font, 30);
	ammotext.setPosition(50, _target->getSize().y - ammotext.getGlobalBounds().height - 50);
	_target->draw(ammotext);

	sf::Text weapontext(weaponstring.str(), *font, 40);
	weapontext.setPosition(ammotext.getPosition().x, ammotext.getPosition().y - weapontext.getGlobalBounds().height);
	_target->draw(weapontext);

	// Health
	sf::Text healthtext(healthstring.str(), *font, 30);
	healthtext.setPosition(_target->getSize().x - 50 - healthtext.getGlobalBounds().width, ammotext.getPosition().y);
	_target->draw(healthtext);

	// Set the view back
	_target->setView(oldview);
}