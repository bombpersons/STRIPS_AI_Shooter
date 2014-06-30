#include <AIGame/Particles/ParticleManager.hpp>
#include <Misc/VectorMaths.hpp>

// Constructors
ParticleManager::ParticleManager(int _maxparticles) :
	maxparticles(_maxparticles),
	particlecount(0) {

	// Create the array of particles
	particles = new Particle[_maxparticles];
}

ParticleManager::~ParticleManager() {
}

// Update
void ParticleManager::Update(float _delta) {
	// Update the all of the particles.
	for (int i = 0; i < particlecount; ++i) {
		// Decrease lifetime
		particles[i].lifetime -= _delta;
		if (particles[i].lifetime < 0) {
			// Remove the particle and continue;
			RemoveParticle(i);
			continue;
		}

		// Move the particle
		particles[i].position += particles[i].velocity * _delta;
		particles[i].velocity += particles[i].acceleration * _delta;

		// Increase / decrease the size
		particles[i].size += particles[i].sizeincrease * _delta;

		// Change the color
		particles[i].alpha += particles[i].alphachange * _delta;
		if (particles[i].alpha < 0)
			particles[i].alpha = 0;
	}
}

// Draw
void ParticleManager::Draw(sf::RenderTarget* _target) {
	// Draw all of the particles.
	for (int i = 0; i < particlecount; ++i) {
		// Color
		sf::Color color = particles[i].color;
		color.a = (sf::Uint8)(particles[i].alpha * 255.0f);

		spritebatch.Draw(_target,
						 NULL,
						 particles[i].position,
						 sf::Vector2f(0.5f, 0.5f),
						 particles[i].size,
						 particles[i].rotation,
						 color);
	}

	// Flush the buffer.
	spritebatch.Flush(_target);
}

// Add a particle
void ParticleManager::AddParticle(const ParticleManager::Particle& _particle) {
	// Check if we can add any more.
	if (particlecount >= maxparticles - 1)
		return;

	// Add the particle onto the end of the list
	particles[particlecount] = _particle;
	particlecount++;
}

// Remove a particle
void ParticleManager::RemoveParticle(int _i) {
	// Check if this 

	// Swap the last particle with this particle and decrease the count
	particles[_i] = particles[particlecount-1];
	particlecount--;

	return;
}

void ParticleManager::ReleaseParticlesRandomVelocity(const Particle& _template, int _minnumber, int _maxnumber, const sf::Vector2f& _dir, float _minangle, float _maxangle, float _minspeed, float _maxspeed) {
	// If there are no particles to release, return
	if (_maxnumber == 0)
		return;

	// Decide how many particles to generate.
	int numparticles = _minnumber + rand() % (_maxnumber - _minnumber);
	for (int i = 0; i < numparticles; ++i) {
		// Decide the angle
		float angle = _minangle + (_maxangle - _minangle) * (rand() / (float)RAND_MAX);

		// Decide the speed
		float speed = _minspeed + (_maxspeed - _minspeed) * (rand() / (float)RAND_MAX);

		// Create the particle
		Particle particle = _template;
		particle.velocity = VectorRotate(_dir, angle) * speed;
		particle.acceleration = VectorNormalize(particle.velocity) * -(speed / particle.lifetime);
		AddParticle(particle);
	}
}