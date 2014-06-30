#ifndef AI_CAMERA_HPP
#define AI_CAMERA_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <AIGame/Macros.hpp>

class Camera {
public:
	// Constructors
	Camera();
	virtual ~Camera();

	// Get / Set
	void SetPosition(const sf::Vector2f& _pos);
	void SetPositionX(float _x);
	void SetPositionY(float _y);
	void SetScale(const sf::Vector2f& _scale);
	void SetScaleX(float _x);
	void SetScaleY(float _y);
	void SetSize(const sf::Vector2f& _size);
	void SetSizeX(float _x);
	void SetSizeY(float _y);
	void SetRotation(float _rotation);

	sf::Vector2f GetPosition();
	float GetPositionX();
	float GetPositionY();
	sf::Vector2f GetScale();
	float GetScaleX();
	float GetScaleY();
	sf::Vector2f GetSize();
	float GetSizeX();
	float GetSizeY();
	float GetRotation();

	// Obtain a view to use with SFML
	sf::View GetView();

private:
	// Location and rotation and scale.
	sf::Vector2f position;
	sf::Vector2f scale;
	sf::Vector2f size;
	float rotation;
};

#endif