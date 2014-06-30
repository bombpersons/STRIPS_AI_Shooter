#include <Misc/VectorMaths.hpp>

#include <cmath>
#include <AIGame/Macros.hpp>

sf::Vector2f VectorNormalize(const sf::Vector2f& _a) {
	float dist = VectorLength(_a);
	return sf::Vector2f(_a.x / dist, _a.y / dist);
}

float VectorLength(const sf::Vector2f& _a) {
	return sqrt(DotProduct(_a, _a));
}

float VectorLength(const sf::Vector2i& _a) {
	sf::Vector2f a((float)_a.x, (float)_a.y);
	return sqrt(DotProduct(a, a));
}

float DotProduct(const sf::Vector2f& _a, const sf::Vector2f& _b) {
	return (_a.x * _b.x + _a.y * _b.y);
}

float CrossProduct(const sf::Vector2f& _a, const sf::Vector2f& _b) {
	return (_a.y * _b.x -_a.x * _b.y);
}

sf::Vector2f VectorRotate(const sf::Vector2f& _dir, float _angle) {
	float cs = cosf(_angle);
	float sn = sinf(_angle);

	return sf::Vector2f(_dir.x * cs - _dir.y * sn, _dir.x * sn + _dir.y * cs);
}

sf::Vector2f VectorOfAngle(float _angle) {
	return sf::Vector2f(cosf(_angle), sinf(_angle));
}

float AngleOfVector(const sf::Vector2f& _dir) {
	return atan2f(_dir.y, _dir.x);
}

float Lerp(float _a, float _b, float _amount) {
	return _a + (_b - _a) * _amount;
}

sf::Vector2f VectorLerp(const sf::Vector2f& _a, const sf::Vector2f& _b, float _amount) {
	return sf::Vector2f(Lerp(_a.x, _b.x, _amount), Lerp(_a.y, _b.y, _amount));
}

float AbsMod(float _a, float _n) {
	return fmod(fmod(_a, _n) + _n, _n);
}

float AngleDelta(float _a, float _b) {
	float diff = _a - _b;
	diff = AbsMod(diff + M_PI, 2*M_PI) - M_PI;
	return diff;
}

float Attenuation(float _d, float _c, float _l, float _q) {
	return 1.0f / (_c + _l * _d + _q * _d * _d);
}

float MannhattonDistance(const sf::Vector2i& _diff) {
	return abs(_diff.x) + abs(_diff.y);
}