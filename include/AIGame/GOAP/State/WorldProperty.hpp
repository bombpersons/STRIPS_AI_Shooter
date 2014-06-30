#ifndef AI_WORLDPROPERTY_HPP
#define AI_WORLDPROPERTY_HPP

#include <AIGame/GOAP/State/WorldState.hpp>

struct WorldProperty {
	WorldProperty() {
		name = WorldState::NOTHING;
		data.iValue = 0;
	}
	WorldProperty(WorldState::DataName _name, bool _value) {
		name = _name;
		data.iValue = _value ? 1 : 0;
	}
	WorldProperty(WorldState::DataName _name, int _value) {
		name = _name;
		data.iValue = _value;
	}
	WorldProperty(WorldState::DataName _name, float _value) {
		name = _name;
		data.fValue = _value;
	}

	WorldState::DataName name;
	WorldState::Data data;
};

#endif