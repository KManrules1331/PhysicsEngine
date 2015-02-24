#pragma once
#include "Command.h"
#include "OpenGL.h"
#include "GameObject.h"

class MoveCommand :
	public Command
{
public:
	MoveCommand(glm::vec3 displacement);
	~MoveCommand();
	virtual void execute(GameObject& actor) override;

private:
	glm::vec3 displacement;
};

