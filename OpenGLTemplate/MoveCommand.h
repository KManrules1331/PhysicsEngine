#pragma once
#include "Command.h"
#include "OpenGL.h"
#include "GameObject.h"

class MoveCommand :
	public Command
{
public:
	MoveCommand(glm::vec3 displacement, GameObject& actor);
	~MoveCommand();
	virtual void execute() override;

private:
	glm::vec3 displacement;
};

