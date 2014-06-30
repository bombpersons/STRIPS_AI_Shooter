#ifndef AI_PARTICLEMANAGER_HPP
#define AI_PARTICLEMANAGER_HPP

#include <AIGame/Macros.hpp>

#include <AIGame/Graphics/Spritebatch.hpp>

class ParticleManager {
public:
	// Structs
	struct Particle {
		sf::Vector2f position;
		sf::Vector2f velocity;
		sf::Vector2f acceleration;
		sf::Vector2f size;
		sf::Vector2f sizeincrease;
		sf::Color color;
		float alphachange;
		float alpha;
		float rotation;
		float rotationchange;
		float lifetime;

		Particle() {
			position = sf::Vector2f();
			velocity = sf::Vector2f();
			acceleration = sf::Vector2f();
			size = sf::Vector2f(1, 1);
			sizeincrease = sf::Vector2f();
			color = sf::Color::White;
			alphachange = 0.0f;
			alpha = 1.0f;
			rotation = 0.0f;
			rotationchange = 0.0f;
			lifetime = 1.0f;
		}
	};

	// Constructors
	ParticleManager(int _maxparticles=1000);
	virtual ~ParticleManager();

	// Update
	void Update(float _delta);

	// Draw
	void Draw(sf::RenderTarget* _target);

	// Add / Remove particles
	void AddParticle(const Particle& _particle);
	void RemoveParticle(int _i); 

	// More specific particle release functions
	void ReleaseParticlesRandomVelocity(const Particle& _template, int _minnumber, int _maxnumber, const sf::Vector2f& _dir, float _minangle, float _maxangle, float _minspeed, float _maxspeed);

private:
	// The maximum amount of particles
	int maxparticles;
	int particlecount;
	Particle* particles;

	// Spritebatch for drawing.
	SpriteBatch spritebatch;

	// No copying
	UNCOPYABLE(ParticleManager)
};

#endif