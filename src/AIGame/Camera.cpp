#include <AIGame/Camera.hpp>

// Constructors
Camera::Camera() :
	position(0, 0),
	scale(0.5, 0.5),
	size(500, 500),
	rotation(0) {
}

Camera::~Camera() {
}

// Set's / Get's
void Camera::SetPosition(const sf::Vector2f& _pos) {
	position = _pos;
}
void Camera::SetPositionX(float _x) {
	position.x = _x;
}
void Camera::SetPositionY(float _y) {
	position.y = _y;
}
void Camera::SetScale(const sf::Vector2f& _scale) {
	scale = _scale;
}
void Camera::SetScaleX(float _x) {
	scale.x = _x;
}
void Camera::SetScaleY(float _y) {
	scale.y = _y;
}
void Camera::SetSize(const sf::Vector2f& _size) {
	size = _size;
}
void Camera::SetSizeX(float _x) {
	size.x = _x;
}
void Camera::SetSizeY(float _y) {
	size.y = _y;
}
void Camera::SetRotation(float _rot) {
	rotation = _rot;
}

sf::Vector2f Camera::GetPosition() {
	return position;
}
float Camera::GetPositionX() {
	return position.x;
}
float Camera::GetPositionY() {
	return position.y;
}
sf::Vector2f Camera::GetScale() {
	return scale;
}
float Camera::GetScaleX() {
	return scale.x;
}
float Camera::GetScaleY() {
	return scale.y;
}
sf::Vector2f Camera::GetSize() {
	return size;
}
float Camera::GetSizeX() {
	return size.x;
}
float Camera::GetSizeY() {
	return size.y;
}
float Camera::GetRotation() {
	return rotation;
}

// Get the view for a render target
sf::View Camera::GetView() {
	sf::View view(position, sf::Vector2f(scale.x * size.x, scale.y * size.y));
	view.setRotation(RADTODEG(rotation));
	return view;
}