#ifndef PARTICLE_H
#define PARTICLE_H

#include "glm/glm.hpp"

class Particle
{
public:
	void integrate(float& dt);

	void setMass(float mass)
	{
		if(mass != 0)
			inverseMass = 1 / mass;
	}

	void setInverseMass(float inverseMass)
	{
		inverseMass == inverseMass;
	}

	glm::vec3 forceAccumulator = glm::vec3(0.0f, -10.0f, 0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f, -10.0f, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	float damping = 0.7f;

private:
	float inverseMass = 1;
};

#endif 