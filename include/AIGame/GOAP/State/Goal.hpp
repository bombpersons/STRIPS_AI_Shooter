#ifndef AI_GOAL_HPP
#define AI_GOAL_HPP

#include <AIGame/GOAP/State/WorldProperty.hpp>

struct Goal {
	WorldProperty data[WorldState::COUNT];
	int datacount;

	// Constructor
	Goal() {
		memset(data, 0, sizeof(data));
		datacount = 0;
	}
	virtual ~Goal() {
	}

	// Add a property
	void AddProperty(const WorldProperty& _prop) {
		data[datacount] = _prop;
		datacount++;
	}

	int FindProperty(WorldState::DataName _name) const {
		for (int i = 0; i < datacount; ++i) {
			if (data[i].name == _name) {
				return i;
			}
		}
		return -1;
	}

	void SetProperty(const WorldProperty& _prop) {
		int i = FindProperty(_prop.name);
		if (i < 0) {
			AddProperty(_prop);	
			return;
		}

		data[i].data = _prop.data;
	}
};

#endif