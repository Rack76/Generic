#include "GameEngine/System/Physics/Particle.h"

void Particle::integrate(float &dt)
{
	if (inverseMass <= 0.0f)
		return;

	acceleration = forceAccumulator * inverseMass;
	velocity = velocity * damping * dt + acceleration * dt;
	forceAccumulator = glm::vec3(0.0f, 0.0f, 0.0f);
}