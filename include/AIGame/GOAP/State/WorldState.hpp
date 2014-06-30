#ifndef AI_WORLDSTATE_HPP
#define AI_WORLDSTATE_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

struct Goal;

struct WorldState {
	// Classes
	struct Data {
		union {
			bool bValue;
			int iValue;
			float fValue;
		};

		Data() {
		}
		Data(bool _v) {
			iValue = _v ? 1 : 0;
		}
		Data(int _v) {
			iValue = _v;
		}
		Data(float _v) {
			fValue = _v;
		}
	};
	enum DataName {
		NOTHING = 0,

		// Positions
		PLAYERPOSITIONX,
		PLAYERPOSITIONY,
		ACTORPOSITIONX,
		ACTORPOSITIONY,

		PLAYERALIVE,
		HASGUN,
		LOWHEALTH,
		LOWAMMO,
		SHORTRANGEGUN,
		MEDIUMRANGEGUN,
		LONGRANGEGUN,

		UNDERFIRE,
		RUNNINGAWAY,

		COUNT
	};

	// Constructor
	WorldState() {
		memset(data, 0, sizeof(data));
		memset(care, 0, sizeof(care));
	}
	WorldState(const WorldState& _state) {
		memcpy(data, _state.data, sizeof(_state.data));
		memcpy(care, _state.care, sizeof(_state.care));
	}

	// The actual data
	Data data[DataName::COUNT];
	bool care[DataName::COUNT]; // values that don't matter

	// Set data
	void SetData(WorldState::DataName _name, WorldState::Data _data) {
		data[_name] = _data;
		care[_name] = true;
	}

	// Check if a goal state matches this state
	bool Check(const WorldState& _goal) const;
};

#endif