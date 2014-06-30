#ifndef AI_GEOMETRY_HPP
#define AI_GEOMETRY_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

bool PointIntersectsRectangle(const sf::Vector2f& _point, const sf::Vector2f& _topleft, const sf::Vector2f& _bottomright);
bool RectangleIntersectRectangle(const sf::Vector2f& _toplefta, const sf::Vector2f& _bottomrighta, const sf::Vector2f& _topleftb, const sf::Vector2f& _bottomrightb);
bool PointIntersectsCircle(sf::Vector2f _point, sf::Vector2f _center, float _radius);
bool LineIntersectsLine(const sf::Vector2f& _a1, const sf::Vector2f& _a2, const sf::Vector2f& _b1, const sf::Vector2f& _b2, sf::Vector2f* _out);
bool LineIntersectsRectangle(const sf::Vector2f& _topleft, const sf::Vector2f& _bottomright, const sf::Vector2f& _start, const sf::Vector2f& _end, sf::Vector2f* _out);
bool LineIntersectsCircle(const sf::Vector2f& _pointa, const sf::Vector2f& _pointb, const sf::Vector2f& _center, float _radius, sf::Vector2f* _out=NULL);
bool CircleIntersectsRectangle(const sf::Vector2f& _center, float _radius, const sf::Vector2f& _topleft, const sf::Vector2f& _bottomright);

#endif