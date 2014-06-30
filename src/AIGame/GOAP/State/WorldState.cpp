#include <AIGame/GOAP/State/WorldState.hpp>
#include <AIGame/GOAP/State/Goal.hpp>

// Check if a goal state matches this state
bool WorldState::Check(const WorldState& _goal) const {
	for (int i = 0; i < WorldState::COUNT; ++i) {
		if (_goal.care[i]) {
			if (memcmp(&data[i], &_goal.data[i], sizeof(WorldState::Data)) != 0)
				return false;
		}
	}
	return true;
}