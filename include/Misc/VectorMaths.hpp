#ifndef AI_VECTORMATHS_HPP
#define AI_VECTORMATHS_HPP

#include <SFML/System.hpp>

sf::Vector2f VectorNormalize(const sf::Vector2f& _a);
float VectorLength(const sf::Vector2f& _a);
float VectorLength(const sf::Vector2i& _a);
float DotProduct(const sf::Vector2f& _a, const sf::Vector2f& _b);
float CrossProduct(const sf::Vector2f& _a, const sf::Vector2f& _b);
sf::Vector2f VectorRotate(const sf::Vector2f& _dir, float _angle);
sf::Vector2f VectorOfAngle(float _angle);
float AngleOfVector(const sf::Vector2f& _dir);
float Lerp(float _a, float _b, float _amount);
sf::Vector2f VectorLerp(const sf::Vector2f& _a, const sf::Vector2f& _b, float _amount);
float AngleDelta(float _a, float _b);
float Attenuation(float _d, float _c, float _l, float _q);
float MannhattonDistance(const sf::Vector2i& _diff);

#endif