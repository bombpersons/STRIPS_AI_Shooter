#ifndef AI_PLANNER_HPP
#define AI_PLANNER_HPP

#include <AIGame/GOAP/State/WorldState.hpp>
#include <AIGame/GOAP/State/WorldProperty.hpp>
#include <AIGame/GOAP/Actions/Action.hpp>
#include <AIGame/GOAP/State/Goal.hpp>
#include <AIGame/GOAP/ImmutableWorldState.hpp>

#include <set>
#include <unordered_map>

class Planner {
public:
	// Classes
	struct ActionNode {
		ActionNode(const Action& _action, const WorldState& _parent, const WorldState& _current, const WorldState& _goal) {
			action = _action;
			G = 0;
			parent = _parent;
			startnode = 0;

			// The goal at the next node
			current = _current;
			goal = _goal;
			//memset(goal.care, 9, sizeof(goal.care));
			//for (int i = 0; i < action.GetPrerequisiteCount(); ++i) {
			//	goal.data[action.GetPrerequisite(i)->name] = action.GetPrerequisite(i)->data;
			//	goal.care[action.GetPrerequisite(i)->name] = true;
			//}

			// Get the effects of the previous nodes
			//memset(goal.care, 0, sizeof(goal.care));
			for (int i = 0; i < action.GetPrerequisiteCount(); ++i) {
				// If we already meet the requirements, there is no need to change the goal.
				if (memcmp(&current.data[action.GetPrerequisite(i)->name], &action.GetPrerequisite(i)->data, sizeof(WorldState::Data)) != 0) {
					goal.data[action.GetPrerequisite(i)->name] = action.GetPrerequisite(i)->data;
					goal.care[action.GetPrerequisite(i)->name] = true;
				} else {
					//goal.care[action.GetPrerequisite(i)->name] = false;
				}
			}
			for (int i = 0; i < action.GetEffectCount(); ++i) {
				current.data[action.GetEffect(i)->name] = action.GetEffect(i)->data;
			}
		}

		Action action;
		float G; // Cost
		float H; // Heuristic
		WorldState current; // The state at this node.
		WorldState goal; // The state that we are aiming for.
		WorldState parent; // The parent state.
		bool startnode;
	};

	class WorldStateComparator {
	public:
		bool operator() (const WorldState& _a, const WorldState& _b) const {
			return memcmp(_a.data, _b.data, sizeof(_a.data)) < 0;
		}
	};

	// Constructors
	Planner();
	virtual ~Planner();

	// Get a path
	void GetPlan(const ImmutableWorldState& _immutable, const WorldState& _current, const WorldState& _goal, std::vector<Action>* _actions);

	// Get possible actions from a state
	void GetPossibleActions(const ImmutableWorldState& _immutable, const WorldState& _current, const WorldState& _goal, std::vector<Action>* _actions);

private:
	// Calculate heuristic
	float CaclHeuristic(const WorldState& _state1, const WorldState& _state2);

	// Types
	typedef std::map<WorldState, ActionNode, WorldStateComparator> ListType;
};

#endif