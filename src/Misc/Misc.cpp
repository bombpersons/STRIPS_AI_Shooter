#include <Misc/Misc.hpp>

float Clamp(float _value, float _min, float _max) {
	if (_value < _min)
		return _min;
	if (_value > _max)
		return _max;
	return _value;
}