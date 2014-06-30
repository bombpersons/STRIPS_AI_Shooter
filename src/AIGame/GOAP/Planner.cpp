#include <AIGame/GOAP/Planner.hpp>

// Constructors
Planner::Planner() {
}
Planner::~Planner() {
}

// Get a plan
void Planner::GetPlan(const ImmutableWorldState& _immutable, const WorldState& _current, const WorldState& _goal, std::vector<Action>* _actions) {
	// We want to find a series of actions, that takes our current worldstate, to the goal worldstate.
	// If our current worldstate already satisfies the goalstate, then we should return (since we don't need to formulate a plan)
	if (_current.Check(_goal)) {
		return;
	}

	// Open and closed lists
	ListType openlist;
	ListType closedlist;

	// List of neighbour actions
	std::vector<Action> neighbours;

	// Get potential actions
	GetPossibleActions(_immutable, _current, _goal, &neighbours);

	// Add these to the openlist and queue
	for (int i = 0; i < (int)neighbours.size(); ++i) {
		ActionNode node(neighbours[i], _current, _current, _goal);
		node.startnode = true;
		node.G = node.action.GetCost();
		node.H = CaclHeuristic(node.current, node.goal);
		openlist.insert(ListType::value_type(node.current, node));
	}

	// Loop through the openlist
	while (openlist.size() != 0) {
		// Pop an item of from the list
		float lowest  = 99999999999999999.0f;
		ListType::iterator lowestiterator;
		for (ListType::iterator it = openlist.begin(); it != openlist.end(); ++it) {
			if (it->second.G + it->second.H < lowest) {
				lowest = it->second.G + it->second.H;
				lowestiterator = it;
			}
		}
		ActionNode node = lowestiterator->second;
		openlist.erase(lowestiterator);

		// Add to closed list
		closedlist.insert(ListType::value_type(node.current, node));

		// Check if using this action will give us a plan that satisfies the goal.
		if (node.current.Check(node.goal)) {
			// Create the plan
			ActionNode* actionnode; 
			actionnode = &node;

			while (true) {
				_actions->push_back(actionnode->action);

				if (actionnode->startnode)
					break;

				actionnode = &closedlist.find(actionnode->parent)->second;
			}

			return;
		}

		// Get possible actions from this point
		GetPossibleActions(_immutable, node.current, node.goal, &neighbours);
		for (int i = 0; i < (int)neighbours.size(); ++i) {
			ActionNode neighbournode(neighbours[i], node.current, node.current, node.goal);
			
			// Is it even possible for anything to be on the closed or not in open list here?
			if (closedlist.find(neighbournode.current) == closedlist.end()) {
				// Calculate the cost
				float G = node.G + neighbournode.action.GetCost();

				// Check if this is in the openlist
				ListType::iterator opennode = openlist.find(neighbournode.current);
				if (opennode != openlist.end()) {
					// If our G is better than the one we currently have, replace it
					if (G < opennode->second.G) {
						openlist.erase(opennode);

						neighbournode.parent = node.current;
						neighbournode.G = G;
						openlist.insert(ListType::value_type(neighbournode.current, neighbournode));
					}
				} else {
					neighbournode.G = G;
					neighbournode.H = CaclHeuristic(neighbournode.current, neighbournode.goal);
					neighbournode.parent = node.current;
					openlist.insert(ListType::value_type(neighbournode.current, neighbournode));
				}
			}
		}
	}

	// No possible plan for this..
	return;
}

// Get possible actions from a state
void Planner::GetPossibleActions(const ImmutableWorldState& _immutable, const WorldState& _current, const WorldState& _goal, std::vector<Action>* _actions) {
	// Clear the list
	_actions->clear();

	// Shoot enemy
	Action shoot;
	shoot.ShootEnemy(_immutable.health);
	_actions->push_back(shoot);

	// Run away
	Action run;
	run.RunAway();
	_actions->push_back(run);

	// Pickup weapons
	for (int i = 0; i < _immutable.weaponcount; ++i) {
		Action action;
		action.PickupWeaponAction(_current.data[WorldState::ACTORPOSITIONX].iValue, _current.data[WorldState::ACTORPOSITIONY].iValue, _immutable.weapons[i].pos.x, _immutable.weapons[i].pos.y);
		_actions->push_back(action);
	}

	// Pickup medkits
	for (int i = 0; i < _immutable.medkitcount; ++i) {
		Action action;
		action.PickupMedkitAction(_current.data[WorldState::ACTORPOSITIONX].iValue, _current.data[WorldState::ACTORPOSITIONY].iValue, _immutable.medkits[i].x, _immutable.medkits[i].y);
		_actions->push_back(action);
	}

	// Take cover
	for (int i = 0; i < _immutable.covercount; ++i) {
		Action action;
		action.TakeCover(_current.data[WorldState::ACTORPOSITIONX].iValue, _current.data[WorldState::ACTORPOSITIONY].iValue, _immutable.cover[i].x, _immutable.cover[i].y);
		_actions->push_back(action);
	}

	// Get rid of any actions that don't satisfy the goal
	for (std::vector<Action>::iterator it = _actions->begin(); it != _actions->end();) {
		bool foundall = false;
		for (int i = 0; i < it->GetEffectCount(); ++i) {
			// If this action has an effect that we care about
			if (_goal.care[it->GetEffect(i)->name]) {
				// Check if this precondition hasn't already been fulfilled
				if (memcmp(&(_current.data[it->GetEffect(i)->name]), &(_goal.data[it->GetEffect(i)->name]), sizeof(WorldState::Data)) != 0) {
					// Check if this effect would give us the correct result
					if (memcmp(&_goal.data[it->GetEffect(i)->name], &it->GetEffect(i)->data, sizeof(WorldState::Data)) == 0) {
						foundall = true;
						break;
					}
				}
			}
		}

		if (!foundall) {
			it = _actions->erase(it);
		} else {
			++it;
		}
	}
}

float Planner::CaclHeuristic(const WorldState& _state1, const WorldState& _state2) {
	float count = 0.0f;
	for (int i = 0; i < WorldState::COUNT; ++i) {
		if (_state1.care[i] || _state2.care[i]) {
			if (memcmp(&_state1.data[i], &_state2.data[i], sizeof(WorldState::Data)) == 0)
				count += 1.0f;
		}
	}
	return count;
}