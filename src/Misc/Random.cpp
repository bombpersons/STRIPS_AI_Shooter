#include <Misc/Random.hpp>

std::default_random_engine randengine;

int RandRange(int _min, int _max) {
	return _min + rand() % (_max - _min);
}

float NormalDistribution(float _mean, float _sigma) {
	std::normal_distribution<float> dist(_mean, _sigma);
	return dist(randengine);
}