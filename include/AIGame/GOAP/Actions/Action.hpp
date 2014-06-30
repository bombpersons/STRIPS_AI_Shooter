#ifndef AI_ACTION_HPP
#define AI_ACTION_HPP

#include <AIGame/GOAP/State/WorldProperty.hpp>

struct Action {
public:
	// Classes
	enum ActionType {
		GOTO = 0,
		SHOOTENEMY,
		CLOSESHOOTENEMY,
		FARSHOOTENEMY,
		PICKUPWEAPON,
		TAKECOVER,
		RUNAWAY,
		PICKUPMEDKIT
	};

	// Constructors
	Action() {
		actionid = 0;
		prerequisitecount = 0;
		effectcount = 0;
	}
	Action(const Action& _action) {
		prerequisitecount = _action.prerequisitecount;
		memcpy(prerequisites, _action.prerequisites, sizeof(WorldProperty) * prerequisitecount);
	
		effectcount = _action.effectcount;
		memcpy(effects, _action.effects, sizeof(WorldProperty) * effectcount);

		actionid = _action.actionid;
		cost = _action.cost;
		pos = _action.pos;
	}
	virtual ~Action() {
	}

	// The action id
	int GetActionId() {
		return actionid;
	}

	// Get the cost
	float GetCost() {
		return cost;
	}

	// Effects
	void SetEffectCount(int _i) {
		effectcount = _i;
	}
	void SetEffect(int _i, const WorldProperty& _prop) {
		effects[_i] = _prop;
	}
	const WorldProperty* GetEffect(int _i) const  {
		return &(effects[_i]);
	}
	int GetEffectCount() const {
		return effectcount;
	}

	// Prerequisites
	void SetPrerequisiteCount(int _i) {
		prerequisitecount = _i;
	}
	void SetPrerequisite(int _i, const WorldProperty& _prop) {
		prerequisites[_i] = _prop;
	}
	const WorldProperty* GetPrerequisite(int _i) const  {
		return &(prerequisites[_i]);
	}
	const WorldProperty* GetPrerequisiteFromName(WorldState::DataName _name) {
		for (int i = 0; i < prerequisitecount; ++i) {
			if (prerequisites[i].name = _name)
				return &(prerequisites[i]);
		}
	}
	int GetPrerequisiteCount() const {
		return prerequisitecount;
	}
	const sf::Vector2i& GetPos() {
		return pos;
	}

	// Actions
	void GotoAction(int _x1, int _y1, int _x2, int _y2) {
		// Prerequisites.
		SetPrerequisiteCount(2);
		SetPrerequisite(0, WorldProperty(WorldState::ACTORPOSITIONX, _x1));
		SetPrerequisite(1, WorldProperty(WorldState::ACTORPOSITIONY, _y1));

		// Effects
		SetEffectCount(2);
		SetEffect(0, WorldProperty(WorldState::ACTORPOSITIONX, _x2));
		SetEffect(1, WorldProperty(WorldState::ACTORPOSITIONY, _y2));

		// Set the action id
		actionid = Action::GOTO;

		// Set the cost
		cost = abs(_x1 - _x2) + abs(_y1 - _y2);

		// Position
		pos = sf::Vector2i(_x2, _y2);
	}
	void PickupMedkitAction(int _x1, int _y1, int _x2, int _y2) {
		// Prerequisites
		SetPrerequisiteCount(1);
		SetPrerequisite(0, WorldProperty(WorldState::UNDERFIRE, false));
		
		// Effects
		SetEffectCount(3);
		SetEffect(0, WorldProperty(WorldState::LOWHEALTH, false));
		SetEffect(1, WorldProperty(WorldState::ACTORPOSITIONX, _x2));
		SetEffect(2, WorldProperty(WorldState::ACTORPOSITIONY, _y2));

		// Set the action id
		actionid = Action::PICKUPMEDKIT;

		// Cost
		cost = abs(_x1 - _x2) + abs(_y1 - _y2) / 2.0f;

		// Position
		pos = sf::Vector2i(_x2, _y2);
	}
	void PickupWeaponAction(int _x1, int _y1, int _x2, int _y2) {
		SetPrerequisiteCount(1);
		SetPrerequisite(0, WorldProperty(WorldState::UNDERFIRE, false));

		// Effects
		SetEffectCount(3);
		SetEffect(0, WorldProperty(WorldState::HASGUN, true));
		SetEffect(1, WorldProperty(WorldState::ACTORPOSITIONX, _x2));
		SetEffect(2, WorldProperty(WorldState::ACTORPOSITIONY, _y2));

		// Set the action id
		actionid = Action::PICKUPWEAPON;

		// Cost
		cost = abs(_x1 - _x2) + abs(_y1 - _y2);

		// Position
		pos = sf::Vector2i(_x2, _y2);
	}
	void TakeCover(int _x1, int _y1, int _x2, int _y2) {
		actionid = TAKECOVER;

		SetPrerequisiteCount(1);
		SetPrerequisite(0, WorldProperty(WorldState::UNDERFIRE, true));

		SetEffectCount(3);
		SetEffect(0, WorldProperty(WorldState::UNDERFIRE, false));
		SetEffect(1, WorldProperty(WorldState::ACTORPOSITIONX, _x2));
		SetEffect(2, WorldProperty(WorldState::ACTORPOSITIONY, _y2));

		// Cost
		cost = (abs(_x1 - _x2) + abs(_y1 - _y2)) * 10.0f; // Make taking cover cheaper;

		// Position
		pos = sf::Vector2i(_x2, _y2);
	}
	void RunAway() {
		actionid = RUNAWAY;

		SetEffectCount(2);
		SetEffect(0, WorldProperty(WorldState::UNDERFIRE, false));
		SetEffect(1, WorldProperty(WorldState::RUNNINGAWAY, true));

		cost = 200.0f; // High cost since this is undesirable. If there is cover we should take it.
	}
	void ShootEnemy(float _health) {
		actionid = SHOOTENEMY;

		SetPrerequisiteCount(2);
		SetPrerequisite(0, WorldProperty(WorldState::HASGUN, true));
		SetPrerequisite(1, WorldProperty(WorldState::UNDERFIRE, false));

		SetEffectCount(1);
		SetEffect(0, WorldProperty(WorldState::PLAYERALIVE, false));

		// Cost
		cost = (_health) * 10.0f; // Character might die, so add some cost.
	}


protected:
	int actionid;

	WorldProperty prerequisites[5];
	int prerequisitecount;

	WorldProperty effects[5];
	int effectcount;

	float cost;
	sf::Vector2i pos;
};

#endif