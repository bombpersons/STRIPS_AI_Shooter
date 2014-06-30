#include <Misc/Geometry.hpp>
#include <Misc/VectorMaths.hpp>

bool PointIntersectsRectangle(const sf::Vector2f& _point, const sf::Vector2f& _topleft, const sf::Vector2f& _bottomright) {
	return (_point.x > _topleft.x && _point.x < _bottomright.x &&
			_point.y > _topleft.y && _point.y < _bottomright.y);
}

bool RectangleIntersectRectangle(const sf::Vector2f& _toplefta, const sf::Vector2f& _bottomrighta, const sf::Vector2f& _topleftb, const sf::Vector2f& _bottomrightb) {
	return (_toplefta.x < _bottomrightb.x && _bottomrighta.x > _topleftb.x &&
			_toplefta.y < _bottomrightb.y && _bottomrighta.y > _topleftb.y);
}

bool PointIntersectsCircle(const sf::Vector2f& _point, const sf::Vector2f& _center, float _radius) {
	// Get the distance
	sf::Vector2f diff = _point - _center;
	float dist = sqrt(diff.x * diff.x + diff.y * diff.y);
	return (dist < _radius);
}

bool LineIntersectsLine(const sf::Vector2f& _a1, const sf::Vector2f& _a2, const sf::Vector2f& _b1, const sf::Vector2f& _b2, sf::Vector2f* _out) {
	sf::Vector2f a = _a2 - _a1;
	sf::Vector2f b = _b2 - _b1;

	float f = CrossProduct(a, b);
	if (f == 0)
		return false;

	sf::Vector2f c = _b2 - _a2;
	float aa = CrossProduct(a, c);
	float bb = CrossProduct(b, c);
	
	if (f < 0)
	{
		if (aa > 0) return false;
		if (bb > 0) return false;
		if (aa < f) return false;
		if (bb < f) return false;
	} else {
		if (aa < 0) return false;
		if (bb < 0) return false;
		if (aa > f) return false;
		if (bb > f) return false;
	}

	float dist = 1.0f - (aa / f);
	*_out = ((_b2 - _b1) * dist) + _b1;
	return true;
}

bool LineIntersectsRectangle(const sf::Vector2f& _topleft, const sf::Vector2f& _bottomright, const sf::Vector2f& _start, const sf::Vector2f& _end, sf::Vector2f* _out) {
	// Check each line on the rectangle
	if (LineIntersectsLine(_start, _end, _topleft, sf::Vector2f(_topleft.y, _bottomright.x), _out)) {
		return true;
	}
	if (LineIntersectsLine(_start, _end, sf::Vector2f(_topleft.y, _bottomright.x), _bottomright, _out)) {
		return true;
	}
	if (LineIntersectsLine(_start, _end, _bottomright, sf::Vector2f(_topleft.x, _bottomright.y), _out)) {
		return true;
	}
	if (LineIntersectsLine(_start, _end, sf::Vector2f(_topleft.x, _bottomright.y), _topleft, _out)) {
		return true;
	}
	return false;
}

// Taken from http://stackoverflow.com/questions/1073336/circle-line-collision-detection
bool LineIntersectsCircle(const sf::Vector2f& _pointa, const sf::Vector2f& _pointb, const sf::Vector2f& _center, float _radius, sf::Vector2f* _out) {
	sf::Vector2f d = _pointb - _pointa;
	sf::Vector2f f = _pointa - _center;
	float r = _radius;

	float a = DotProduct(d, d);
	float b = 2 * DotProduct(f, d);
	float c = DotProduct(f, f) - r*r ;

	float discriminant = b*b-4*a*c;
	if( discriminant < 0 )
		return false;

	// ray didn't totally miss sphere,
	// so there is a solution to
	// the equation.

	discriminant = sqrt( discriminant );

	// either solution may be on or off the ray so need to test both
	// t1 is always the smaller value, because BOTH discriminant and
	// a are nonnegative.
	float t1 = (-b - discriminant)/(2*a);
	float t2 = (-b + discriminant)/(2*a);

	// 3x HIT cases:
	//          -o->             --|-->  |            |  --|->
	// Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit), 

	// 3x MISS cases:
	//       ->  o                     o ->              | -> |
	// FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

	if( t1 >= 0 && t1 <= 1 )
	{
		// t1 is the intersection, and it's closer than t2
		// (since t1 uses -b - discriminant)
		// Impale, Poke
		
		// Caclulate the intersection position.
		if (_out != NULL) {
			*_out = _pointa + (_pointb - _pointa) * t1;
		}

		return true ;
	}

	// here t1 didn't intersect so we are either started
	// inside the sphere or completely past it
	if( t2 >= 0 && t2 <= 1 )
	{
		// Caclulate the intersection position.
		if (_out != NULL) {
			*_out = _pointa + (_pointb - _pointa) * t2;
		}

		// ExitWound
		return true ;
	}

	// no intn: FallShort, Past, CompletelyInside
	return false ;
}

bool CircleIntersectsRectangle(const sf::Vector2f& _center, float _radius, const sf::Vector2f& _topleft, const sf::Vector2f& _bottomright) {
	return (PointIntersectsRectangle(_center, _topleft, _bottomright) ||
			LineIntersectsCircle(_topleft, sf::Vector2f(_topleft.y, _bottomright.x), _center, _radius) ||
			LineIntersectsCircle(sf::Vector2f(_topleft.y, _bottomright.x), _bottomright, _center, _radius) ||
			LineIntersectsCircle(_bottomright, sf::Vector2f(_topleft.x, _bottomright.y), _center, _radius) ||
			LineIntersectsCircle(sf::Vector2f(_topleft.x, _bottomright.y), _topleft, _center, _radius));
}