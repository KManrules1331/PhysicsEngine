#include "Spring.h"


Spring::Spring(PhysicsComponent& nodeA, PhysicsComponent& nodeB, glm::vec3 nodeAAnchorPoint, glm::vec3 nodeBAnchorPoint, float k ) : GameObject(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)), a{ nodeA }, b{ nodeB }
{
	//aLoc and bLoc are relative to the object's 0 transform.  To get world coordinates, multiply by node's transform matrices
	aLoc = glm::vec4(nodeAAnchorPoint, 1.0f);
	bLoc = glm::vec4(nodeBAnchorPoint, 1.0f);

	restingLength = glm::length((b.GOTransform.transformMatrix * bLoc) - (a.GOTransform.transformMatrix * aLoc));
	this->k = k;
}


Spring::~Spring()
{
}

void Spring::update(float dt) {
	State aState = State();
	aState.position = a.GOTransform.getPosition();
	aState.velocity = a.getVelocity();

	State bState = State();
	bState.position = b.GOTransform.getPosition();
	bState.velocity = b.getVelocity();

	integrate(aState, bState, dt);

	a.setVelocity(aState.velocity);
	a.GOTransform.setPosition(aState.position);
	b.setVelocity(bState.velocity);
	b.GOTransform.setPosition(bState.position);

	/*glm::vec3 aPosition = glm::vec3(a.GOTransform.transformMatrix * aLoc);
	glm::vec3 bPosition = glm::vec3(b.GOTransform.transformMatrix * bLoc);

	glm::vec3 diff = bPosition - aPosition;
	float displacement = glm::length(diff) - restingLength;
	if (!std::isfinite(displacement))
	{
		displacement = std::numeric_limits<float>::max();
		diff *= 1.0f / diff.x;
	}

	diff = glm::normalize(diff);
	diff *= k * displacement * 0.5f;
	a.addForce(diff, aPosition);
	diff *= -1;
	b.addForce(diff, bPosition);*/
}

void Spring::draw() {
	glm::vec3 aPosition = glm::vec3(a.GOTransform.transformMatrix * aLoc);
	glm::vec3 bPosition = glm::vec3(b.GOTransform.transformMatrix * bLoc);
	glm::vec3 diff = bPosition - aPosition;

	this->transform->setPosition((aPosition + bPosition) * 0.5f);
	this->transform->setScale(glm::vec3(0.01f, glm::length(diff), 0.01f));
	//If angle is 0 or PI or 2PI, produces error
	glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), bPosition - aPosition);
	if (glm::length(axis) <= 0)
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	else
		axis = glm::normalize(axis);
	this->transform->setRotation(glm::angle(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(bPosition - aPosition)), glm::normalize(axis));

	GameObject::draw();
}


//Runge-Kutta integration section
void Spring::integrate(State& initialStateA, State& initialStateB, float dt)
{
	State d1A, d2A, d3A, d4A, d1B, d2B, d3B, d4B;
	getDerivative(initialStateA, initialStateB, &d1A, &d1B);
	getDerivative(initialStateA, initialStateB, &d2A, &d2B, dt * 0.5f, d1A, d1B);
	getDerivative(initialStateA, initialStateB, &d3A, &d3B, dt * 0.5f, d2A, d2B);
	getDerivative(initialStateA, initialStateB, &d4A, &d4B, dt * 0.5f, d3A, d3B);

	//Todo: finish this section (need to have 2 instances for each state);
	const glm::vec3 dXdTA = 1.0f / 6.0f * (d1A.position + 2.0f * (d2A.position + d3A.position) + d4A.position);
	const glm::vec3 dVdTA = 1.0f / 6.0f * (d1A.velocity + 2.0f * (d2A.velocity + d3A.velocity) + d4A.velocity);

	initialStateA.position = initialStateA.position + dXdTA * dt;
	initialStateA.velocity = initialStateA.velocity + dVdTA * dt;

	const glm::vec3 dXdTB = 1.0f / 6.0f * (d1B.position + 2.0f * (d2B.position + d3B.position) + d4B.position);
	const glm::vec3 dVdTB = 1.0f / 6.0f * (d1B.velocity + 2.0f * (d2B.velocity + d3B.velocity) + d4B.velocity);

	initialStateB.position = initialStateB.position + dXdTB * dt;
	initialStateB.velocity = initialStateB.velocity + dVdTB * dt;
}

void Spring::getDerivative(const State& initStateA, const State& initStateB, State* derivStateA, State* derivStateB)
{
	(*derivStateA).position = initStateA.velocity;
	(*derivStateB).position = initStateB.velocity;
	glm::vec3 accelA, accelB;
	getAcceleration(initStateA, initStateB, &accelA, &accelB);

	(*derivStateA).velocity = accelA;
	(*derivStateB).velocity = accelB;

}

void Spring::getDerivative(const State& initStateA, const State& initStateB, State* derivStateA, State* derivStateB, float dt, const State& dA, const State& dB)
{
	State stateA;
	stateA.position = initStateA.position + dA.position * dt;
	stateA.velocity = initStateA.velocity + dA.velocity * dt;

	State stateB;
	stateB.position = initStateB.position + dB.position * dt;
	stateB.velocity = initStateB.velocity + dB.velocity * dt;

	(*derivStateA).position = stateA.velocity;
	(*derivStateB).position = stateB.velocity;
	glm::vec3 accelA, accelB;
	getAcceleration(stateA, stateB, &accelA, &accelB);

	(*derivStateA).velocity = accelA;
	(*derivStateB).velocity = accelB;
}

void Spring::getAcceleration(const State& stateA, const State& stateB, glm::vec3* accelA, glm::vec3* accelB)
{
	const float q = 1.0f;
	glm::vec3 diff = stateA.position - stateB.position;
	float displacement = glm::length(diff) - restingLength;
	if (!std::isfinite(displacement))
	{
		displacement = std::numeric_limits<float>::max();
		diff *= 1.0f / diff.x;
	}
	diff = glm::normalize(diff);
	diff *= displacement * k * 0.5f;
	*(accelA) = -diff * a.inverseMass - q * stateA.velocity;
	*(accelB) = diff * b.inverseMass - q * stateB.velocity;
}