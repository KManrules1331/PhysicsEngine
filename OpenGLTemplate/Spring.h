#pragma once
#include "GameObject.h"
#include "PhysicsComponent.h"

class Spring :
	public GameObject
{
public:
	Spring(PhysicsComponent& nodeA, PhysicsComponent& nodeB, glm::vec3 nodeAAnchorPoint, glm::vec3 nodeBAnchorPoint, float k = 0.001f);
	~Spring();

	void update(float dt) override;
	void draw() override;

private:
	PhysicsComponent& a;
	PhysicsComponent& b;
	glm::vec4 aLoc;
	glm::vec4 bLoc;

	float restingLength;
	float k;

	//Runge-Kutta integration on each spring
	struct State {
		glm::vec3 position;
		glm::vec3 velocity;
	};

	void integrate(State& initStateA, State& initStateB, float dt);
	void getDerivative(const State& initStateA, const State& initStateB, State* derivStateA, State* derivStateB);
	void getDerivative(const State& initStateA, const State& initStateB, State* derivStateA, State* derivStateB, float dt, const State& dA, const State& dB);
	void getAcceleration(const State& stateA, const State& stateB, glm::vec3* accelA, glm::vec3* accelB);
};

